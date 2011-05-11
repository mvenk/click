#ifndef CLICK_BASHRADIXIPLOOKUP_HH
#define CLICK_BASHRADIXIPLOOKUP_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup104;

class BashRadixIPLookup : public Element { public:

    BashRadixIPLookup();
    ~BashRadixIPLookup();

    const char *class_name() const	{ return "BashRadixIPLookup"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup104 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
