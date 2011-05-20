// -*- c-basic-offset: 4;
#ifndef CLICK_RECLAIMER_HH
#define CLICK_RECLAIMER_HH
#include <click/element.hh>
#include <click/task.hh>
#include <click/reclaimable.hh>
#include <click/sync.hh>
#include <click/router.hh>
#include <click/master.hh>
#include <click/hook.hh>
CLICK_DECLS
class RouterThread;
class Master;

class ReclaimHook:public Hook {public:

    inline ReclaimHook(Reclaimable *);
    inline ~ReclaimHook() {}

    inline void initialize(Element *, Task *, bool);

    inline void fire();

    private:
    Element *_owner;
    Task *_task;
    RouterThread *_thread;
    void * _thunk;
    friend class RouterThread;

};

inline
ReclaimHook::ReclaimHook(Reclaimable *r)
    :_thunk(r){

}

void
ReclaimHook::initialize(Element *owner, Task *task, bool schedule) {
    assert(owner);
    Router *router = owner->router();
    int tid = router->initial_home_thread_id(owner, task, schedule);
    if(tid == ThreadSched::THREAD_UNKNOWN)
	tid = 0;
    _thread = router->master()->thread(tid);
    _owner = owner;
    _task = task;
    _thread->add_reclaim_hook(this);
}

void
ReclaimHook::fire() {
  if(_thunk) {
    ((Reclaimable *)_thunk)->reclaim();
  }
}
CLICK_ENDDECLS
#endif
