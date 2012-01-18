#include <click/config.h>

#include <click/glue.hh>
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

/** @brief Acquires the ReadWriteLock for reading.
 *
 * On return, this thread has acquired the lock for reading.  The function
 * will spin indefinitely until the lock is acquired.  It is OK to acquire a
 * lock you have already acquired, but you must release it as many times as
 * you have acquired it.
 *
 * @sa Spinlock::acquire
 */
void
ReadWriteLock::acquire_read()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    click_processor_t my_cpu = click_get_processor();
    _l[my_cpu]._lock.acquire();
#endif
}

/** @brief Attempts to acquire the ReadWriteLock for reading.
 * @return True iff the ReadWriteLock was acquired.
 *
 * This function will acquire the lock for reading and return true only if the
 * ReadWriteLock can be acquired right away, without retries.
 */
bool
ReadWriteLock::attempt_read()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    click_processor_t my_cpu = click_get_processor();
    bool result = _l[my_cpu]._lock.attempt();
    if (!result)
	click_put_processor();
    return result;
#else
    return true;
#endif
}

/** @brief Releases the ReadWriteLock for reading.
 *
 * The ReadWriteLock must have been previously acquired by either
 * ReadWriteLock::acquire_read or ReadWriteLock::attempt_read.  Do not call
 * release_read() on a lock that was acquired for writing.
 */
void
ReadWriteLock::release_read()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    _l[click_current_processor()]._lock.release();
    click_put_processor();
#endif
}

/** @brief Acquires the ReadWriteLock for writing.
 *
 * On return, this thread has acquired the lock for writing.  The function
 * will spin indefinitely until the lock is acquired.  It is OK to acquire a
 * lock you have already acquired, but you must release it as many times as
 * you have acquired it.
 *
 * @sa ReadWriteLock::acquire_read
 */
void
ReadWriteLock::acquire_write()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    for (unsigned i = 0; i < num_possible_cpus(); i++)
	_l[i]._lock.acquire();
#endif
}

/** @brief Attempts to acquire the ReadWriteLock for writing.
 * @return True iff the ReadWriteLock was acquired.
 *
 * This function will acquire the lock for writing and return true only if the
 * ReadWriteLock can be acquired right away, without retries.  Note, however,
 * that acquiring a ReadWriteLock requires as many operations as there are
 * CPUs.
 *
 * @sa ReadWriteLock::attempt_read
 */
bool
ReadWriteLock::attempt_write()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    bool all = true;
    unsigned i;
    for (i = 0; i < num_possible_cpus(); i++)
	if (!(_l[i]._lock.attempt())) {
	    all = false;
	    break;
	}
    if (!all)
	for (unsigned j = 0; j < i; j++)
	    _l[j]._lock.release();
    return all;
#else
    return true;
#endif
}

/** @brief Releases the ReadWriteLock for writing.
 *
 * The ReadWriteLock must have been previously acquired by either
 * ReadWriteLock::acquire_write or ReadWriteLock::attempt_write.  Do not call
 * release_write() on a lock that was acquired for reading.
 *
 * @sa ReadWriteLock::release_read
 */
void
ReadWriteLock::release_write()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    for (unsigned i = 0; i < num_possible_cpus(); i++)
	_l[i]._lock.release();
#endif
}

void
ReadWriteLockUser::acquire_read()
{
    // acquire lock to prevent writers
    _lock.acquire();
    _readers++;
    _lock.release();
}

bool
ReadWriteLockUser::attempt_read() 
{
    bool result = _lock.attempt();
    if(result) {
	_readers++;
	_lock.release();
	return true;
    }
    return false;
}
	

void
ReadWriteLockUser::release_read()
{
    _readers--;
}

void
ReadWriteLockUser::acquire_write()
{
    // acquire lock to block any future readers
    _lock.acquire();
    // spin till all readers are done
    while(_readers > 0) asm volatile ("" : : : "memory");
}

bool
ReadWriteLockUser::attempt_write()
{
    if(_lock.attempt()) {
	if(_readers == 0)
	    return true;
	else
	    _lock.release();
    }
    return false;
}
    
void
ReadWriteLockUser::release_write()
{
    _lock.release();
}

CLICK_ENDDECLS
