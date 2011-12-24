#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookupm107.hh"
#include "radixiplookup107.hh"
#include  "167k_input.hh"
#include <arpa/inet.h>
CLICK_DECLS

PoundRadixIPLookupM107::PoundRadixIPLookupM107()
    : _task(this) {
}

PoundRadixIPLookupM107::~PoundRadixIPLookupM107() {
}

int
PoundRadixIPLookupM107::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup107", &_l,
			cpEnd);
}

int
PoundRadixIPLookupM107::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}


bool
PoundRadixIPLookupM107::run_task(Task *) {
  IPAddress ip, gw(0);
  int  n = 1000;
  for(int k=0;k<n;k++)
    {
       IPRoute r =get_route_for_update(k);
       // call add_route with route, replace =true, oldroute = NULL, errhandler =NULL
      _l->add_route(r, true, 0, 0);
    }
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PoundRadixIPLookupM107)
