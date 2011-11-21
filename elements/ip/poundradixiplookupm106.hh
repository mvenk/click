#ifndef CLICK_POUNDRADIXIPLOOKUPM106_HH
#define CLICK_POUNDRADIXIPLOOKUPM106_HH
#include <click/element.hh>
#include <click/task.hh>
#include "iproutetable.hh"
CLICK_DECLS
class RadixIPLookup106;

class PoundRadixIPLookupM106 : public Element { public:

    PoundRadixIPLookupM106();
    ~PoundRadixIPLookupM106();

    const char *class_name() const	{ return "PoundRadixIPLookupM106"; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    int initialize(ErrorHandler *errh);

  bool run_task(Task *task);

private:
  RadixIPLookup106 *_l;
  Task _task;

};

CLICK_ENDDECLS
#endif
