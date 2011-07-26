#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookup107.hh"
#include "radixiplookup107.hh"
CLICK_DECLS

ReadRadixIPLookup107::ReadRadixIPLookup107()
    : _task(this) {
}

ReadRadixIPLookup107::~ReadRadixIPLookup107() {
}

int
ReadRadixIPLookup107::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup107", &_l,
			cpEnd);
}

int
ReadRadixIPLookup107::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
ReadRadixIPLookup107::run_task(Task *) {
  IPAddress ip(htonl(0x01010100)), gw(0);
  int port = 0;
  for(int k=0;k<100000;k++) {
      port = _l->lookup_route(ip, gw);
      // click_chatter("Port: %d GW: %d\n", port, gw.addr());
  }
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadixIPLookup107)
