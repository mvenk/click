#ifndef BUCKET_ARRAY_HH
#define BUCKET_ARRAY_HH
CLICK_DECLS

template <class T>
class BucketArray {
 
public:

  BucketArray():
    _l(0), _nelems(0), _npointers(0) {
    
  }
  ~BucketArray();
  enum { RESERVE_GROW = (int) - 1};
  static const int ARRAY_SIZE = 1024;

  T &operator[] (int i) {
    assert(i >= 0 && i < _nelems);
    int bidx = i/ARRAY_SIZE;
    int idx = i % ARRAY_SIZE;
    return _l[bidx][idx];
  }

  const T &operator[] (int i) const {
    assert(i >= 0 && i < _nelems);
    int bidx = i/ARRAY_SIZE;
    int idx = i % ARRAY_SIZE;
    return _l[bidx][idx];
  }
  
  void clear() {}
  inline void push_back(const T&x);
  int capacity() const {return _npointers * ARRAY_SIZE;}
  int size() const {return _nelems;}
private:
  bool reserve();
  T **_l;

  // keeps track of the number of elements inserted in the bucket array
  int _nelems;
  
  // keeps track of the number of pointers which are currently in the
  // bucket array
  int _npointers;
};

CLICK_ENDDECLS
#include<click/bucketarray.cc>
#endif

