#ifndef BUCKET_ARRAY_CC
#define BUCKET_ARRAY_CC
#include <click/glue.hh>
#include <click/bucketarray.hh>
#include <click/atomic.hh>
CLICK_DECLS

#define assert2(cond, msg) do { if(!cond) click_chatter(msg);} while(0)

template <class T>
BucketArray<T>::~BucketArray() {
  // click_chatter("In bucket array destructor");
  /*
    for(int i=0; i < (int)_npointers; i++) {
    for(int j=0; j < ARRAY_SIZE; j++) {
    _l[i][j].~T();
    }
    CLICK_LFREE(_l[i], sizeof(T) * ARRAY_SIZE);
    }
    CLICK_LFREE(_l, sizeof(T*) * _npointers); 
  */
}

template <class T> inline int
BucketArray<T>::push_back(const T& x) 
{
  _lock.acquire();
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
    struct bucket* required_bucket = _bucket_start;
    // find out which bucket-array it belongs to.
    int bucket_id = oldn/ BUCKET_CAPACITY;
    int i = oldn;
    for(int k =0; k < bucket_id; k++)
      {
	
	// follow pointers to reach the correct bucket
	required_bucket = required_bucket->next;
	i = i- BUCKET_CAPACITY;
      }
    T** l = required_bucket->_l;
    int bidx = i/ARRAY_SIZE;
    int idx = i % ARRAY_SIZE;
    click_chatter("i:%d",i);
    click_chatter("l:%d", l);
    l[bidx][idx]=x;
    _lock.release();
    return oldn;
  }
  _lock.release();
  return -1;
}

template <class T> inline struct BucketArray<T>::bucket*
BucketArray<T>:: allocate_bucket(void) {
  bucket * bucket_array = new bucket();
  memset(bucket_array,0,sizeof(bucket));
  T ** new_l = new T*[BUCKET_SIZE];
  bucket_array->_l = new_l;
  bucket_array->next = 0;
  _npointers = 0;
  return bucket_array;  
}

template <class T> inline bool
BucketArray<T>::reserve() {
  if(_nelems >= capacity()) {
    T** l;
    if(!_bucket_start){
      _bucket_start = allocate_bucket();
      if(!_bucket_start){
	return false;
      }
      l = _bucket_start->_l;
    }// if bucket_start
    else{
      struct bucket* last_bucket = _bucket_start;
      int bucket_id = (capacity()-1)/ BUCKET_CAPACITY; 
      for(int k =0; k < bucket_id; k++){
	// follow pointers to reach the correct bucket
	last_bucket = last_bucket->next;
      }
      if(_npointers == BUCKET_SIZE){
	// find the last bucket
	assert2(last_bucket != 0, "Last bucket was found to be null");
	struct bucket* new_bucket = allocate_bucket();
	last_bucket->next = new_bucket;
	last_bucket = last_bucket->next;
	if(!last_bucket){
	  return false;
	}
	
      }// if _npointers
      l = last_bucket->_l;
    }// else
    
    l[_npointers] = (T *) CLICK_LALLOC(sizeof(T) * ARRAY_SIZE);
    click_chatter("allocated an array");
    if(!l[_npointers]) {
      click_chatter("could not allocate");
      return false;
    }
    _npointers++;
    _narrays++;
    click_chatter("narrays: %d npointers: %d",_narrays,_npointers);
  }
  return true;
}

/*
  template <class T> inline void
  BucketArray<T>::reclaim_l()
  {
 
  _lock.acquire();

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

  _lock.release();
 
  }
*/


CLICK_ENDDECLS
#endif
