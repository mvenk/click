#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookupm.hh"
#include "radixiplookup.hh"
#include  "167k_input.hh"
#include <arpa/inet.h>
CLICK_DECLS

ReadRadixIPLookupM::ReadRadixIPLookupM()
    : _task(this) {
}

ReadRadixIPLookupM::~ReadRadixIPLookupM() {
}

int
ReadRadixIPLookupM::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
ReadRadixIPLookupM::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
ReadRadixIPLookupM::run_task(Task *) {
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
EXPORT_ELEMENT(ReadRadixIPLookupM)
