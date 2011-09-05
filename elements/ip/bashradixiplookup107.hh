#ifndef CLICK_BASHRADIXIPLOOKUP107_HH
#define CLICK_BASHRADIXIPLOOKUP107_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup107;

class BashRadixIPLookup107 : public Element { public:

    BashRadixIPLookup107();
    ~BashRadixIPLookup107();

    const char *class_name() const	{ return "BashRadixIPLookup107"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup107 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
