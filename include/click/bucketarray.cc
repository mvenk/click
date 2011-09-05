#ifndef BUCKET_ARRAY_CC
#define BUCKET_ARRAY_CC
#include <click/glue.hh>
#include <click/bucketarray.hh>
#include <click/atomic.hh>

CLICK_DECLS

template <class T>
BucketArray<T>::~BucketArray() {
  // click_chatter("In bucket array destructor");
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
  f_mutex.lock();
  if(_nelems >= capacity()) {
    T ** new_l = (T **) new unsigned char[sizeof(T*) * (_npointers + 1)];
    if(!new_l) {
      f_mutex.lock();
      return false;
    }
    
    new_l[_npointers] = (T *) CLICK_LALLOC(sizeof(T) * ARRAY_SIZE);
    
    if(!new_l[_npointers]) {
      f_mutex.unlock();
      return false;
    }
    
    memcpy(new_l, _l, sizeof(T**) * _npointers);
    _reclaim_later.push_back((void*)_l);
    //_reclaimhook.schedule();

    _l = new_l;
    _npointers++;
    
  }
  f_mutex.unlock();
  return true;
}

template <class T> inline void
BucketArray<T>::reclaim_l()
{
 
  f_mutex.lock();

    while(!_reclaim_now.empty()) {
      void * l = _reclaim_now.front();
      delete[] (unsigned char *)l;
      _reclaim_now.pop_front();
    }        
    Vector<void*> temp = _reclaim_now;
    _reclaim_now = _reclaim_later;
    _reclaim_later = temp;

    // If there is nothing to free in the next quiescent state
    // we unschedule ourselves.
    //if(_reclaim_now.empty()) {
    //_reclaimhook.unschedule();
    //}

    f_mutex.unlock();
 
}

CLICK_ENDDECLS
#endif
