#ifndef FUTEX_HH
#define FUTEX_HH

#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <errno.h>
CLICK_DECLS

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
  return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

class Futex_mutex
{
public:
  Futex_mutex () : val(0) { }
  void lock () {
    int c;
    while ((c = atomic_inc(&val,1) ) != 0)
      futex_wait (&val, c + 1); }
  void unlock () {
    val = 0; futex_wake (&val, 1); }
private:

  int val;

  void futex_wait(int* value, int wait_condition) {
    if(sys_futex(value, FUTEX_WAIT_PRIVATE, wait_condition, NULL, NULL, 0)== -1)
      click_chatter("error: %s",strerror(errno));
  }

  void futex_wake(int* value, int wait_condition) {
    if(sys_futex(value, FUTEX_WAKE_PRIVATE, wait_condition, NULL, NULL, 0)== -1)
      click_chatter("error: %s",strerror(errno));
  }
  
  inline int atomic_inc(volatile int* mem, int val)
  {
    int r;

    asm volatile
      (
       "lock\n\t"
       "xadd %1, %0":
       "+m"( *mem ), "=r"( r ):
       "1"( val ):
       "memory", "cc"
       );

    return r;
  }

};
CLICK_ENDDECLS
#endif
