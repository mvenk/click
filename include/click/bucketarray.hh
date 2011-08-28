#ifndef BUCKET_ARRAY_HH
#define BUCKET_ARRAY_HH
#include <click/sync.hh>
#include <click/reclaimable.hh>
#include <click/reclaimhook.hh>
CLICK_DECLS

template <class T>
class BucketArray{
 
public:

  BucketArray():
    _bucket_start(0), _nelems(0), _npointers(0), _narrays(0) {
    
  }


  ~BucketArray();
  enum { RESERVE_GROW = (int) - 1};
  static const uint32_t ARRAY_SIZE = 1024;
  static const uint32_t BUCKET_SIZE = 128;
  static const uint32_t BUCKET_CAPACITY = ARRAY_SIZE * BUCKET_SIZE;
  T &operator[] (int i) {
    assert(i >= 0 && i < (int)_nelems);
    struct bucket* required_bucket = _bucket_start;
    // find out which bucket-array it belongs to.
    int bucket_id = i/ BUCKET_CAPACITY; 
    for(int k =0; k < bucket_id; k++)
      {
	// follow pointers to reach the correct bucket
	required_bucket = required_bucket->next;
	i = i- BUCKET_CAPACITY;
      }
    T** l = required_bucket->_l;
    int bidx = i/ARRAY_SIZE;
    int idx = i % ARRAY_SIZE;
    return l[bidx][idx];
  }

  const T &operator[] (int i) const {
    assert(i >= 0 && i < (int)_nelems);
    struct bucket* required_bucket = _bucket_start;
    // find out which bucket-array it belongs to.
    int bucket_id = i/ BUCKET_CAPACITY; 
    for(int k =0; k < bucket_id; k++)
      {
	// follow pointers to reach the correct bucket
	required_bucket = required_bucket->next;
	i = i- BUCKET_CAPACITY;
      }
    T** l = required_bucket->_l;
    int bidx = i/ARRAY_SIZE;
    int idx = i % ARRAY_SIZE;
    return l[bidx][idx];
  }
  
  void clear() {}
  inline int push_back(const T&x);
  uint32_t capacity() const {return _narrays * ARRAY_SIZE;}
  int size() const {return _nelems;}
  void reclaim_l();
  
  // 
  struct bucket{
    T ** _l;
    struct bucket* next;
  };
private:
  bool reserve();
  bucket* allocate_bucket(void);
  bucket* _bucket_start;
  

  // keeps track of the number of elements inserted in the bucket array
  uint32_t _nelems;
  
  // keeps track of the number of pointers which are currently in the
  // bucket array
  uint32_t _npointers;

  // keeps track of the number of bucket arrays
  uint32_t _narrays;
  Spinlock _lock;
  // RCU
  // ReclaimHook _reclaimhook;
  Vector<void*> _reclaim_now;
  Vector<void*> _reclaim_later;

};

CLICK_ENDDECLS
#include<click/bucketarray.cc>
#endif

