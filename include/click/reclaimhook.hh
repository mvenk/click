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

    inline void fire();

    private:
    Element *_owner;
    void * _thunk;
    friend class RouterThread;

};

inline
ReclaimHook::ReclaimHook(Reclaimable *r)
    :_thunk(r){

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
