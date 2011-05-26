#ifndef CLICK_POUNDRADIXIPLOOKUP106_HH
#define CLICK_POUNDRADIXIPLOOKUP106_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup106;

class PoundRadixIPLookup106 : public Element { public:

    PoundRadixIPLookup106();
    ~PoundRadixIPLookup106();

    const char *class_name() const	{ return "PoundRadixIPLookup106"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup106 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
