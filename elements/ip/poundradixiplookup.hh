#ifndef CLICK_POUNDRADIXIPLOOKUP_HH
#define CLICK_POUNDRADIXIPLOOKUP_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup101;

class PoundRadixIPLookup : public Element { public:

    PoundRadixIPLookup();
    ~PoundRadixIPLookup();

    const char *class_name() const	{ return "PoundRadixIPLookup"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup101 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
