#ifndef SIMPLE_VECTOR_HH
#define SIMPLE_VECTOR_HH
CLICK_DECLS

// A basic implementation of a lock free vector
template <class T>
class SimpleVector { 

public:
    
  explicit SimpleVector(): 
    _l(0), _n(0), _capacity(0) {
  }

  ~SimpleVector();
  enum { RESERVE_GROW = (int) - 1 };
  
  T &operator[] (int i) {
    assert((unsigned) i < (unsigned) _n.value());
    return _l[i];
  }

  uint32_t size() const { return _n.value(); }
  int capacity() const { return _capacity; }
  bool reserve(int n);
  void clear() { }
  inline void push_back(const T&x);

private:
  T *_l;
  atomic_uint32_t _n;
  int _capacity;
};

template <class T> inline void 
SimpleVector<T>::push_back(const T& x)
{
  int old = _n.value();
  if (old < _capacity || reserve(RESERVE_GROW)) {     
    // compare and swap _n to its new value
    while(_n.compare_swap(old, old+1) != old) {
      old = _n.value();
      // we double check the new "_n" value, because another 
      // thread could have succeded in the CAS and caused "_n" to
      // go above capacity
      if(old >= capacity) 
	reserve(RESERVE_GROW);
    }
    _l[old] = x;    
  }
}

template <class T> inline bool
SimpleVector<T>::reserve(int want) 
{
  if(want < 0) 
    want = (_capacity > 0 ? _capacity * 2 : 4);

  while(true) {
    if(want <= _capacity) 
      return true;
    
    uint32_t old_l = reinterpret_cast<uint32_t>(_l);
    T * new_l = (T *) CLICK_LALLOC(sizeof(T) * want);
    if(!new_l) 
      return false;
    
    memcpy(new_l, _l, sizeof(T) * _n);
    // compare and swap _l with _newl and iff it succeeds free _l, else check again.
    
    if(atomic_uint32_t::compare_swap((uint32_t) _l, old_l, new_l) == old_l) {   
      CLICK_LFREE((T*) old_l, sizeof(T) * _capacity);
      _capacity = want;
      return true;
    } 
  }
}

CLICK_ENDDECLS
#endif
