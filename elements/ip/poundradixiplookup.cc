#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookup.hh"
#include "radixiplookup105.hh"
CLICK_DECLS

PoundRadixIPLookup::PoundRadixIPLookup()
    : _task(this) {
}

PoundRadixIPLookup::~PoundRadixIPLookup() {
}

int
PoundRadixIPLookup::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup105", &_l,
			cpEnd);
}

int
PoundRadixIPLookup::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
PoundRadixIPLookup::run_task(Task *) {
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
    return false;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PoundRadixIPLookup)
