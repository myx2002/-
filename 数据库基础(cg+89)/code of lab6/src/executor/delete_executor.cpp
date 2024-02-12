//
// Created by njz on 2023/1/29.
//

#include "executor/executors/delete_executor.h"

/**
* TODO: Student Implement
*/

DeleteExecutor::DeleteExecutor(ExecuteContext *exec_ctx, const DeletePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {

}

void DeleteExecutor::Init() {
  child_executor_->Init();
  exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(),table_info);
}

bool DeleteExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {

 RowId delete_rid;
 //child_executor_->table_iterator=child_executor_->table_info->GetTableHeap()->Begin(exec_ctx_->GetTransaction());
  while(child_executor_->Next(row,&delete_rid)){
    vector<IndexInfo*>index_infos;
    exec_ctx_->GetCatalog()->GetIndexes(index_infos);
    for(auto it:index_infos){
      Row key_row;
      row->GetKeyFromRow(table_info->GetSchema(),it->GetIndex()->GetKeySchema(),key_row);
      it->GetIndex()->RemoveEntry(key_row,row->GetRowId(),exec_ctx_->GetTransaction());

    }
    //row->GetRowId();
     child_executor_->table_info->GetTableHeap()->ApplyDelete(delete_rid,exec_ctx_->GetTransaction());


  }
  return false;
}