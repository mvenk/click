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

    inline void initialize(Element *);
    inline bool scheduled()  { return _is_scheduled; }
    inline void schedule()   { _is_scheduled = true; } 
    inline void unschedule() { _is_scheduled = false; }
    inline void fire();

    private:
    Element *_owner;
    void * _thunk;
    bool _is_scheduled;
    

};

inline
ReclaimHook::ReclaimHook(Reclaimable *r)
    :_thunk(r), _is_scheduled(false){

}

void
ReclaimHook::initialize(Element *owner) {
    assert(owner);
    Router *router = owner->router();    
    router->master()->add_reclaim_hook(this);
    _owner = owner;
}


void
ReclaimHook::fire() {
  if(_thunk) {
    ((Reclaimable *)_thunk)->reclaim();
  }
}
CLICK_ENDDECLS
#endif
