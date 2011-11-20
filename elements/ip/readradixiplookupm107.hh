#ifndef CLICK_READRADIXIPLOOKUP107M_HH
#define CLICK_READRADIXIPLOOKUP107M_HH
#include <click/element.hh>
#include <click/task.hh>
CLICK_DECLS
class RadixIPLookup107;


class ReadRadixIPLookupM107 : public Element { public:

    ReadRadixIPLookupM107();
    ~ReadRadixIPLookupM107();

  const char *class_name() const	{ return "ReadRadixIPLookupM107"; }

  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  bool run_task(Task *task);

private:
  inline int get_table_size(void);
  void initialize_rand(void);
  IPAddress get_ip_for_lookup(void);

  RadixIPLookup107 *_l;
  Task _task;
  

  
};

CLICK_ENDDECLS
#endif
