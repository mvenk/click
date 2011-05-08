#ifndef CLICK_SIMPLE_VECTOR_CC
#define CLICK_SIMPLE_VECTOR_CC
#include <click/glue.hh>
#include <click/simplevector.hh>
#include <stdio.h>
#include <unistd.h>
CLICK_DECLS

template <class T> 
SimpleVector<T>::~SimpleVector() {
  for (uint32_t i = 0; i < _n.value(); i++)
    _l[i].~T();
  CLICK_LFREE(_l, sizeof(T) * _capacity);
}

// Returns the index of the location where the item was pushed to
template <class T> inline int
SimpleVector<T>::push_back(const T& x)
{
  uint32_t old = _n.value();
  if (old < (uint32_t) _capacity || reserve(RESERVE_GROW)) {     
    // compare and swap _n to its new value
    while(_n.compare_swap(old, old+1) != old) {
      old = _n.value();
      // we double check the new "_n" value, because another 
      // thread could have succeded in the CAS and caused "_n" to
      // go above capacity
      if(old >= (uint32_t) _capacity) 
	reserve(RESERVE_GROW);
    }
    _l[old] = x;
    return old;
  }
  return -ENOENT;
}

template <class T> inline bool
SimpleVector<T>::reserve(int want) 
{
  int tid = syscall(224);
  if(want < 0) 
    want = (_capacity > 0 ? _capacity * 2 : 4);

  while(true) {
#if __x86_64__
    uint64_t old_l = reinterpret_cast<uint64_t>(_l);
#else
    uint32_t old_l = reinterpret_cast<uint32_t>(_l);
#endif

    if(want <= _capacity) 
      return true;

    printf("%d: Allocating %d\n", tid, want);
    T * new_l = (T *) CLICK_LALLOC(sizeof(T) * want);
    if(!new_l) 
      return false;

    // compare and swap _l with _newl and iff it succeeds free _l, else check again.
    // TODO: dirty as we have to deal with 64 and 32 bit pointers
#if __x86_64__ 
    if(compare_and_swap(reinterpret_cast<uint64_t &>(_l), 
			old_l, 
			reinterpret_cast<uint64_t &>(new_l)) == old_l) {
#else
      if(compare_and_swap(reinterpret_cast<uint32_t &>(_l),
			  old_l,
			  reinterpret_cast<uint32_t &>(new_l)) == old_l) {
#endif
	printf("%d: Succeeded!\n", tid);
	if(old_l)
	  memcpy(new_l, _l, sizeof(T) * _n.value());
	if(old_l)
	  CLICK_LFREE(reinterpret_cast<T *>(old_l), sizeof(T) * _capacity);
	_capacity = want;
	return true;
      }
      else {
	printf("%d: Freeing %d\n", tid, want);
	CLICK_LFREE((T*) new_l, sizeof(T) * want);
      }     
  }
}

template <class T> inline uint32_t
SimpleVector<T>::compare_and_swap(volatile uint32_t &x, uint32_t expected, uint32_t desired) {
    asm volatile (CLICK_ATOMIC_LOCK "cmpxchgl %2,%1"
		  : "=a" (expected), "=m" (x)
		  : "r" (desired), "0" (expected), "m" (x)
		  : "cc", "memory");
    return expected;
}

template <class T> inline uint64_t
SimpleVector<T>::compare_and_swap(volatile uint64_t &x, uint64_t expected, uint64_t desired) {
    asm volatile (CLICK_ATOMIC_LOCK "cmpxchgq %2,%1"
		  : "=a" (expected), "=m" (x)
		  : "r" (desired), "0" (expected), "m" (x)
		  : "cc", "memory");
    return expected;
}

CLICK_ENDDECLS
#endif
