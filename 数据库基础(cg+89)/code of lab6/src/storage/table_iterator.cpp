#include "storage/table_iterator.h"

#include "common/macros.h"
#include "storage/table_heap.h"

/**
 * TODO: Student Implement
 */

TableIterator::TableIterator(TableHeap* new_tableheap, RowId rid) {
  ASSERT(new_tableheap != nullptr,"Empty pointer does not have an iterator!");
  //若new_tableheap不为nullptr时，直接赋值即可
  //因为不会去改变，所以即使是指针也可以直接复制
  it_tableheap = new_tableheap;
  //cout<<"300"<<endl;
  //无论rid数值如何，均可以直接构建Row对象
  it_row = new Row(rid);
//  it_row->GetRowId();
  //若rid不为无效对象，需要给it_row分配对象数据
  //注意rowid只重载了==，没有重载!=
  if(rid == INVALID_ROWID);
  //cout<<"500"<<endl;
  else {
  // tranction参数好像无所谓，class都没有定义
  it_tableheap->GetTuple(it_row, nullptr);
  // it_row->GetFieldCount();
  }
}

TableIterator::TableIterator(const TableIterator &other) {
  it_tableheap = other.it_tableheap;

  it_row = new Row(other.it_row->GetRowId());
  it_tableheap->GetTuple(it_row,nullptr);

}

TableIterator::~TableIterator() {
  //delete it_row;
}

bool TableIterator::operator==(const TableIterator &itr) const {
  if(it_row->GetRowId() == itr.it_row->GetRowId())
    return true;
  else
    return false;
}

bool TableIterator::operator!=(const TableIterator &itr) const {
  if(*this == itr)
    return false;
  else
    return true;
}

const Row &TableIterator::operator*() {
  return *it_row;
}

Row *TableIterator::operator->() {
    return it_row;
}

TableIterator &TableIterator::operator=(const TableIterator &itr) noexcept {
    //delete it_row;
    it_row = new Row(itr.it_row->GetRowId());
    it_tableheap = itr.it_tableheap;
    if(itr.it_row->GetRowId()== INVALID_ROWID);
    else
    it_tableheap->GetTuple(it_row,nullptr);
    return *this;

}

// ++iter
TableIterator &TableIterator::operator++() {
    //需要先判断it_row的合法性
    //特别注意，rowid没有重载!=运算符
    ASSERT(!(it_row->GetRowId() == INVALID_ROWID),"RowId is illegal!");
    //获得当前it_row对映的page_id（依稀记得page_id是逻辑页）,便于pin页unpin页操作
    page_id_t it_page_id = it_row->GetRowId().GetPageId();
    //获得对映的逻辑数据页
    //注意FetchPage已经将对映的page pin固定了，最后需要unpin
    TablePage* true_page = reinterpret_cast<TablePage*>(it_tableheap->buffer_pool_manager_->FetchPage(it_page_id));
    //通过rowid获得++后的结果
    RowId next_rowid;
    //写数据上锁,确保一致性（尝试）
    true_page->RLatch();
    //判断下一rowid是否在本页，若在本页，则直接读取
    bool next_rowid_result = true_page->GetNextTupleRid(it_row->GetRowId(),&next_rowid);
    //若搜得next_rowid且已经返回
    if(next_rowid_result)
    {
    //注意指针的赋值，不能直接作等
    delete it_row;
    it_row = new Row(next_rowid);
    }
    else
    {
    //本页判断完成，解开read latch并unpin该页面
    true_page->RUnlatch();
    it_tableheap->buffer_pool_manager_->UnpinPage(it_page_id,false);
    //next_rowid/it_page_id/true_page/next_rowid_result可继续使用
    while(1)
    {
      //获取下一page_id
      it_page_id = true_page->GetNextPageId();
      //若page_id已至结尾，说明没有下一rowid，重新构造unvalid it_row并返回
      if(it_page_id == INVALID_PAGE_ID)
      {
        delete it_row;
        it_row = new Row(INVALID_ROWID);
        return *this;
      }
      //通过逻辑页号取回数据页
      //注意fetch已pin页面
      true_page = reinterpret_cast<TablePage*>(it_tableheap->buffer_pool_manager_->FetchPage(it_page_id));
      //上锁
      true_page->RLatch();
      //判断首个rowid是否为下一个rowid，因为顺序存放，所以非本页时，必然为下一页的第一项（下一页可能不存在，所以需要遍历）
      if(true_page->GetFirstTupleRid(&next_rowid))
      {
        delete it_row;
        //若寻得则直接获得相应rowid用于更新迭代器
        it_row = new Row(next_rowid);
        break;
      }
      //解锁
      true_page->RUnlatch();
      //unpin使用页面
      it_tableheap->buffer_pool_manager_->UnpinPage(it_page_id,false);
    }
    }
    //对于读到it_tableheap结尾的情况，不需要再读取数据
    if(it_row->GetRowId().GetPageId() != INVALID_PAGE_ID)
    it_tableheap->GetTuple(it_row,nullptr);
    true_page->RUnlatch();
    //unpin该数据页
    //仅读数据，is_dirty无需置位
    it_tableheap->buffer_pool_manager_->UnpinPage(it_page_id,false);
    return *this;
}

// iter++
TableIterator TableIterator::operator++(int) {
  TableIterator temp(it_tableheap,it_row->GetRowId());
  //利用前置完成自增操作
  ++(*this);
  //没办法直接返回temp，故使用拷贝赋值函数
  return TableIterator(temp);
}
