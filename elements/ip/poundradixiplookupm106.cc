#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookupm106.hh"
#include "radixiplookup106.hh"
#include  "167k_input.hh"
#include <arpa/inet.h>
CLICK_DECLS

PoundRadixIPLookupM106::PoundRadixIPLookupM106()
    : _task(this) {
}

PoundRadixIPLookupM106::~PoundRadixIPLookupM106() {
}

int
PoundRadixIPLookupM106::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
PoundRadixIPLookupM106::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
PoundRadixIPLookupM106::run_task(Task *) {
  IPAddress ip, gw(0);
  for(int k=0;k<1000;k++)
    {
       IPRoute r =get_route_for_update(k);
       // call add_route with route, replace =true, oldroute = NULL, errhandler =NULL
      _l->add_route(r, true, 0, 0);
    }
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PoundRadixIPLookupM106)
