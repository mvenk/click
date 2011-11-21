#ifndef CLICK_READRADIXIPLOOKUP106M_HH
#define CLICK_READRADIXIPLOOKUP106M_HH
#include <click/element.hh>
#include <click/task.hh>
#include "167k_input.hh"
CLICK_DECLS
class RadixIPLookup106;


class ReadRadixIPLookupM106 : public Element { public:

    ReadRadixIPLookupM106();
    ~ReadRadixIPLookupM106();

  const char *class_name() const	{ return "ReadRadixIPLookupM106"; }

  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  bool run_task(Task *task);

private:

  RadixIPLookup106 *_l;
  Task _task;
  

  
};

CLICK_ENDDECLS
#endif
