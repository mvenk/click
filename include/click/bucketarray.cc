#ifndef BUCKET_ARRAY_CC
#define BUCKET_ARRAY_CC
#include <click/glue.hh>
#include <click/bucketarray.hh>

CLICK_DECLS

template <class T>
BucketArray<T>::~BucketArray() {
  for(int i=0; i < _npointers; i++) {
    for(int j=0; j < ARRAY_SIZE; j++) {
      _l[i][j].~T();
    }
    CLICK_LFREE(_l[i], sizeof(T) * ARRAY_SIZE);
  }
  CLICK_LFREE(_l, sizeof(T*) * _npointers);
}

template <class T> inline void
BucketArray<T>::push_back(const T& x) 
{
  if(_nelems < capacity() || reserve()) {
    int pidx = _nelems/ARRAY_SIZE;
    int idx  = _nelems % ARRAY_SIZE;
    _l[pidx][idx] = x;
    _nelems++;
  }
}

template <class T> inline bool
BucketArray<T>::reserve() {
  T ** new_l = (T **) CLICK_LALLOC(sizeof(T*) * (_npointers + 1));
  if(!new_l) 
    return false;

  new_l[_npointers] = (T *) CLICK_LALLOC(sizeof(T) * ARRAY_SIZE);

  if(!new_l[_npointers]) 
    return false;

  memcpy(new_l, _l, sizeof(T**) * _npointers);
  CLICK_LFREE((T**) _l, sizeof(T**) * _npointers);
  _l = new_l;
  _npointers++;
  return true;
}

CLICK_ENDDECLS
#endif
