#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "bashradixiplookup107.hh"
#include "radixiplookup107.hh"
CLICK_DECLS

BashRadixIPLookup107::BashRadixIPLookup107()
    : _task(this) {
}

BashRadixIPLookup107::~BashRadixIPLookup107() {
}

int
BashRadixIPLookup107::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup107", &_l,
			cpEnd);
}

int
BashRadixIPLookup107::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
BashRadixIPLookup107::run_task(Task *) {
    IPRoute r(IPAddress(htonl(0x10101000)),
	      IPAddress(htonl(0xFFFFFF00)),
	      IPAddress(htonl(0xA1A2A3A)),
	      0);
    for(int k=0;k<100;k++)
      {
	_l->add_route(r, true, 0, 0);
	_l->remove_route(r,0,0);
      }
     _l->add_route(r, true, 0, 0);
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(BashRadixIPLookup107)
