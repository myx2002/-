#include "page/b_plus_tree_leaf_page.h"

#include <algorithm>

#include "index/generic_key.h"

#define pairs_off (data_ + LEAF_PAGE_HEADER_SIZE)
#define pair_size (GetKeySize() + sizeof(RowId))
#define key_off 0
#define val_off GetKeySize()
/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/

/**
 * TODO: Student Implement
 */
/**
 * Init method after creating a new leaf page
 * Including set page type, set current size to zero, set page id/parent id, set
 * next page id and set max size
 * 未初始化next_page_id
 */
void LeafPage::Init(page_id_t page_id, page_id_t parent_id, int key_size, int max_size) {
  SetPageType(IndexPageType::LEAF_PAGE);
  // 设置当前大小为 0
  SetSize(0);
  // 设置页 ID 和父节点 ID
  SetPageId(page_id);
  SetParentPageId(parent_id);
  // 初始化 next_page_id
  SetNextPageId(INVALID_PAGE_ID);
  // 设置最大大小
    SetMaxSize(max_size);
  // 设置 key 大小
    SetKeySize(key_size);


}

/**
 * Helper methods to set/get next page id
 */
page_id_t LeafPage::GetNextPageId() const {
  return next_page_id_;
}

void LeafPage::SetNextPageId(page_id_t next_page_id) {
  next_page_id_ = next_page_id;
  if (next_page_id == 0) {
    LOG(INFO) << "Fatal error";
  }
}

/**
 * TODO: Student Implement
 */
/**
 * Helper method to find the first index i so that pairs_[i].first >= key
 * NOTE: This method is only used when generating index iterator
 * 二分查找
 */
int LeafPage::KeyIndex(const GenericKey *key, const KeyManager &KM) {
  int left = 0, right = GetSize()-1 ;  // 第一个Key是Invalid_Page_Id，所以从1开始
  while (left <= right) {
    int mid = (left + right) >> 1;
    auto cur = KeyAt(mid);
    //ASSERT(false,"fail");
    std::vector<Column *> columns = {
        new Column("int", TypeId::kTypeInt, 0, false, false),
    };
   // Row keyrow(INVALID_ROWID);
    //Schema *table_schema = new Schema(columns);
    //KeyManager KP(table_schema, 16);

    //KP.DeserializeToKey(key,keyrow,table_schema );
   // Field *row_value =keyrow.GetField(0);
   // auto x=row_value ->toString();
    if (KM.CompareKeys(key, cur) >0)
      left = mid +1;
    else
      right = mid -1;
  }
  return right+1 ;
  return 0;
}

/*
 * Helper method to find and return the key associated with input "index"(a.k.a
 * array offset)
 */
GenericKey *LeafPage::KeyAt(int index) {
  return reinterpret_cast<GenericKey *>(pairs_off + index * pair_size + key_off);
}

void LeafPage::SetKeyAt(int index, GenericKey *key) {
  memmove(pairs_off + index * pair_size + key_off, key, GetKeySize());
}

RowId LeafPage::ValueAt(int index) const {
  return *reinterpret_cast<const RowId *>(pairs_off + index * pair_size + val_off);
}

void LeafPage::SetValueAt(int index, RowId value) {
  *reinterpret_cast<RowId *>(pairs_off + index * pair_size + val_off) = value;
}

void *LeafPage::PairPtrAt(int index) {
  return KeyAt(index);
}

void LeafPage::PairCopy(void *dest, void *src, int pair_num) {
  memmove(dest, src, pair_num * (GetKeySize() + sizeof(RowId)));
}

/*
 * Helper method to find and return the key & value pair associated with input
 * "index"(a.k.a. array offset)
 */
std::pair<GenericKey *, RowId> LeafPage::GetItem(int index) {
    // replace with your own code
  return std::make_pair(KeyAt(index), ValueAt(index));
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/
/*
 * Insert key & value pair into leaf page ordered by key
 * @return page size after insertion
 */
int LeafPage::Insert(GenericKey *key, const RowId &value, const KeyManager &KM) {
  // 查找插入位置
  int idx = KeyIndex(key, KM);

  // 将该位置之后的元素全部向后移动一位
  for (int i = GetSize() - 1; i >= idx; --i) {
    memmove(KeyAt(i + 1), KeyAt(i), GetKeySize());
    SetValueAt(i + 1, ValueAt(i));
  }
  // 插入 key 和 value
  SetKeyAt(idx, key);
  SetValueAt(idx, value);
  // 更新 current size
  IncreaseSize(1);
  return GetSize();
}

/*****************************************************************************
 * SPLIT
 *****************************************************************************/
/*
 * Remove half of key & value pairs from this page to "recipient" page
 */
void LeafPage::MoveHalfTo(LeafPage *recipient) {
  int mid = GetSize() / 2;
  int count = GetSize() - mid;
  recipient->CopyNFrom(PairPtrAt(mid), count);
 // memset(PairPtrAt(mid),0,count* pair_size);
  SetSize(mid);
  //recipient->IncreaseSize(count);

  recipient->SetNextPageId(GetNextPageId());
}

/*
 * Copy starting from items, and copy {size} number of elements into me.
 */
void LeafPage::CopyNFrom(void *src, int size) {
  memmove(pairs_off + GetSize() * pair_size, src, size * pair_size);
  SetSize(GetSize() + size);
}

/*****************************************************************************
 * LOOKUP
 *****************************************************************************/
/*
 * For the given key, check to see whether it exists in the leaf page. If it
 * does, then store its corresponding value in input "value" and return true.
 * If the key does not exist, then return false
 */
bool LeafPage::Lookup(const GenericKey *key, RowId &value, const KeyManager &KM) {
  int idx = KeyIndex(key, KM);
  if (idx < GetSize() && KM.CompareKeys(key, KeyAt(idx)) == 0) {
    value = ValueAt(idx);
    return true;
  }
  return false;
}

int  LeafPage ::SearchKey(const GenericKey *key,const KeyManager &KM) {
  int low = 0;
  int high = GetSize() - 1;

  while (low < high) {
    int mid = (low + high) / 2;
    int cmp = KM.CompareKeys(key, KeyAt(mid));
    if (cmp <=0) {
      high = mid;
    } else {
      low = mid +1;
    }
  }

  if( KM.CompareKeys(key, KeyAt(low))==0)
    return low;
  return -1;
}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
/*
 * First look through leaf page to see whether delete key exist or not. If
 * existed, perform deletion, otherwise return immediately.
 * NOTE: store key&value pair continuously after deletion
 * @return  page size after deletion
 */
int LeafPage::RemoveAndDeleteRecord(const GenericKey *key, const KeyManager &KM) {
 /* for (int i = 0; i < GetSize(); ++i) {
    if (KM.CompareKeys(KeyAt(i), key) == 0) {
      int pos = i;
      cout<<pos<<"here"<<endl;
      break;
    }}*/
  int idx = SearchKey(key, KM);
 /* cout<<idx<<endl;*/
  if (idx <0) {
    // 如果未找到，返回-1


    return -1;
  }
  // 将该位置之后的元素全部向前移动一位
 /* for (int i = idx + 1; i < GetSize(); ++i) {
    memmove(KeyAt(i - 1), KeyAt(i), GetKeySize());
    SetValueAt(i - 1, ValueAt(i));
  }*/
  memmove(KeyAt(idx), KeyAt(idx + 1), (GetSize() - idx - 1) * pair_size);
  //memset(KeyAt(GetSize()-1),0,pair_size);
  // 更新 current size
  IncreaseSize(-1);
  return 1;
}

/*****************************************************************************
 * MERGE
 *****************************************************************************/
/*
 * Remove all key & value pairs from this page to "recipient" page. Don't forget
 * to update the next_page id in the sibling page
 */
void LeafPage::MoveAllTo(LeafPage *recipient) {

  recipient->CopyNFrom(pairs_off, GetSize());
  //memset(pairs_off,0,GetSize()*pair_size);
  recipient->SetNextPageId(GetNextPageId());
  SetSize(0);
}

/*****************************************************************************
 * REDISTRIBUTE
 *****************************************************************************/
/*
 * Remove the first key & value pair from this page to "recipient" page.
 *
 */
void LeafPage::MoveFirstToEndOf(LeafPage *recipient) {
  recipient->CopyLastFrom(KeyAt(0), ValueAt(0));
  int newsize = GetSize() - 1;
  for (int i = 0; i < newsize; ++i) {
    memmove(KeyAt(i), KeyAt(i + 1), GetKeySize());
    SetValueAt(i, ValueAt(i + 1));
  }
  memset(KeyAt(GetSize() - 1),0,pair_size);
  SetSize(newsize);
}

/*
 * Copy the item into the end of my item list. (Append item to my array)
 */
void LeafPage::CopyLastFrom(GenericKey *key, const RowId value) {
  SetKeyAt(GetSize(), key);
  SetValueAt(GetSize(), value);
  IncreaseSize(1);
}

/*
 * Remove the last key & value pair from this page to "recipient" page.
 */
void LeafPage::MoveLastToFrontOf(LeafPage *recipient) {
  auto last_key = KeyAt(GetSize() - 1);
  auto last_val = ValueAt(GetSize() - 1);
  recipient->CopyFirstFrom(last_key, last_val);
  memset(KeyAt(GetSize() - 1),0,pair_size);
  SetSize(GetSize() - 1);
}

/*
 * Insert item at the front of my items. Move items accordingly.
 *
 */
void LeafPage::CopyFirstFrom(GenericKey *key, const RowId value) {
  int count = GetSize();
  for (int i = count - 1; i >= 0; --i) {
    memmove(KeyAt(i + 1), KeyAt(i), GetKeySize());
    SetValueAt(i + 1, ValueAt(i));
  }
  memmove(KeyAt(0), key, GetKeySize());
  SetValueAt(0, value);
  SetSize(count + 1);

}
