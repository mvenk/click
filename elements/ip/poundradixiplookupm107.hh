#ifndef CLICK_POUNDRADIXIPLOOKUPM107_HH
#define CLICK_POUNDRADIXIPLOOKUPM107_HH
#include <click/element.hh>
#include <click/task.hh>
#include "iproutetable.hh"
CLICK_DECLS
class RadixIPLookup107;

class PoundRadixIPLookupM107 : public Element { public:

    PoundRadixIPLookupM107();
    ~PoundRadixIPLookupM107();

    const char *class_name() const	{ return "PoundRadixIPLookupM107"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

  bool run_task(Task *task);

private:
  RadixIPLookup107 *_l;
  Task _task;

};

CLICK_ENDDECLS
#endif
