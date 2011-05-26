#ifndef CLICK_BASHRADIXIPLOOKUP105_HH
#define CLICK_BASHRADIXIPLOOKUP105_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup105;

class BashRadixIPLookup105 : public Element { public:

    BashRadixIPLookup105();
    ~BashRadixIPLookup105();

    const char *class_name() const	{ return "BashRadixIPLookup105"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup105 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
