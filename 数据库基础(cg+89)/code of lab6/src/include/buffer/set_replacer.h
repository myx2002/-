#ifndef MINISQL_Set_REPLACER_H
#define MINISQL_Set_REPLACER_H

#include <list>
#include <mutex>
#include <unordered_set>
#include <vector>

#include "buffer/replacer.h"
#include "common/config.h"

using namespace std;

/**
 * SetReplacer implements the Least Recently Used replacement policy.
 */
class SetReplacer : public Replacer {
 public:
  /**
   * Create a new SetReplacer.
   * @param num_pages the maximum number of pages the SetReplacer will be required to store
   */
  explicit SetReplacer(size_t num_pages);

  /**
   * Destroys the SetReplacer.
   */
  ~SetReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

private:
  // add your own private member variables here
  //使用链表记录便于随机取出插入,frame_id_t本身即int类型
  //Set_number表示记录最大个数
  size_t Set_number;
  unordered_set<frame_id_t> unpin_;
  vector<frame_id_t> victim_;
};

#endif  // MINISQL_Set_REPLACER_H
