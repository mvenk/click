// -*- c-basic-offset: 4 -*-
#ifndef CLICK_RADIXIPLOOKUP19_HH
#define CLICK_RADIXIPLOOKUP19_HH
#include <click/glue.hh>
#include <click/element.hh>
#include "iproutetable.hh"
CLICK_DECLS

/*
=c

RadixIPLookup19(ADDR1/MASK1 [GW1] OUT1, ADDR2/MASK2 [GW2] OUT2, ...)

=s iproute

IP lookup using a radix trie

=d

Performs IP lookup using a radix trie.  The first level of the trie has 256
buckets; each succeeding level has 16.  The maximum number of levels that will
be traversed is thus 7.

Expects a destination IP address annotation with each packet. Looks up that
address in its routing table, using longest-prefix-match, sets the destination
annotation to the corresponding GW (if specified), and emits the packet on the
indicated OUTput port.

Each argument is a route, specifying a destination and mask, an optional
gateway IP address, and an output port.

Uses the IPRouteTable interface; see IPRouteTable for description.

=h table read-only

Outputs a human-readable version of the current routing table.

=h lookup read-only

Reports the OUTput port and GW corresponding to an address.

=h add write-only

Adds a route to the table. Format should be `C<ADDR/MASK [GW] OUT>'. Should
fail if a route for C<ADDR/MASK> already exists, but currently does not.

=h set write-only

Sets a route, whether or not a route for the same prefix already exists.

=h remove write-only

Removes a route from the table. Format should be `C<ADDR/MASK>'.

=h ctrl write-only

Adds or removes a group of routes. Write `C<add>/C<set ADDR/MASK [GW] OUT>' to
add a route, and `C<remove ADDR/MASK>' to remove a route. You can supply
multiple commands, one per line; all commands are executed as one atomic
operation.

=n

See IPRouteTable for a performance comparison of the various IP routing
elements.

=a IPRouteTable, DirectIPLookup, RangeIPLookup, StaticIPLookup,
LinearIPLookup, SortedIPLookup, LinuxIPLookup
*/

struct GWPort {
    IPAddress gw;
    int32_t port;
};


class RadixIPLookup19 : public IPRouteTable { public:

    RadixIPLookup19();
    ~RadixIPLookup19();

    const char *class_name() const		{ return "RadixIPLookup19"; }
    const char *port_count() const		{ return "1/-"; }
    const char *processing() const		{ return PUSH; }


    void cleanup(CleanupStage);

    int add_route(const IPRoute&, bool, IPRoute*, ErrorHandler *);
    int remove_route(const IPRoute&, IPRoute*, ErrorHandler *);
    int lookup_route(IPAddress, IPAddress&) const;
    int find_lookup_key(IPAddress gw, int port);
    String dump_routes();

  private:

    class Radix;

    // Simple routing table
    Vector<IPRoute> _v;
    int _vfree;
        
    Vector<GWPort> _lookup;

    int _default_key;
    Radix *_radix;

};


CLICK_ENDDECLS
#endif
