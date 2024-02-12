#include "executor/execute_engine.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <chrono>

#include "common/result_writer.h"
#include "executor/executors/delete_executor.h"
#include "executor/executors/index_scan_executor.h"
#include "executor/executors/insert_executor.h"
#include "executor/executors/seq_scan_executor.h"
#include "executor/executors/update_executor.h"
#include "executor/executors/values_executor.h"
#include "glog/logging.h"
#include "planner/planner.h"
#include "utils/utils.h"

ExecuteEngine::ExecuteEngine() {
  char path[] = "./databases";
  DIR *dir;
  if((dir = opendir(path)) == nullptr) {
    mkdir("./databases", 0777);
    dir = opendir(path);
  }

  struct dirent *stdir;
  while((stdir = readdir(dir)) != nullptr) {
    if( strcmp( stdir->d_name , "." ) == 0 ||
        strcmp( stdir->d_name , "..") == 0 ||
        stdir->d_name[0] == '.')
      continue;
    //cout<<stdir->d_name<<endl;
    dbs_[stdir->d_name] = new DBStorageEngine(stdir->d_name, false);
  }

  closedir(dir);
}

std::unique_ptr<AbstractExecutor> ExecuteEngine::CreateExecutor(ExecuteContext *exec_ctx,
                                                                const AbstractPlanNodeRef &plan) {
  switch (plan->GetType()) {
    // Create a new sequential scan executor
    case PlanType::SeqScan: {
      return std::make_unique<SeqScanExecutor>(exec_ctx, dynamic_cast<const SeqScanPlanNode *>(plan.get()));
    }
    // Create a new index scan executor
    case PlanType::IndexScan: {
      return std::make_unique<IndexScanExecutor>(exec_ctx, dynamic_cast<const IndexScanPlanNode *>(plan.get()));
    }
    // Create a new update executor
    case PlanType::Update: {
      auto update_plan = dynamic_cast<const UpdatePlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, update_plan->GetChildPlan());
      return std::make_unique<UpdateExecutor>(exec_ctx, update_plan, std::move(child_executor));
    }
      // Create a new delete executor
    case PlanType::Delete: {
      auto delete_plan = dynamic_cast<const DeletePlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, delete_plan->GetChildPlan());
      return std::make_unique<DeleteExecutor>(exec_ctx, delete_plan, std::move(child_executor));
    }
    case PlanType::Insert: {
      auto insert_plan = dynamic_cast<const InsertPlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, insert_plan->GetChildPlan());
      return std::make_unique<InsertExecutor>(exec_ctx, insert_plan, std::move(child_executor));
    }
    case PlanType::Values: {
      return std::make_unique<ValuesExecutor>(exec_ctx, dynamic_cast<const ValuesPlanNode *>(plan.get()));
    }
    default:
      throw std::logic_error("Unsupported plan type.");
  }

}

dberr_t ExecuteEngine::ExecutePlan(const AbstractPlanNodeRef &plan, std::vector<Row> *result_set, Transaction *txn,
                                   ExecuteContext *exec_ctx) {
  // Construct the executor for the abstract plan node
  auto executor = CreateExecutor(exec_ctx, plan);

  try {

    executor->Init();
    RowId rid{};
    Row row{};
    while (executor->Next(&row, &rid)) {

      if (result_set != nullptr) {
        result_set->push_back(row);
      }
    }
  } catch (const exception &ex) {
    std::cout << "Error Encountered in Executor Execution: " << ex.what() << std::endl;
    if (result_set != nullptr) {
      result_set->clear();
    }
    return DB_FAILED;
  }
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::Execute(pSyntaxNode ast) {
  if (ast == nullptr) {
    return DB_FAILED;
  }
  auto start_time = std::chrono::system_clock::now();
  unique_ptr<ExecuteContext> context(nullptr);
  if(!current_db_.empty())
    context = dbs_[current_db_]->MakeExecuteContext(nullptr);
  switch (ast->type_) {
    case kNodeCreateDB:
      return ExecuteCreateDatabase(ast, context.get());
    case kNodeDropDB:
      return ExecuteDropDatabase(ast, context.get());
    case kNodeShowDB:
      return ExecuteShowDatabases(ast, context.get());
    case kNodeUseDB:
      return ExecuteUseDatabase(ast, context.get());
    case kNodeShowTables:
      return ExecuteShowTables(ast, context.get());
    case kNodeCreateTable:
      return ExecuteCreateTable(ast, context.get());
    case kNodeDropTable:
      return ExecuteDropTable(ast, context.get());
    case kNodeShowIndexes:
      return ExecuteShowIndexes(ast, context.get());
    case kNodeCreateIndex:
      return ExecuteCreateIndex(ast, context.get());
    case kNodeDropIndex:
      return ExecuteDropIndex(ast, context.get());
    case kNodeTrxBegin:
      return ExecuteTrxBegin(ast, context.get());
    case kNodeTrxCommit:
      return ExecuteTrxCommit(ast, context.get());
    case kNodeTrxRollback:
      return ExecuteTrxRollback(ast, context.get());
    case kNodeExecFile:
      return ExecuteExecfile(ast, context.get());
    case kNodeQuit:
      return ExecuteQuit(ast, context.get());
    default:
      break;
  }
  // Plan the query.
  Planner planner(context.get());
  std::vector<Row> result_set{};
  try {
    planner.PlanQuery(ast);
    // Execute the query.
    ExecutePlan(planner.plan_, &result_set, nullptr, context.get());
  } catch (const exception &ex) {
    std::cout << "Error Encountered in Planner: " << ex.what() << std::endl;
    return DB_FAILED;
  }
  auto stop_time = std::chrono::system_clock::now();
  double duration_time =
      double((std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time)).count());
  // Return the result set as string.
  std::stringstream ss;
  ResultWriter writer(ss);

  if (planner.plan_->GetType() == PlanType::SeqScan || planner.plan_->GetType() == PlanType::IndexScan) {
    auto schema = planner.plan_->OutputSchema();
    auto num_of_columns = schema->GetColumnCount();
    if (!result_set.empty()) {
      // find the max width for each column
      vector<int> data_width(num_of_columns, 0);
      for (const auto &row : result_set) {
        for (uint32_t i = 0; i < num_of_columns; i++) {
          data_width[i] = max(data_width[i], int(row.GetField(i)->toString().size()));
        }
      }
      int k = 0;
      for (const auto &column : schema->GetColumns()) {
        data_width[k] = max(data_width[k], int(column->GetName().length()));
        k++;
      }
      // Generate header for the result set.
      writer.Divider(data_width);
      k = 0;
      writer.BeginRow();
      for (const auto &column : schema->GetColumns()) {
        writer.WriteHeaderCell(column->GetName(), data_width[k++]);
      }
      writer.EndRow();
      writer.Divider(data_width);

      // Transforming result set into strings.
      for (const auto &row : result_set) {
        writer.BeginRow();
        for (uint32_t i = 0; i < schema->GetColumnCount(); i++) {
          writer.WriteCell(row.GetField(i)->toString(), data_width[i]);
        }
        writer.EndRow();
      }
      writer.Divider(data_width);
    }
    writer.EndInformation(result_set.size(), duration_time, true);
  } else {
    writer.EndInformation(result_set.size(), duration_time, false);
  }
  std::cout << writer.stream_.rdbuf();
  return DB_SUCCESS;
}

void ExecuteEngine::ExecuteInformation(dberr_t result) {
  switch (result) {
    case DB_ALREADY_EXIST:
      cout << "Database already exists." << endl;
      break;
    case DB_NOT_EXIST:
      cout << "Database not exists." << endl;
      break;
    case DB_TABLE_ALREADY_EXIST:
      cout << "Table already exists." << endl;
      break;
    case DB_TABLE_NOT_EXIST:
      cout << "Table not exists." << endl;
      break;
    case DB_INDEX_ALREADY_EXIST:
      cout << "Index already exists." << endl;
      break;
    case DB_INDEX_NOT_FOUND:
      cout << "Index not exists." << endl;
      break;
    case DB_COLUMN_NAME_NOT_EXIST:
      cout << "Column not exists." << endl;
      break;
    case DB_KEY_NOT_FOUND:
      cout << "Key not exists." << endl;
      break;
    case DB_QUIT:
      cout << "Bye." << endl;
      break;
    default:
      break;
  }
}
/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteCreateDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateDatabase" << std::endl;
#endif
  if ( !ast->child_->val_) {
    cout<<"failed"<<endl;
    return DB_FAILED;
  }
  std::string db_name = string(ast->child_->val_);
  if (dbs_.count(db_name)) {
    return DB_ALREADY_EXIST;
  }

  DBStorageEngine* db = new DBStorageEngine(db_name,true);
  dbs_[db_name] = db;
  cout<<"Create database successfully"<<endl;
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropDatabase" << std::endl;
#endif
  if (!ast->child_->val_) {
    cout<<"failed"<<endl;
    return DB_FAILED;
  }

  std::string db_name = string(ast->child_->val_);
  if (!dbs_.count(db_name)) {

    return DB_NOT_EXIST;
  }

  delete dbs_[db_name];
  dbs_.erase(db_name);
  cout<<"Drop database successfully "<<endl;
  return DB_SUCCESS;

}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowDatabases(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowDatabases" << std::endl;
#endif
  printf("DATABASES:\n");
  for (auto it : dbs_) {
    cout<<it.first<<endl;

  }

  return DB_SUCCESS;

}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteUseDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteUseDatabase" << std::endl;
#endif
  if ( !ast->child_->val_) {
    cout<<"failed"<<endl;
    return DB_FAILED;
  }
  std::string db_name =ast->child_->val_;//获取nameif(!this->dbs_.count(name)) return DB NOT_EXIST;
  if(!this->dbs_.count(db_name)) {
    cout << "Database do not exist" << endl;
    return DB_NOT_EXIST;
  }
  current_db_ = db_name;
  cout<<"Use database successfully"<<endl;
  return DB_SUCCESS;

}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowTables(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowTables" << std::endl;
#endif
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  vector<string> table_names;
  context->GetCatalog()->GetTableNames(table_names);
  for (const auto& name : table_names) {
    std::cout << name << endl;
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */

dberr_t ExecuteEngine::ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateTable" << std::endl;
#endif

  std::ofstream ofs("output.txt", std::ios::out);
  SyntaxTreePrinter a= SyntaxTreePrinter(ast);
  a.PrintTree(ofs);
  if(!ast->child_->val_){
    cout <<"failed"<<endl;
    return DB_FAILED;}
  std::string table_name = string(ast->child_->val_);
  TableInfo *table_info= nullptr;
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }

  if(context->GetCatalog()->GetTable(table_name,table_info)==DB_SUCCESS){
    return DB_TABLE_ALREADY_EXIST;
  }
  vector<string>val_vector;
  AstToVector(ast,val_vector);
  std::vector<Column *> columns;
  // iterate over the tokens and parse them to build the schema

  vector<vector<string>>index_vector;
  vector<string> index_names;;
  //cout<<index_name<<endl;
 vector<vector<string>> index_keys;
  for (size_t i = 1; i < val_vector.size(); ++i) {
    if (val_vector[i] == "primary keys") {
      // skip primary key definition
       index_names.push_back("_<" + table_name + ">_primary_keys_");
      //cout<<index_name<<endl;
      //cout<<table_name<<endl;
      vector<string> index_key;
      for (int j=i+1; j < val_vector.size() ; j++){

        index_key.push_back(val_vector[j]);
        //cout<<val_vector[j]<<endl;
      }
      /* for(auto it:index_keys){
         cout<<it<<endl;
       }*/
       index_keys.push_back(index_key);
      i += 1000;
    } else {


      TypeId type;
      int length = 0;
      bool unique = false;
      if (i + 1 < val_vector.size() && val_vector[i ] == "unique") {
        vector<string> index_key2;
        index_key2.push_back(val_vector[i+1]);
        index_names.push_back("_<"+table_name+">_"+"unique_attribute_index ("+val_vector[i+1]+")");
        index_keys.push_back(index_key2);
        unique = true;
        i++;

      }
      std::string column_name = val_vector[i];


      // check if this column is a char column with a specified length
      if (i + 2 < val_vector.size() && val_vector[i+1 ] == "char" ) {
        type = TypeId::kTypeChar;
        length = std::stoi(val_vector[i+2]);
        if(length <= 0){
          cout<<"The length of char type is not allowed to be less than zero"<<endl;
          return DB_FAILED;
        }

        i += 2;
      } else {
        // otherwise, assume it's an int or float column
        if (val_vector[i+1 ] == "int") {
          type = TypeId::kTypeInt;
        } else {
          type = TypeId::kTypeFloat;
        }
        i+=1;
      }

      // check if this column has a UNIQUE constraint


      // add the new column to the schema
      if(type==TypeId::kTypeChar)
        columns.push_back(new Column(column_name, type,length, static_cast<uint32_t>(columns.size()),true, unique));
      else{
        columns.push_back(new Column(column_name, type, static_cast<uint32_t>(columns.size()),true, unique));
      }
    }

  }   auto table_schema = std::make_shared<Schema>(columns);

  context->GetCatalog()->CreateTable(table_name, table_schema.get(), context->GetTransaction(), table_info);
 /* cout<<table_info->GetTableName()<<endl;
  for(auto it:table_info->GetSchema()->GetColumns()){
    cout<<it->GetName()<<endl;
  }*/
  ASSERT(table_info != nullptr,"Failed to create table");
 // context->GetCatalog()->CreateTable()
  IndexInfo *index_info = nullptr;


  // IndexInfo *index_info2 = nullptr;
  // std::vector<std::string> index_keys2{"a", "b"};
   for(int i=0;i<index_names.size();i++){
    context->GetCatalog()->CreateIndex(table_name,index_names[i],index_keys[i],context->GetTransaction(),index_info,"bptree");
   }

 /* std::vector<std::string> index_keys{"a", "b"};
  context->GetCatalog()->CreateIndex("t1","index_name",index_keys,context->GetTransaction(),index_info,"bptree");*/

  cout<<"Creates table successfully"<<endl;
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropTable" << std::endl;
#endif
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  if(!ast->child_->val_){
    cout <<"failed"<<endl;
    return DB_FAILED;}
  std::string table_name = string(ast->child_->val_);
  if(context->GetCatalog()->DropTable(table_name)==DB_SUCCESS){
    cout<<"Drops table successfully."<<endl;
    return DB_SUCCESS;
  }
  else{
    return DB_TABLE_NOT_EXIST;
  }


}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowIndexes" << std::endl;
#endif
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  vector<string> index_names;
  context->GetCatalog()->GetIndexNames(index_names);
  for (const auto& name : index_names) {
    std::cout << name << endl;
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateIndex" << std::endl;
#endif
  std::ofstream ofs("output.txt", std::ios::out);
  SyntaxTreePrinter a= SyntaxTreePrinter(ast);
  a.PrintTree(ofs);
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  if(!ast->child_->val_){
    cout <<"failed"<<endl;
    return DB_FAILED;
  }
  vector<string>val_vector;
  AstToVector(ast,val_vector);
  string index_name=val_vector[0];
  string table_name=val_vector[1];
  vector<string> index_keys;
  //cout<<index_name<<endl;
  //cout<<table_name<<endl;
  for (int i = 3; i < val_vector.size() ; i++){
    index_keys.push_back(val_vector[i]);
  }
 /* for(auto it:index_keys){
    cout<<it<<endl;
  }*/
  IndexInfo *index_info = nullptr;

 // IndexInfo *index_info2 = nullptr;
 // std::vector<std::string> index_keys2{"a", "b"};

 auto result=context->GetCatalog()->CreateIndex(table_name,index_name,index_keys,context->GetTransaction(),index_info,"bptree");
 if(result==DB_SUCCESS){
   TableInfo* table_info= nullptr;
   context->GetCatalog()->GetTable(table_name,table_info);
   TableHeap* table_heap=table_info->GetTableHeap();
   TableIterator table_iterator=table_heap->Begin(context->GetTransaction());
   while(!(table_iterator==(table_info->GetTableHeap()->End()))){
      Row row=*table_iterator;
      Row new_row;
      row.GetKeyFromRow(table_info->GetSchema(), index_info->GetIndex()->GetKeySchema(), new_row);
      index_info->GetIndex()->InsertEntry(new_row, row.GetRowId(), context->GetTransaction());
      table_iterator ++;
   }
    cout<<"Creates index successfully."<<endl;}
  return result;

}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropIndex" << std::endl;
#endif
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  if(!ast->child_->val_){
    cout <<"failed"<<endl;
    return DB_FAILED;}
  std::string index_name = string(ast->child_->val_);
  auto result=context->GetCatalog()->DropIndex(index_name);
  if(result==DB_SUCCESS)
    cout<<"Drops index successfully."<<endl;
  return  result;
}


dberr_t ExecuteEngine::ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxBegin" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxCommit" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxRollback" << std::endl;
#endif
  return DB_FAILED;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteExecfile" << std::endl;
#endif
  if(!dbs_[current_db_]){
    cout<<"You are not using any database,please choose one"<<endl;
    return DB_FAILED;
  }
  string filename=string(ast->child_->val_);
  std::ifstream infile;
  infile.open(filename);
  if (!infile) {
    printf("Failed to open file: %s\n", filename.c_str());
    return DB_FAILED;
  }

  // read file line by line
  std::string line;
  while (std::getline(infile, line)) {
    // create buffer for sql input
    const int buf_size = line.size() + 1;
    char cmd[buf_size];
    strncpy(cmd, line.c_str(), buf_size);

    // parse SQL statement
    YY_BUFFER_STATE bp = yy_scan_string(cmd);
    if (bp == nullptr) {
      LOG(ERROR) << "Failed to create yy buffer state." << std::endl;
      exit(1);
    }
    yy_switch_to_buffer(bp);
    MinisqlParserInit();
    yyparse();

    // execute parsed SQL statement
    auto result = Execute(MinisqlGetParserRootNode());
    // clean up memory after parsing
    MinisqlParserFinish();
    yy_delete_buffer(bp);
    yylex_destroy();
    ExecuteInformation(result);
    if (result == DB_QUIT) {
      break;
    }

  }

  infile.close();
  return DB_SUCCESS;
  return DB_FAILED;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteQuit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteQuit" << std::endl;
#endif
  return DB_QUIT;
  return DB_FAILED;
}
