#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookupm107.hh"
#include "radixiplookup107.hh"
#include "167k_input.hh"
#include <arpa/inet.h>
CLICK_DECLS

ReadRadixIPLookupM107::ReadRadixIPLookupM107()
    : _task(this) {
}

ReadRadixIPLookupM107::~ReadRadixIPLookupM107() {
}

int
ReadRadixIPLookupM107::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup107", &_l,
			cpEnd);
}

int
ReadRadixIPLookupM107::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}


bool
ReadRadixIPLookupM107::run_task(Task *) {

    IPAddress ip, gw(0);
    int port = 0;
    int n = 100000;
    for(int k=0;k<n;k++) {
        ip =get_ip_for_lookup(k);  
        port = _l->lookup_route(ip, gw);
    }
    click_chatter("Attempts: %u",_l->get_attempts());
    _task.fast_reschedule();
    return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadixIPLookupM107)
