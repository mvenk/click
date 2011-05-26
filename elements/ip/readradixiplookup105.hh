#ifndef CLICK_READRADIXIPLOOKUP105_HH
#define CLICK_READRADIXIPLOOKUP105_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup105;

class ReadRadixIPLookup105 : public Element { public:

    ReadRadixIPLookup105();
    ~ReadRadixIPLookup105();

    const char *class_name() const	{ return "ReadRadixIPLookup105"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

    bool run_task(Task *task);

  private:

    RadixIPLookup105 *_l;
    Task _task;

};

CLICK_ENDDECLS
#endif
