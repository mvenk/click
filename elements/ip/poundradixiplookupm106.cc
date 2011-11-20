#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "poundradixiplookupm106.hh"
#include "radixiplookup106.hh"
#include  "167k_table.hh"
#include <arpa/inet.h>
CLICK_DECLS

PoundRadixIPLookupM106::PoundRadixIPLookupM106()
    : _task(this) {
}

PoundRadixIPLookupM106::~PoundRadixIPLookupM106() {
}

int
PoundRadixIPLookupM106::configure(Vector<String> &conf, ErrorHandler *errh) {
    return cp_va_kparse(conf, this, errh,
			"RADIX", cpkP+cpkM, cpElementCast, "RadixIPLookup106", &_l,
			cpEnd);
}

int
PoundRadixIPLookupM106::initialize(ErrorHandler *errh) {
    ScheduleInfo::initialize_task(this, &_task, true, errh);
    return 0;
}

inline int 
PoundRadixIPLookupM106::get_table_size(void){
    return sizeof(_rtable)/sizeof(rtable);
}

void
PoundRadixIPLookupM106::initialize_rand(){
    srand(time(NULL));
}

IPRoute
PoundRadixIPLookupM106::get_route_for_update(){

    // generate through random number generator
    int addr_select = 0;
    struct sockaddr_in sa;
    const char *addr;
    int random_val;
    int table_size = get_table_size();
    addr_select = rand() % table_size;
    addr = _rtable[addr_select].addr;
    int suffix = 32 - _rtable[addr_select].prefix;
    int prefix = _rtable[addr_select].prefix;
    int subnet_mask;
    random_val= (!suffix)? 0 : (rand() % suffix);

     // convert the IP address string in the table into a 32-bit IP address
    inet_pton(AF_INET, addr, &(sa.sin_addr));     
    int addr_value = htonl(sa.sin_addr.s_addr);

    // append the random value to the address
    addr_value = addr_value + random_val;
    int lookup_addr = htonl(addr_value);

    // wrap and serve
    IPAddress ip(lookup_addr);
    if(prefix ==0)
	subnet_mask = 0;
      else
	subnet_mask = 0xFFFFFFFF << (32-prefix);

    IPRoute route(ip,
	      IPAddress(htonl(subnet_mask)),
	      IPAddress(htonl(0x01010101)),
	       0);
    //click_chatter("\n%s  addr: %08x added: %08x addr %08x lookup %08x ",addr, addr_value,random_val,addr_value,lookup_addr);
    return route;
}


bool
PoundRadixIPLookupM106::run_task(Task *) {
  initialize_rand();
  IPAddress ip, gw(0);
  for(int k=0;k<100;k++)
    {
       IPRoute r =get_route_for_update();
       // call add_route with route, replace =true, oldroute = NULL, errhandler =NULL
      _l->add_route(r, true, 0, 0);
    }
  _task.fast_reschedule();
  return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PoundRadixIPLookupM106)
