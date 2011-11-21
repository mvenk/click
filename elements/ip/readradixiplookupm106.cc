#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookupm106.hh"
#include "radixiplookup106.hh"
#include <arpa/inet.h>
CLICK_DECLS

ReadRadixIPLookupM106::ReadRadixIPLookupM106()
    : _task(this) {
}

ReadRadixIPLookupM106::~ReadRadixIPLookupM106() {
}

int
ReadRadixIPLookupM106::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
ReadRadixIPLookupM106::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
ReadRadixIPLookupM106::run_task(Task *) {
    IPAddress ip, gw(0);
    int port = 0;
    for(int k=0;k<100000;k++) {
        ip =get_ip_for_lookup(k%1000);  
        port = _l->lookup_route(ip, gw);
    }
    _task.fast_reschedule();
    return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadixIPLookupM106)
