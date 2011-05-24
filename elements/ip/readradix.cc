#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradix.hh"
#include "radixiplookup106.hh"
#include <stdio.h>
CLICK_DECLS

ReadRadix::ReadRadix()
    : _task(this) {
}

ReadRadix::~ReadRadix() {
}

int
ReadRadix::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
ReadRadix::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

bool
ReadRadix::run_task(Task *) {
  IPAddress ip(htonl(0x01010100)), gw(0);
  int port = 0;
  for(int k=0;k<100;k++) {
      port = _l->lookup_route(ip, gw);
      printf("Port: %d GW: %d\n", port, gw.addr());
  }
  return false;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadix)
