//
// Created by njz on 2023/1/30.
//

#include "executor/executors/update_executor.h"

UpdateExecutor::UpdateExecutor(ExecuteContext *exec_ctx, const UpdatePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

/**
* TODO: Student Implement
*/
void UpdateExecutor::Init() {
  child_executor_->Init();
  exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(),table_info);
}

bool UpdateExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {
  RowId delete_rid;
  //child_executor_->table_iterator=child_executor_->table_info->GetTableHeap()->Begin(exec_ctx_->GetTransaction());
  while(child_executor_->Next(row,rid)){
    Row new_row=GenerateUpdatedTuple(*row);
    //new_row.GetFieldCount();
    //row->GetRowId();
   // new_row.GetRowId()
   //row->GetFieldCount();
    vector<IndexInfo*>index_infos;
    exec_ctx_->GetCatalog()->GetIndexes(index_infos);
    for(auto it:index_infos){
      Row key_row;
      row->GetKeyFromRow(table_info->GetSchema(),it->GetIndex()->GetKeySchema(),key_row);
      it->GetIndex()->RemoveEntry(key_row,row->GetRowId(),exec_ctx_->GetTransaction());

    }
    child_executor_->table_info->GetTableHeap()->ApplyDelete(row->GetRowId(),exec_ctx_->GetTransaction());
    for(auto itr:index_infos){
      Row key_row2;
      new_row.GetKeyFromRow(table_info->GetSchema(),itr->GetIndex()->GetKeySchema(),key_row2);
      itr->GetIndex()->InsertEntry(key_row2,row->GetRowId(),exec_ctx_->GetTransaction());

    }
    child_executor_->table_info->GetTableHeap()->InsertTuple(new_row,exec_ctx_->GetTransaction());
  }
  return false;
}

Row UpdateExecutor::GenerateUpdatedTuple(const Row &src_row) {
  Row new_row = Row(src_row);
  for(auto const &entry : plan_->update_attrs_) {
   //cout<<entry.first<<endl;
  new_row.SetFieldAt(entry.first,Field(entry.second->Evaluate(&new_row)));

  }
  return new_row;
}