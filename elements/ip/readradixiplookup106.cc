#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookup106.hh"
#include "radixiplookup106.hh"

CLICK_DECLS

ReadRadixIPLookup106::ReadRadixIPLookup106()
    : _task(this) {
}

ReadRadixIPLookup106::~ReadRadixIPLookup106() {
}

int
ReadRadixIPLookup106::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
ReadRadixIPLookup106::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
ReadRadixIPLookup106::run_task(Task *) {
  IPAddress ip(htonl(0x01010100)), gw(0);
  int port = 0;
  for(int k=0;k<1000000;k++) {
      port = _l->lookup_route(ip, gw);
      // click_chatter("Port: %d GW: %d\n", port, gw.addr());
  }
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadixIPLookup106)
