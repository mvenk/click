#ifndef BUCKET_ARRAY_CC
#define BUCKET_ARRAY_CC
#include <click/glue.hh>
#include <click/bucketarray.hh>
#include <click/atomic.hh>

CLICK_DECLS

template <class T>
BucketArray<T>::~BucketArray() {
  click_chatter("In bucket array destructor");
  for(int i=0; i < (int)_npointers; i++) {
    for(int j=0; j < ARRAY_SIZE; j++) {
      _l[i][j].~T();
    }
    CLICK_LFREE(_l[i], sizeof(T) * ARRAY_SIZE);
  }
  CLICK_LFREE(_l, sizeof(T*) * _npointers);
}

template <class T> inline int
BucketArray<T>::push_back(const T& x) 
{
  uint32_t oldn = _nelems;
  if(oldn < capacity() || reserve()) {
    while(true) {      
      if(atomic_uint32_t::compare_swap(_nelems, oldn, oldn+1) != oldn) {
	oldn = _nelems;
	if(oldn >= capacity())
	  reserve();
      }
      else {
	break;
      }
    }
    int pidx = oldn/ARRAY_SIZE;
    int idx  = oldn % ARRAY_SIZE;
    _l[pidx][idx] = x;
    return oldn;
  }
  return -1;
}

template <class T> inline bool
BucketArray<T>::reserve() {
  _lock.acquire();
  if(_nelems >= capacity()) {
    T ** new_l = (T **) CLICK_LALLOC(sizeof(T*) * (_npointers + 1));
    if(!new_l) 
      return false;
    
    new_l[_npointers] = (T *) CLICK_LALLOC(sizeof(T) * ARRAY_SIZE);
    
    if(!new_l[_npointers]) 
      return false;
    
    memcpy(new_l, _l, sizeof(T**) * _npointers);
    // see if seg-fault goes away if we do not free.
    // CLICK_LFREE((T**) _l, sizeof(T**) * _npointers);
    _l = new_l;

    // Using CAS to increment _npointers, as it also acts as a memory
    // barrier. A CAS is strictly not required here, because we have already
    // acquired a lock.
    atomic_uint32_t::compare_swap(_npointers, _npointers, _npointers + 1);
    
  }
  _lock.release();
  return true;
}

CLICK_ENDDECLS
#endif
