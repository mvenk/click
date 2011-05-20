#ifndef CLICK_READRADIX_HH
#define CLICK_READRADIX_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup106;

class ReadRadix : public Element { public:

    ReadRadix();
    ~ReadRadix();

    const char *class_name() const	{ return "ReadRadix"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup106 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
