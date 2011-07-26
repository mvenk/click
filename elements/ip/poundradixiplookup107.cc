#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookup107.hh"
#include "radixiplookup107.hh"
CLICK_DECLS

PoundRadixIPLookup107::PoundRadixIPLookup107()
    : _task(this) {
}

PoundRadixIPLookup107::~PoundRadixIPLookup107() {
}

int
PoundRadixIPLookup107::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup107", &_l,
			cpEnd);
}

int
PoundRadixIPLookup107::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
PoundRadixIPLookup107::run_task(Task *) {
    IPRoute r(IPAddress(htonl(0x01010100)),
	      IPAddress(htonl(0xFFFFFF00)),
	      IPAddress(htonl(0x01010101)),
	      1);
    for(int k=0;k<10000;k++)
      {
	_l->add_route(r, true, 0, 0);
	_l->remove_route(r,0,0);
      }
     _l->add_route(r, true, 0, 0);
     _task.fast_reschedule();
    return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PoundRadixIPLookup107)
