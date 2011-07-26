#ifndef CLICK_POUNDRADIXIPLOOKUP107_HH
#define CLICK_POUNDRADIXIPLOOKUP107_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup107;

class PoundRadixIPLookup107 : public Element { public:

    PoundRadixIPLookup107();
    ~PoundRadixIPLookup107();

    const char *class_name() const	{ return "PoundRadixIPLookup107"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup107 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
