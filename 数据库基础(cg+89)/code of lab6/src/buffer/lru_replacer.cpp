#include "buffer/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages){}

LRUReplacer::~LRUReplacer() = default;

/**
 * TODO: Student Implement
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
  if (head==nullptr) return false;//not exist
  lru_list* p1=head;lru_list* p2=nullptr;
    while(p1->next){
      p2=p1;p1=p1->next;
    }
    *frame_id=p1->f_id;
    if(p2) p2->next=nullptr;
    else head=nullptr;
    delete p1;
    size-=1;
  return true;
}

/**
 * TODO: Student Implement
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
  lru_list* p1=head;lru_list* p2=nullptr;
    while(p1){
      if(p1->f_id==frame_id)//exist
        {
          if(p2==nullptr)//p1 is head
            head=p1->next;
          else {p2->next=p1->next;
                delete p1;}
          size-=1;
          return;
        }
      p2=p1;p1=p1->next;
    }
}

/**
 * TODO: Student Implement
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
  if(head==nullptr){
    head=new lru_list;//
    head->f_id=frame_id;
    head->next=nullptr;
    size+=1;
  }
  else{
    lru_list* p1=head;
    while(p1){
      if(p1->f_id==frame_id)//already exist
        return;
      p1=p1->next;
    }
    //not exist
      lru_list* p2=new lru_list;
      p2->f_id=frame_id;
      p2->next=head;
      head=p2;
      size+=1;
  }
  
}

/**
 * TODO: Student Implement
 */
size_t LRUReplacer::Size() {
  return size;
}