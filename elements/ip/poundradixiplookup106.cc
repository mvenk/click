#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookup106.hh"
#include "radixiplookup106.hh"
CLICK_DECLS

PoundRadixIPLookup106::PoundRadixIPLookup106()
    : _task(this) {
}

PoundRadixIPLookup106::~PoundRadixIPLookup106() {
}

int
PoundRadixIPLookup106::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
PoundRadixIPLookup106::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
PoundRadixIPLookup106::run_task(Task *) {
    IPRoute r(IPAddress(htonl(0x01010100)),
	      IPAddress(htonl(0xFFFFFF00)),
	      IPAddress(htonl(0x01010101)),
	      1);
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
EXPORT_ELEMENT(PoundRadixIPLookup106)
