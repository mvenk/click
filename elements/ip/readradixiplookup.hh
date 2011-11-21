#ifndef CLICK_READRADIXIPLOOKUP_HH
#define CLICK_READRADIXIPLOOKUP_HH
#include <click/element.hh>
#include <click/task.hh>
#include "167k_input.hh"
CLICK_DECLS
class RadixIPLookup;

class ReadRadixIPLookup : public Element { public:

    ReadRadixIPLookup();
    ~ReadRadixIPLookup();

    const char *class_name() const	{ return "ReadRadixIPLookup"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
