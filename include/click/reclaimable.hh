#ifndef CLICK_RECLAIMABLE_HH
#define CLICK_RECLAIMABLE_HH
CLICK_DECLS

class Reclaimable { public:    
    virtual void reclaim() = 0;
};


CLICK_ENDDECLS
#endif
