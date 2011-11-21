#ifndef CLICK_READRADIXIPLOOKUPM_HH
#define CLICK_READRADIXIPLOOKUPM_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup;


class ReadRadixIPLookupM : public Element { public:

    ReadRadixIPLookupM();
    ~ReadRadixIPLookupM();

  const char *class_name() const	{ return "ReadRadixIPLookupM"; }

  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  bool run_task(Task *task);

private:

  RadixIPLookup *_l;
  Task _task;
  

  
};

CLICK_ENDDECLS
#endif
