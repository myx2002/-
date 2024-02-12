//
// Created by njz on 2023/1/27.
//

#include "executor/executors/insert_executor.h"

InsertExecutor::InsertExecutor(ExecuteContext *exec_ctx, const InsertPlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

void InsertExecutor::Init() {
  child_executor_->Init();
  exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(),table_info);

  //TableHeap* table_heap=table_info->GetTableHeap();
}

bool InsertExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {
  while(child_executor_->Next(row,rid)){
    TableHeap* table_heap=table_info->GetTableHeap();
    //table_heap->InsertTuple(*row,exec_ctx_->GetTransaction());
    vector<IndexInfo*>index_infos;
    exec_ctx_->GetCatalog()->GetIndexes(index_infos);
    int is_conflict=0;
    for(auto it:index_infos) {
      Row key_row;
      row->GetKeyFromRow(table_info->GetSchema(), it->GetIndex()->GetKeySchema(), key_row);
      vector<RowId> result;
      it->GetIndex()->ScanKey(key_row, result, exec_ctx_->GetTransaction());

      if (result.size() > 0) {
        is_conflict++;
        cout << "Conflict on " + it->GetIndexName() << endl;
        break;
      }
    }
    if(is_conflict==0){
      table_heap->InsertTuple(*row,exec_ctx_->GetTransaction());

      for(auto it:index_infos) {
        Row key_row;
        row->GetKeyFromRow(table_info->GetSchema(), it->GetIndex()->GetKeySchema(), key_row);
        it->GetIndex()->InsertEntry(key_row, row->GetRowId(), exec_ctx_->GetTransaction());
      }

    }

  }


  return false;
}