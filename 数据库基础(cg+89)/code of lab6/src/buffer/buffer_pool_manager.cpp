#include "buffer/buffer_pool_manager.h"

#include "glog/logging.h"//
#include "page/bitmap_page.h"

static const char EMPTY_PAGE_DATA[PAGE_SIZE] = {0};

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager) {
  pages_ = new Page[pool_size_];
  replacer_ = new SetReplacer(pool_size_);
  for (size_t i = 0; i < pool_size_; i++) {
    free_list_.emplace_back(i);
  }
}

BufferPoolManager::~BufferPoolManager() {
  for (auto page : page_table_) {
    FlushPage(page.first);
  }
  delete[] pages_;
  delete replacer_;
}

/**
 * TODO: Student Implement
 */
Page *BufferPoolManager::FetchPage(page_id_t page_id) {
  // 1.     Search the page table for the requested page (P).
  // 1.1    If P exists, pin it and return it immediately.
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  // 2.     If R is dirty, write it back to the disk.
  // 3.     Delete R from the page table and insert P.
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
  frame_id_t frame_id;
  if (page_table_.find(page_id) != page_table_.end()) {//存在
    frame_id= (page_table_.find(page_id))->second;
    pages_[frame_id].pin_count_++;
    replacer_->Pin(frame_id);
    return &pages_[frame_id];
  }
  int i;
  for (i = 0; i < pool_size_; i++) {//找到空闲的buffer
    if (pages_[i].GetPinCount() == 0) {
      break;
    }
  }
  if (i>=pool_size_) {
    return nullptr;
  }
  //find the place to set the page from free-list
  if (!free_list_.empty()) {
    frame_id = free_list_.back();
    free_list_.pop_back();
  }
  //find the place to set the page from replacer
  else if (replacer_->Size()){
     replacer_->Victim(&frame_id);
    if (pages_[frame_id].IsDirty()) {  // if the page is dirty, flush it to disk(have been changed)
      FlushPage(pages_[frame_id].GetPageId());
    }
  }

    pages_[frame_id].ResetMemory();//清零
    page_table_.erase(pages_[frame_id].GetPageId());//去掉关联

  std::pair<page_id_t, frame_id_t> newpair(page_id,frame_id);
  page_table_.insert(newpair);//新建关联
  pages_[frame_id].page_id_=page_id;
  pages_[frame_id].pin_count_=1;//有一个pin
  pages_[frame_id].is_dirty_ = false;
  replacer_->Pin(frame_id);
  disk_manager_->ReadPage(page_id, pages_[frame_id].GetData());//写入内容
  return &pages_[frame_id];
}

/**
 * TODO: Student Implement
 */
Page *BufferPoolManager::NewPage(page_id_t &page_id) {
  // 0.   Make sure you call AllocatePage!
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  // 4.   Set the page ID output parameter. Return a pointer to P.

 //test if all be pinned
  int i;
  for (i = 0; i < pool_size_; i++) {
    if (pages_[i].GetPinCount() == 0) {
      break;
    }
  }
  if (i>=pool_size_) {
    return nullptr;
  }

  //page_id=AllocatePage();
  frame_id_t frame_id;
  page_id_t free_page_id;
  //find the place to set the page from free-list
  if (!free_list_.empty()) {
    frame_id = free_list_.back();
    free_list_.pop_back();
  }
  //find the place to set the page from replacer
  else if (replacer_->Size()){
     replacer_->Victim(&frame_id);
    if (pages_[frame_id].IsDirty()) {//if the page is dirty, flush it to disk(have been changed)
      FlushPage(pages_[frame_id].GetPageId());
    }
    pages_[frame_id].ResetMemory();//清零
    page_table_.erase(pages_[frame_id].GetPageId());//去掉关联
  }
  //申请一个新的page_id
 page_id = AllocatePage();
  std::pair<page_id_t, frame_id_t> newpair(page_id,frame_id);
  page_table_.insert(newpair);//新建关联
  pages_[frame_id].page_id_=page_id;
  pages_[frame_id].pin_count_=1;//有一个pin
  pages_[frame_id].is_dirty_=false;
  replacer_->Pin(frame_id);
  return &pages_[frame_id];
}

/**
 * TODO: Student Implement
 */
bool BufferPoolManager::DeletePage(page_id_t page_id) {
  // 0.   Make sure you call DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  frame_id_t frame_id;
  if (page_table_.find(page_id)==page_table_.end()) {//不存在
    DeallocatePage(page_id);//在磁盘中删除
    return true;
  }
  //can't delete
    frame_id= (page_table_.find(page_id))->second;
    if (pages_[frame_id].GetPinCount() > 0)
      return false;
    pages_[frame_id].ResetMemory();
    pages_[frame_id].page_id_ = INVALID_PAGE_ID;
    pages_[frame_id].is_dirty_ = false;
  page_table_.erase(page_id);//断开连接
  replacer_->Pin(frame_id);//从lru中删除
  free_list_.push_back(frame_id);//放入freelist
  DeallocatePage(page_id);//在磁盘中删除
  return false;
}

/**
 * TODO: Student Implement
 */
bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
  frame_id_t frame_id;
  if (page_table_.find(page_id)==page_table_.end()) {//不存在
    return false;
  }
  else {//without pin, false
    frame_id= (page_table_.find(page_id))->second;
    if (pages_[frame_id].GetPinCount() <= 0)
      return false;
  }
  pages_[frame_id].pin_count_--;
  if(pages_[frame_id].pin_count_==0){//put into lru
    replacer_->Unpin(frame_id);
  }
  if(pages_[frame_id].is_dirty_ || is_dirty )
    pages_[frame_id].is_dirty_ = true;
  else
    pages_[frame_id].is_dirty_ = false;
  return true;
}

/**
 * TODO: Student Implement
 */
bool BufferPoolManager::FlushPage(page_id_t page_id) {
  if(page_id==INVALID_PAGE_ID) return false;
  frame_id_t frame_id;
  if (page_table_.find(page_id)==page_table_.end()) {//不存在
    return false;
  }
  else {
    frame_id= (page_table_.find(page_id))->second;
    if(pages_[frame_id].is_dirty_)
    {
      disk_manager_->WritePage(page_id , pages_[frame_id].data_);
      //将dirty标识重置
      pages_[frame_id].is_dirty_ = false;
    }
  }
  return true;
}

page_id_t BufferPoolManager::AllocatePage() {
  int next_page_id = disk_manager_->AllocatePage();
  return next_page_id;
}

void BufferPoolManager::DeallocatePage(__attribute__((unused)) page_id_t page_id) {
  disk_manager_->DeAllocatePage(page_id);
}

bool BufferPoolManager::IsPageFree(page_id_t page_id) {
  return disk_manager_->IsPageFree(page_id);
}

// Only used for debug
bool BufferPoolManager::CheckAllUnpinned() {
  bool res = true;
  for (size_t i = 0; i < pool_size_; i++) {
    if (pages_[i].pin_count_ != 0) {
      res = false;
      LOG(ERROR) << "page " << pages_[i].page_id_ << " pin count:" << pages_[i].pin_count_ << endl;
    }
  }
  return res;
}
