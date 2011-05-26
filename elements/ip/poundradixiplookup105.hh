#ifndef CLICK_POUNDRADIXIPLOOKUP105_HH
#define CLICK_POUNDRADIXIPLOOKUP105_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup105;

class PoundRadixIPLookup105 : public Element { public:

    PoundRadixIPLookup105();
    ~PoundRadixIPLookup105();

    const char *class_name() const	{ return "PoundRadixIPLookup105"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup105 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
