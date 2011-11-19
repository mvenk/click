#ifndef CLICK_READRADIXIPLOOKUP106M_HH
#define CLICK_READRADIXIPLOOKUP106M_HH
#include <click/element.hh>
#include <click/task.hh>
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
  inline int get_table_size(void);
  void initialize_rand(void);
  IPAddress get_ip_for_lookup(void);

  RadixIPLookup106 *_l;
  Task _task;
  

  
};

CLICK_ENDDECLS
#endif
