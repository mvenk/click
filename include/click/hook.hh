// -*- c-basic-offset:4
#ifndef CLICK_HOOK
#define CLICK_HOOK

CLICK_DECLS

class Hook {public:
    virtual void fire() = 0;
    virtual bool scheduled() = 0;
    virtual void schedule() = 0;
    virtual void unschedule() = 0;
};

CLICK_ENDDECLS
#endif
