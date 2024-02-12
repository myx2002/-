#include "buffer/set_replacer.h"
//新增库函数引用
//无法正常引用"macros.h"文件
#include<assert.h>

SetReplacer::SetReplacer(size_t num_pages):Set_number(num_pages){}

SetReplacer::~SetReplacer() = default;

/**
 * TODO: Student Implement
 */
bool SetReplacer::Victim(frame_id_t *frame_id) {
  if(victim_.empty())
  {
    return false;
  }
  else
  {
    //随机取出一个frame_id
    while(victim_.size() > 0)
    {
      frame_id_t temp = victim_.back();
      victim_.pop_back();
      //判断是否在pin中
      if(unpin_.find(temp) != unpin_.end())
      {
        *frame_id = temp;
        unpin_.erase(temp);
        return true;
      }
    }
    return false;
  }
}

/**
 * TODO: Student Implement
 */
void SetReplacer::Pin(frame_id_t frame_id) {
  unpin_.erase(frame_id);
}

/**
 * TODO: Student Implement
 */
void SetReplacer::Unpin(frame_id_t frame_id) {
  //判断是否已经在pin中
  if(unpin_.find(frame_id) == unpin_.end())
  {
    if(unpin_.size()>=Set_number)
    {
      //随机删除一个
      unpin_.erase(unpin_.begin());
    }
    unpin_.insert(frame_id);
    victim_.push_back(frame_id);
  }
}

/**
 * TODO: Student Implement
 */
size_t SetReplacer::Size() {
  //返回list中元素个数
  return unpin_.size();
}