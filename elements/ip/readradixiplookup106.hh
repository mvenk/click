#ifndef CLICK_READRADIXIPLOOKUP106_HH
#define CLICK_READRADIXIPLOOKUP106_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup106;

class ReadRadixIPLookup106 : public Element { public:

    ReadRadixIPLookup106();
    ~ReadRadixIPLookup106();

    const char *class_name() const	{ return "ReadRadixIPLookup106"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup106 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
