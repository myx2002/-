#include "storage/table_heap.h"

/**
 * TODO: Student Implement
 */
bool TableHeap::InsertTuple(Row &row, Transaction *txn) {
 //获得row对映的tablepage最大支持size
  size_t max_size_row = TablePage::SIZE_MAX_ROW;
  //如果单行数据超过此大小，返回false

  if(row.GetSerializedSize(schema_) > max_size_row)
    return false;
  //从table中第一逻辑页开始遍历获得free page用于填入该row
  //第一页的page_id不会为INVALID_PAGE_ID，与初始化实现相关
  page_id_t next_page_id = GetFirstPageId();
 int x= reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(next_page_id))->GetFreeSpacePointer();
  //next_page_id为无效值时返回nullptr
  TablePage* true_page;

  while(1)
  {
    //firstpageid不可能为无效值
    if(next_page_id == INVALID_PAGE_ID)
    {
      //没有有效页，则尝试分配新页
      TablePage* new_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->NewPage(next_page_id));
      //若分配失败，则返回false
      if(new_page == nullptr)
        return false;
      //对成功分配页面初始化
      new_page->Init(next_page_id,true_page->GetPageId(),log_manager_,txn);
     // new_page->WLatch();
      //向其中插入元素
      new_page->InsertTuple(row,schema_ ,txn,lock_manager_,log_manager_);
     // new_page->WUnlatch();
      //newpage函数默认pin页，此处unpin
      buffer_pool_manager_->UnpinPage(next_page_id,true);
      //修改原数据页的nextpageid记录并unpin写回
      true_page->SetNextPageId(next_page_id);
      buffer_pool_manager_->UnpinPage(true_page->GetPageId(),true);
      return true;
    }
    else//函数从这里开始执行，因为firstpageid不可能为无效值
         {

      true_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(next_page_id));
    //可能存在分配错误的情况
    if(true_page == nullptr)
      return false;
    //进行写入操作
    //上锁
   // true_page->WLatch();
    //尝试在当前true_page插入row
    //return true;
    if(true_page->InsertTuple(row,schema_,txn,lock_manager_,log_manager_))
    {
      buffer_pool_manager_->UnpinPage(next_page_id,true);
      //true_page->WUnlatch();
      return true;
    }
    //true_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(next_page_id,false);
    //next_page_id/true_page的遍历
    next_page_id = true_page->GetNextPageId();

  }}
  return true;
}

bool TableHeap::MarkDelete(const RowId &rid, Transaction *txn) {
  // Find the page which contains the tuple.
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  // If the page could not be found, then abort the transaction.
  if (page == nullptr) {
    return false;
  }
  // Otherwise, mark the tuple as deleted.
  page->WLatch();
  page->MarkDelete(rid, txn, lock_manager_, log_manager_);
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
  return true;
}

/**
 * TODO: Student Implement
 */
bool TableHeap::UpdateTuple(const Row &row, const RowId &rid, Transaction *txn) {
  //rid非法直接返回false
  if(rid == INVALID_ROWID)
    return false;
  //获得原数据对映的数据页
  TablePage* true_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  if(true_page == nullptr)
    return false;
  //通过rowid唯一标识建立row对象
  Row ori_row = Row(rid);
  //gettuple函数本身已上锁
  //true_page->RLatch();
  bool get_tuple_result = GetTuple(&ori_row,txn);
  //true_page->RUnlatch();
  if(get_tuple_result == false)
    return false;
  //注意TablePage::UpdateTuple返回值已修改，因而定义int类型接受结果
  int update_tuple_result;
  true_page->WLatch();
  //尝试更新对映的数据页
  update_tuple_result = true_page->UpdateTuple(row,&ori_row,schema_,txn,lock_manager_,log_manager_);
  true_page->WUnlatch();
  //若返回值为1，表示更新成功，可以返回true
  if(update_tuple_result == 0)
  {
    buffer_pool_manager_->UnpinPage(true_page->GetTablePageId(),true);
    return true;
  }
  //若tuple已被删除或slot_num越界，直接返回false
  else if(update_tuple_result == -1 || update_tuple_result == 1)
  {
    buffer_pool_manager_->UnpinPage(true_page->GetTablePageId(),false);
    return false;
  }
  //特殊情况，因为位置不足而发生false的返回，本函数应该如何处理
  else if(update_tuple_result == 2)
  {
    buffer_pool_manager_->UnpinPage(true_page->GetTablePageId(),false);

    return false;
  }
  //以防奇怪的返回值
  return false;
}

/**
 * TODO: Student Implement
 */
void TableHeap::ApplyDelete(const RowId &rid, Transaction *txn) {
  // Step1: Find the page which contains the tuple.
  // Step2: Delete the tuple from the page.
  TablePage* true_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  //没有返回值，先用assert函数进行界定
  ASSERT(true_page != nullptr,"Page not exist!");
  true_page->WLatch();
  //注意是单挑记录的删除，非page
  true_page->ApplyDelete(rid,txn,log_manager_);
  true_page->WUnlatch();
  buffer_pool_manager_->UnpinPage(true_page->GetTablePageId(),true);
}

void TableHeap::RollbackDelete(const RowId &rid, Transaction *txn) {
  // Find the page which contains the tuple.
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  assert(page != nullptr);
  // Rollback to delete.
  page->WLatch();
  page->RollbackDelete(rid, txn, log_manager_);
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
}

/**
 * TODO: Student Implement
 */
bool TableHeap::GetTuple(Row *row, Transaction *txn) {
  //借助row对象获得对映数据页
  TablePage* true_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(row->GetRowId().GetPageId()));
  //若数据页不存在，直接返回false
  if(true_page == nullptr)
    return false;
  //true_page->RLatch();
  //读取对映数据并返回判断结果
  bool gettuple_result = true_page->GetTuple(row,schema_,txn,lock_manager_);
  //true_page->RUnlatch();
  //unpin对映数据页
  buffer_pool_manager_->UnpinPage(true_page->GetTablePageId(),false);
  if(gettuple_result)
    //读取成功unpin后返回true
    return true;
  //其余返回false
  else
    return false;
}

void TableHeap::DeleteTable(page_id_t page_id) {
  if (page_id != INVALID_PAGE_ID) {
    auto temp_table_page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(page_id));  // 删除table_heap
    if (temp_table_page->GetNextPageId() != INVALID_PAGE_ID)
      DeleteTable(temp_table_page->GetNextPageId());
    buffer_pool_manager_->UnpinPage(page_id, false);
    buffer_pool_manager_->DeletePage(page_id);
  } else {
    DeleteTable(first_page_id_);
  }
}

/**
 * TODO: Student Implement
 */
TableIterator TableHeap::Begin(Transaction *txn) {
  //注意fetch已pin页面
  RowId now_rowid;
  //根据第一逻辑页获得对映的具体数据页

  TablePage* true_page = reinterpret_cast<TablePage*>(buffer_pool_manager_->FetchPage(first_page_id_));
  //cout<<first_page_id_<<endl;
  //添加锁
  //true_page->WLatch();
  //从数据页中获得对映的row对象，用于迭代器的构建
  true_page->GetFirstTupleRid(&now_rowid);
  //cout<<"25"<<endl;
  //打开锁
 // true_page->WUnlatch();
  //unpin使用的页，未改变页面内容
  buffer_pool_manager_->UnpinPage(first_page_id_,false);
  //cout<<"26"<<endl;
  return TableIterator(this,now_rowid);
}

/**
 * TODO: Student Implement
 */
TableIterator TableHeap::End() {
  return TableIterator(this,INVALID_ROWID);
}
