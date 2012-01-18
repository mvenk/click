
// -*- c-basic-offset: 4 -*-
#ifndef CLICK_SYNC_HH
#define CLICK_SYNC_HH
#include <click/glue.hh>
#include <click/atomic.hh>
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
# include <click/cxxprotect.h>
CLICK_CXX_PROTECT
# include <linux/threads.h>
# include <linux/sched.h>
# if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
#  define num_possible_cpus()	smp_num_cpus
# endif
CLICK_CXX_UNPROTECT
# include <click/cxxunprotect.h>
#endif
#if CLICK_LINUXMODULE || (CLICK_USERLEVEL && HAVE_MULTITHREAD)
# define CLICK_MULTITHREAD_SPINLOCK 1
#endif
#if CLICK_USERLEVEL && !NDEBUG
# define SPINLOCK_ASSERTLEVEL "<-999>"
#else
# define SPINLOCK_ASSERTLEVEL "<1>"
#endif
CLICK_DECLS

/** @file <click/sync.hh>
 * @brief Classes for synchronizing among multiple CPUs, particularly in the
 * Linux kernel.
 */

/** @class Spinlock
 * @brief A spinlock for SMP Click threads.
 *
 * The Spinlock class abstracts a spinlock, or polling mutex, in SMP Click.
 * This is a type of mutual-exclusion lock in which acquiring the lock is a
 * polling operation (basically a "while (lock.acquired()) do nothing;" loop).
 * Spinlocks can be used to synchronize access to shared data among multiple
 * Click SMP threads.  Spinlocks should not be held for long periods of time:
 * use them for quick updates and such.
 *
 * Spinlock operations do nothing unless Click was compiled with SMP support
 * (with --enable-multithread).  Therefore, Spinlock should not be used to,
 * for example, synchronize handlers with main element threads.  See also
 * SpinlockIRQ.
 *
 * The main Spinlock operations are acquire(), which acquires the lock, and
 * release(), which releases the lock.  attempt() acquires the lock only if it
 * can be acquired instantaneously.
 *
 * It is OK for a thread to acquire a lock it has already acquired, but you
 * must release it as many times as you have acquired it.
 */
class Spinlock { public:

    inline Spinlock();
    inline ~Spinlock();

    void acquire();
    inline void release();
    inline bool attempt();
    inline bool nested() const;

#if CLICK_MULTITHREAD_SPINLOCK
  private:

    atomic_uint32_t _lock;
    int32_t _depth;
    click_processor_t _owner;
#endif

};

/** @brief Create a Spinlock. */
inline
Spinlock::Spinlock()
#if CLICK_MULTITHREAD_SPINLOCK
    : _depth(0), _owner(click_invalid_processor())
#endif
{
#if CLICK_MULTITHREAD_SPINLOCK
    _lock = 0;
#endif
}

inline
Spinlock::~Spinlock()
{
#if CLICK_MULTITHREAD_SPINLOCK
    if (_depth != 0)
	click_chatter(SPINLOCK_ASSERTLEVEL "Spinlock::~Spinlock(): assertion \"_depth == 0\" failed");
#endif
}







/** @BRIEF Attempts to acquire the Spinlock.
 * @return True iff the Spinlock was acquired.
 *
 * This function will acquire the lock and return true only if the Spinlock
 * can be acquired right away, without retries.
 */
inline bool
Spinlock::attempt()
{
#if CLICK_MULTITHREAD_SPINLOCK
    click_processor_t my_cpu = click_get_processor();
    if (_owner != my_cpu) {
	if (_lock.swap(1) != 0) {
	    click_put_processor();
	    return false;
	}
	_owner = my_cpu;
    }
    _depth++;
    return true;
#else
    return true;
#endif
}

/** @brief Releases the Spinlock.
 *
 * The Spinlock must have been previously acquired by either Spinlock::acquire
 * or Spinlock::attempt.
 */
inline void
Spinlock::release()
{
#if CLICK_MULTITHREAD_SPINLOCK
    if (unlikely(_owner != click_current_processor()))
	click_chatter(SPINLOCK_ASSERTLEVEL "Spinlock::release(): assertion \"owner == click_current_processor()\" failed");
    if (likely(_depth > 0)) {
	if (--_depth == 0) {
	    _owner = click_invalid_processor();
	    _lock = 0;
	}
    } else
	click_chatter(SPINLOCK_ASSERTLEVEL "Spinlock::release(): assertion \"_depth > 0\" failed");
    click_put_processor();
#endif
}

/** @brief Returns true iff the Spinlock has been acquired more than once by
 * the current thread.
 */
inline bool
Spinlock::nested() const
{
#if CLICK_MULTITHREAD_SPINLOCK
    return _depth > 1;
#else
    return false;
#endif
}


/** @class SpinlockIRQ
 * @brief A spinlock that disables interrupts.
 *
 * The SpinlockIRQ class abstracts a spinlock, or polling mutex, that also
 * turns off interrupts.  Spinlocks are a type of mutual-exclusion lock in
 * which acquiring the lock is a polling operation (basically a "while
 * (lock.acquired()) do nothing;" loop).  The SpinlockIRQ variant can be used
 * to protect Click data structures from interrupts and from other threads.
 * Very few objects in Click need this protection; the Click Master object,
 * which protects the task list, uses it, but that's hidden from users.
 * Spinlocks should not be held for long periods of time: use them for quick
 * updates and such.
 *
 * In the Linux kernel, SpinlockIRQ is equivalent to a combination of
 * local_irq_save and the spinlock_t type.
 *
 * The SpinlockIRQ operations are acquire(), which acquires the lock, and
 * release(), which releases the lock.
 *
 * It is NOT OK for a SpinlockIRQ thread to acquire a lock it has already
 * acquired.
 */
class SpinlockIRQ { public:

    inline SpinlockIRQ();

#if CLICK_LINUXMODULE
    typedef unsigned long flags_t;
#else
    typedef int flags_t;
#endif

    inline flags_t acquire();
    inline void release(flags_t);

#if CLICK_LINUXMODULE
  private:
    spinlock_t _lock;
#elif CLICK_USERLEVEL && HAVE_MULTITHREAD
  private:
    Spinlock _lock;
#endif

};

/** @brief Creates a SpinlockIRQ. */
inline
SpinlockIRQ::SpinlockIRQ()
{
#if CLICK_LINUXMODULE
    spin_lock_init(&_lock);
#endif
}

/** @brief Acquires the SpinlockIRQ.
 * @return The current state of the interrupt flags.
 */
inline SpinlockIRQ::flags_t
SpinlockIRQ::acquire()
{
#if CLICK_LINUXMODULE
    flags_t flags;
    spin_lock_irqsave(&_lock, flags);
    return flags;
#elif CLICK_USERLEVEL && HAVE_MULTITHREAD
    _lock.acquire();
    return 0;
#else
    return 0;
#endif
}

/** @brief Releases the SpinlockIRQ.
 * @param flags The value returned by SpinlockIRQ::acquire().
 */
inline void
SpinlockIRQ::release(flags_t flags)
{
#if CLICK_LINUXMODULE
    spin_unlock_irqrestore(&_lock, flags);
#elif CLICK_USERLEVEL && HAVE_MULTITHREAD
    (void) flags;
    _lock.release();
#else
    (void) flags;
#endif
}


// read-write lock
//
// on read: acquire local read lock
// on write: acquire every read lock
//
// alternatively, we could use a read counter and a write lock. we don't do
// that because we'd like to avoid a cache miss for read acquires. this makes
// reads very fast, and writes more expensive

/** @class ReadWriteLock
 * @brief A read/write lock.
 *
 * The ReadWriteLock class abstracts a read/write lock in SMP Click.  Multiple
 * SMP Click threads can hold read locks simultaneously, but if any thread
 * holds a write lock, then no other thread holds any kind of lock.  The
 * read/write lock is implemented with Spinlock objects, so acquiring a lock
 * is a polling operation.  ReadWriteLocks can be used to synchronize access
 * to shared data among multiple Click SMP threads.  ReadWriteLocks should not
 * be held for long periods of time.
 *
 * ReadWriteLock operations do nothing unless Click was compiled with
 * --enable-multithread.  Therefore, ReadWriteLock should not be used to, for
 * example, synchronize handlers with main element threads.
 *
 * The main ReadWriteLock operations are acquire_read() and acquire_write(),
 * which acquire the lock for reading or writing, respectively, and
 * release_read() and release_write(), which similarly release the lock.
 * attempt_read() and attempt_write() acquire the lock only if it can be
 * acquired instantaneously.
 *
 * It is OK for a thread to acquire a lock it has already acquired, but you
 * must release it as many times as you have acquired it.
 *
 * ReadWriteLock objects are relatively large in terms of memory usage; don't
 * create too many of them.
 */
class ReadWriteLock { public:

    inline ReadWriteLock();
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    inline ~ReadWriteLock();
#endif

    void acquire_read();
    bool attempt_read();
    void release_read();
    void acquire_write();
    bool attempt_write();
    void release_write();

#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
  private:
    // allocate 32 bytes (size of a cache line) for every member
    struct lock_t {
	Spinlock _lock;
	unsigned char reserved[32 - sizeof(Spinlock)];
    } *_l;
#endif

};

/** @brief Creates a ReadWriteLock. */
inline
ReadWriteLock::ReadWriteLock()
{
#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
    _l = new lock_t[num_possible_cpus()];
#endif
}

#if CLICK_LINUXMODULE && defined(CONFIG_SMP)
inline
ReadWriteLock::~ReadWriteLock()
{
    delete[] _l;
}
#endif

/* A user-level Read Write Lock*/
class ReadWriteLockUser { public:
    inline ReadWriteLockUser();
    inline ~ReadWriteLockUser();

    void acquire_read();
    void release_read();
    bool attempt_read();

    void acquire_write();
    void release_write();
    bool attempt_write();

private:
    atomic_uint32_t _readers;
    Spinlock _lock;    
};

inline
ReadWriteLockUser :: ReadWriteLockUser() 
{
    _readers = 0;
}

inline
ReadWriteLockUser :: ~ReadWriteLockUser()
{
}

CLICK_ENDDECLS
#undef SPINLOCK_ASSERTLEVEL
#endif
