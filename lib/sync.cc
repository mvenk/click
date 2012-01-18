#include <click/sync.hh>

CLICK_DECLS

/** @brief Acquires the Spinlock.
 *
 * On return, this thread has acquired the lock.  The function will spin
 * indefinitely until the lock is acquired.  It is OK to acquire a lock you
 * have already acquired, but you must release it as many times as you have
 * acquired it.
 */
void
Spinlock::acquire()
{
#if CLICK_MULTITHREAD_SPINLOCK
  click_processor_t my_cpu = click_get_processor();
  if (_owner != my_cpu) {
    while (_lock.swap(1) != 0)
      while (_lock != 0)
	asm volatile ("" : : : "memory");
    _owner = my_cpu;
  }
  _depth++;
#endif
}

CLICK_ENDDECLS
