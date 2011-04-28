#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "bashradixiplookup.hh"
#include "radixiplookup100.hh"
CLICK_DECLS

BashRadixIPLookup::BashRadixIPLookup()
    : _task(this) {
}

BashRadixIPLookup::~BashRadixIPLookup() {
}

int
BashRadixIPLookup::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup100", &_l,
			cpEnd);
}

int
BashRadixIPLookup::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
BashRadixIPLookup::run_task(Task *) {
    IPRoute r(IPAddress(htonl(0x10101000)),
	      IPAddress(htonl(0xFFFFFF00)),
	      IPAddress(htonl(0xA1A2A3A)),
	      0);
    for(int k=0;k<10000;k++)
      {
	_l->add_route(r, true, 0, 0);
	_l->remove_route(r,0,0);
      }
     _l->add_route(r, true, 0, 0);
    return false;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(BashRadixIPLookup)
