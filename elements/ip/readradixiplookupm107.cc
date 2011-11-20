#include <click/config.h>
#include <click/confparse.hh>
#include <click/standard/scheduleinfo.hh>
#include "readradixiplookupm107.hh"
#include "radixiplookup107.hh"
#include  "167k_table.hh"
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

inline int 
ReadRadixIPLookupM107::get_table_size(void){
    return sizeof(_rtable)/sizeof(rtable);
}

void
ReadRadixIPLookupM107::initialize_rand(){
    srand(time(NULL));
}

IPAddress
ReadRadixIPLookupM107::get_ip_for_lookup(){

    // generate through random number generator
    int addr_select = 0;
    struct sockaddr_in sa;
    const char *addr;
    int random_val;
    int table_size = get_table_size();
    addr_select = rand() % table_size;
    addr = _rtable[addr_select].addr;
    int suffix = 32 - _rtable[addr_select].prefix;
    random_val= (!suffix)? 0 : (rand() % suffix);

     // convert the IP address string in the table into a 32-bit IP address
    inet_pton(AF_INET, addr, &(sa.sin_addr));     
    int addr_value = htonl(sa.sin_addr.s_addr);

    // append the random value to the address
    addr_value = addr_value + random_val;
    int lookup_addr = htonl(addr_value);

    // wrap and serve
    IPAddress ip(lookup_addr);
    //click_chatter("\n%s  addr: %08x added: %08x addr %08x lookup %08x ",addr, addr_value,random_val,addr_value,lookup_addr);
    return ip;
}

bool
ReadRadixIPLookupM107::run_task(Task *) {
    initialize_rand();
    IPAddress ip, gw(0);
    int port = 0;
    for(int k=0;k<100000;k++) {
        ip =get_ip_for_lookup();  
        port = _l->lookup_route(ip, gw);
    }
    _task.fast_reschedule();
    return true;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ReadRadixIPLookupM107)
