#ifndef SIMPLE_VECTOR_HH
#define SIMPLE_VECTOR_HH
CLICK_DECLS

// A basic implementation of a lock free vector
template <class T>
class SimpleVector { 

public:
    
  explicit SimpleVector(): 
    _l(0), _capacity(0) {
  }

  ~SimpleVector();
  enum { RESERVE_GROW = (int) - 1 };

  T &operator[] (int i) {
    assert((unsigned) i < (unsigned) _n.value());
    return _l[i];
  }
  
  const T &operator[] (int i) const {
    assert((unsigned) i < (unsigned) _n.value());
    return _l[i];
  }

  int size() const { return (int) _n.value(); }
  int capacity() const { return _capacity; }
  bool reserve(int n);
  void clear() { }
  inline int push_back(const T&x);

private:
  T *_l;
  atomic_uint32_t _n;
  uint32_t compare_and_swap(volatile uint32_t &x, uint32_t expected, uint32_t desired);
  uint64_t compare_and_swap(volatile uint64_t &x, uint64_t expected, uint64_t desired);
  int _capacity;
  
};

#include<click/simplevector.cc>
#endif
