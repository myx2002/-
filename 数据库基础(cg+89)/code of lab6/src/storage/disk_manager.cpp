#include "storage/disk_manager.h"

#include <sys/stat.h>
#include <filesystem>
#include <stdexcept>

#include "glog/logging.h"
#include "page/bitmap_page.h"

DiskManager::DiskManager(const std::string &db_file) : file_name_(db_file) {
  std::scoped_lock<std::recursive_mutex> lock(db_io_latch_);
  db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
  // directory or file does not exist
  if (!db_io_.is_open()) {
    db_io_.clear();
    // create a new file
    std::filesystem::path p = db_file;
    if(p.has_parent_path()) std::filesystem::create_directories(p.parent_path());
    db_io_.open(db_file, std::ios::binary | std::ios::trunc | std::ios::out);
    db_io_.close();
    // reopen with original mode
    db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io_.is_open()) {
      throw std::exception();
    }
  }
  ReadPhysicalPage(META_PAGE_ID, meta_data_);
}

void DiskManager::Close() {
  std::scoped_lock<std::recursive_mutex> lock(db_io_latch_);
  if (!closed) {
    db_io_.close();
    closed = true;
  }
}

void DiskManager::ReadPage(page_id_t logical_page_id, char *page_data) {
  ASSERT(logical_page_id >= 0, "Invalid page id.");
  ReadPhysicalPage(MapPageId(logical_page_id), page_data);
}

void DiskManager::WritePage(page_id_t logical_page_id, const char *page_data) {
  ASSERT(logical_page_id >= 0, "Invalid page id.");
  WritePhysicalPage(MapPageId(logical_page_id), page_data);
}

/**
 * TODO: Student Implement
 */
page_id_t DiskManager::AllocatePage() {
 // DiskFileMetaPage* meta=reinterpret_cast< DiskFileMetaPage*>(GetMetaData());//转成diskFileMetaPage
//  if(meta->GetAllocatedPages()>MAX_VALID_PAGE_ID) {LOG(ERROR)<<"exceed maximun allocate quantity";return INVALID_PAGE_ID; }
 // int num_extent=meta->GetExtentNums();
 // int i;
 // for(i=0;i<num_extent;i++){//detect if has available position
 //   if(meta->GetExtentUsedPage(i) <BitmapPage<PAGE_SIZE>::GetMaxSupportedSize()){
    //    meta->extent_used_page_[i]+=1;//increase the page in this extent
      //  break;
 // }
 // if(i>=num_extent){//former extents has no place
  //  meta->num_extents_+=1;
  //  meta->extent_used_page_[num_extent]+=1;
 // }
 // meta->num_allocated_pages_+=1;
  //return meta->GetAllocatedPages()-1;
  //分区从0开始记录,寻找第一空闲页物理编号
  //DiskFileMetaPage只是储存其它页的信息，只是类似索引，要先修改其它页再修改它，只修改它只是修改它并没有实际作用，页面还是没有分配，数据还是没写回磁盘
  page_id_t next_free_page_id;
  //分区序号
  uint32_t first_free_bitmap = 0;
  DiskFileMetaPage *meta_page =reinterpret_cast<DiskFileMetaPage *>(meta_data_);
  while(meta_page->extent_used_page_[first_free_bitmap] == BITMAP_SIZE)
  {
    first_free_bitmap++;
    ASSERT(first_free_bitmap < MAX_VALID_PAGE_ID,"exceed maxinum allocate quantity");
  }
  //找到目标bitmap区域后
  page_id_t bitmap_physical_page_id = first_free_bitmap * (1 + BITMAP_SIZE) + 1;
  char page_data[PAGE_SIZE];
  //读取数据并转换类型
  ReadPhysicalPage(bitmap_physical_page_id,page_data);
  BitmapPage<PAGE_SIZE> *bitmap_page = reinterpret_cast<BitmapPage<PAGE_SIZE> *>(page_data);
  //offset
  uint32_t next_fre_page;
  bitmap_page->AllocatePage(next_fre_page);

  //计算逻辑页数(均从0开始记录，不用offset)
  next_free_page_id = first_free_bitmap  * BITMAP_SIZE + next_fre_page;

  //修改记录参数
  //修改已分配页数
  meta_page->num_allocated_pages_++;
  //必要时修改分区数量(分区0开始记录，数量1开始记录，所以需要等号)
  if(first_free_bitmap >= meta_page->num_extents_)
    meta_page->num_extents_++;
  //修改对映分区分配数
  meta_page->extent_used_page_[first_free_bitmap]++;

  //更新disk与meta_data_数值
  WritePhysicalPage(META_PAGE_ID,meta_data_);
  WritePhysicalPage(bitmap_physical_page_id,page_data);

  return next_free_page_id;
}

/**
 * TODO: Student Implement
 */
void DiskManager::DeAllocatePage(page_id_t logical_page_id) {
  //DiskFileMetaPage* meta=reinterpret_cast< DiskFileMetaPage*>(GetMetaData());//转成diskFileMetaPage
 // if(this->IsPageFree(logical_page_id)) {LOG(ERROR)<<"the page is free thus can't be deallocated";return; }
 // meta->num_allocated_pages_-=1;
  //int num_extent=meta->GetExtentNums();
  //int i;int cnt=logical_page_id;
 // for(i=0;i<num_extent;i++){//detect the page is in which extent
   // if(meta->GetExtentUsedPage(i) >=cnt){//in this extent
       // meta->extent_used_page_[i]-=1;//decrease the page in this extent
       // break;
   // }
    //cnt-=meta->GetExtentUsedPage(i);
 // }
 // if(meta->GetExtentUsedPage(i)==0){//if this extent is empty
    //for(int j=i;j<num_extent-1;j++){//move the extents
     // meta->extent_used_page_[j]=meta->extent_used_page_[j+1];
   // }
   // meta->num_extents_-=1;
  //}
  //通过逻辑页id计算对映bitmap页物理id
  page_id_t physical_page_id = MapPageId(logical_page_id);
  page_id_t bitmap_physical_page_id = 1 + (physical_page_id - 1) / (1 + BITMAP_SIZE) * (1 + BITMAP_SIZE);
  //从disk读出数据并进行相应转化
  char page_data[PAGE_SIZE];
  ReadPhysicalPage(bitmap_physical_page_id,page_data);
  BitmapPage<PAGE_SIZE> *bitmap_page = reinterpret_cast<BitmapPage<PAGE_SIZE> *>(page_data);
  //通过bitmap类型free page
  bitmap_page->DeAllocatePage(physical_page_id - bitmap_physical_page_id - 1);
  //修改元数据信息
  DiskFileMetaPage *meta_page =reinterpret_cast<DiskFileMetaPage *>(meta_data_);
  //减去相应的分配总页数
  meta_page->num_allocated_pages_--;
  //减少相应分区的分配总页数
  uint32_t modify_page = (physical_page_id - 1) / (1 + BITMAP_SIZE);
  meta_page->extent_used_page_[modify_page]--;
  //根据当前分区的实际分配量修改分配分区数量
  if(meta_page->extent_used_page_[modify_page] == 0)
    meta_page->num_extents_--;

  //写回disk
  WritePhysicalPage(META_PAGE_ID,meta_data_);
  WritePhysicalPage(bitmap_physical_page_id,page_data);
}

/**
 * TODO: Student Implement
 */
bool DiskManager::IsPageFree(page_id_t logical_page_id) {
  //DiskFileMetaPage* meta=reinterpret_cast< DiskFileMetaPage*>(GetMetaData());//转成diskFileMetaPage
 // if(meta->GetAllocatedPages()<logical_page_id)
   // return true;
  //return false;

  page_id_t physical_page_id = logical_page_id + logical_page_id/(BITMAP_SIZE) + 2;//通过逻辑页号计算物理页号，语雀可以看对应关系
  //通过bitmap去判断page是否free
  //通过数据页计算相应的bitmap页
  page_id_t bitmap_physical_page_id = 1 + (physical_page_id - 1)/(1 + BITMAP_SIZE) * (1 + BITMAP_SIZE);
  //从disk读取数据
  char page_data[PAGE_SIZE];
  ReadPhysicalPage(bitmap_physical_page_id,page_data);
  //类型转换
  BitmapPage<PAGE_SIZE> *bitmap_page = reinterpret_cast<BitmapPage<PAGE_SIZE> *>(page_data);
  //注意bitmap_page下标从0开始
  bool result;
  //result = bitmap_page->IsPageFree(0);
  result = bitmap_page->IsPageFree(physical_page_id - bitmap_physical_page_id - 1);
  if (result)
    return true;
  else
    return false;
}

/**
 * TODO: Student Implement
 */
page_id_t DiskManager::MapPageId(page_id_t logical_page_id) {//它框架这个函数是转物理页ID，不是转位图页ID
  //计算式应该为logical_page_id/physical_page_id*(1+physical_page_id)+1，可以去语雀那里看物理页和逻辑页的对应关系
  //physical_page_id从1开始,logical_page_id从0开始
  page_id_t result =  logical_page_id + logical_page_id/BITMAP_SIZE + 2;
  return result;
}

int DiskManager::GetFileSize(const std::string &file_name) {
  struct stat stat_buf;
  int rc = stat(file_name.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}

void DiskManager::ReadPhysicalPage(page_id_t physical_page_id, char *page_data) {
  int offset = physical_page_id * PAGE_SIZE;
  // check if read beyond file length
  if (offset >= GetFileSize(file_name_)) {
#ifdef ENABLE_BPM_DEBUG
    LOG(INFO) << "Read less than a page" << std::endl;
#endif
    memset(page_data, 0, PAGE_SIZE);
  } else {
    // set read cursor to offset
    db_io_.seekp(offset);
    db_io_.read(page_data, PAGE_SIZE);
    // if file ends before reading PAGE_SIZE
    int read_count = db_io_.gcount();
    if (read_count < PAGE_SIZE) {
#ifdef ENABLE_BPM_DEBUG
      LOG(INFO) << "Read less than a page" << std::endl;
#endif
      memset(page_data + read_count, 0, PAGE_SIZE - read_count);
    }
  }
}

void DiskManager::WritePhysicalPage(page_id_t physical_page_id, const char *page_data) {
  size_t offset = static_cast<size_t>(physical_page_id) * PAGE_SIZE;
  // set write cursor to offset
  db_io_.seekp(offset);
  db_io_.write(page_data, PAGE_SIZE);
  // check for I/O error
  if (db_io_.bad()) {
    LOG(ERROR) << "I/O error while writing";
    return;
  }
  // needs to flush to keep disk file in sync
  db_io_.flush();
}
