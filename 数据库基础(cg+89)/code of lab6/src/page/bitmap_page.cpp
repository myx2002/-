#include "page/bitmap_page.h"

#include "glog/logging.h"

/**
 * TODO: Student Implement
 */
// MAX_CHARS: 位图页中字节的数量
template <size_t PageSize>
bool BitmapPage<PageSize>::AllocatePage(uint32_t &page_offset) {
      for (uint32_t i = 0; i < MAX_CHARS; i++) {
        if(bytes[i] != 0xff) {//如果不是全1
          for (uint8_t j = 0; j < 8; j++) {
            //若某一位为0，表示该位未被使用
            if((bytes[i] & (128 >> j)) == 0) {//检验每个bit(按位与)，j前的bit都是1，j个是0
              bytes[i] += (128>>j);//set it to be 1
              page_offset = i * 8 + j;
              return true;
            }
          }
        }
  }
  return false;//没有空的
}

/**
 * TODO: Student Implement
 */
template <size_t PageSize>
bool BitmapPage<PageSize>::DeAllocatePage(uint32_t page_offset) {
  int i=page_offset/8; int j=page_offset%8;
  if((bytes[i]&(128>>j))==0)//本来就是空闲的
    return false;
  bytes[i]-=(128>>j);
  return true;
}

/**
 * TODO: Student Implement
 */
template <size_t PageSize>
bool BitmapPage<PageSize>::IsPageFree(uint32_t page_offset) const {
  uint32_t i=page_offset/8; uint8_t j=page_offset%8;
  if((bytes[i]&(128>>j))==0)//本来就是空闲的
    return true;
  return false;
}

template <size_t PageSize>
bool BitmapPage<PageSize>::IsPageFreeLow(uint32_t byte_index, uint8_t bit_index) const {
  return !(bytes[byte_index] & (128 >> bit_index));
  //return false;
}

template class BitmapPage<64>;

template class BitmapPage<128>;

template class BitmapPage<256>;

template class BitmapPage<512>;

template class BitmapPage<1024>;

template class BitmapPage<2048>;

template class BitmapPage<4096>;
