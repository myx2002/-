//
// Created by njz on 2023/1/17.
//
#include "executor/executors/seq_scan_executor.h"

/**
* TODO: Student Implement
*/
SeqScanExecutor::SeqScanExecutor(ExecuteContext *exec_ctx, const SeqScanPlanNode *plan)
    : AbstractExecutor(exec_ctx),
      plan_(plan), is_Init(false){

}

void SeqScanExecutor::Init() {
  if(is_Init==false){
    //cout<<"1"<<endl;
  exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(),table_info);
 //cout<<"2"<<endl;
  TableHeap* table_heap=table_info->GetTableHeap();
  //cout<<"4"<<endl;
  table_iterator=table_heap->Begin(exec_ctx_->GetTransaction());
  //cout<<"3"<<endl;
  // cout <<table_iterator->GetRowId().GetSlotNum()<<endl;
  is_Init=true;
}
}

bool SeqScanExecutor::Next(Row *row, RowId *rid) {
  //cout<<"here3"<<endl;
if (table_iterator == table_info->GetTableHeap()->End()) {
 // cout<<"here"<<endl;
  is_Init = false;
  return false;
}
  while(!(table_iterator==(table_info->GetTableHeap()->End()))){
//cout<<"here2"<<endl;
    *row=*table_iterator;
  if (plan_ ->GetPredicate() == nullptr){
    Row new_row;

  row->GetKeyFromRow(table_info ->GetSchema(),plan_ ->OutputSchema(), new_row);
  new_row.SetRowId(row->GetRowId());
  *row = new_row;
  *rid = row->GetRowId();
  table_iterator ++;
  return true;
  }
  auto eva_res = plan_->GetPredicate()->Evaluate(row);
  if (eva_res.CompareEquals(Field(kTypeInt,1)) == CmpBool::kTrue){

    Row new_row;
  row->GetKeyFromRow(table_info ->GetSchema(),plan_->OutputSchema(), new_row);
  new_row.SetRowId(row->GetRowId());
  *row = new_row;
  //row->GetRowId();
  *rid = row->GetRowId();
  RowId a=row->GetRowId();
  table_iterator++;
  return true;
  }
  else{
      table_iterator ++;

  }
  }
  return false;
}
