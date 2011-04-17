// -*- c-basic-offset: 4 -*-
/*
 * radixiplookup.{cc,hh} -- looks up next-hop address in radix table
 * Eddie Kohler (earlier versions: Thomer M. Gil, Benjie Chen)
 *
 * Copyright (c) 1999-2001 Massachusetts Institute of Technology
 * Copyright (c) 2002 International Computer Science Institute
 * Copyright (c) 2005 Regents of the University of California
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, subject to the conditions listed in the Click LICENSE
 * file. These conditions include: you must preserve this copyright
 * notice, and you cannot mention the copyright holders in advertising
 * related to the Software without their permission.  The Software is
 * provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This notice is a
 * summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include <click/ipaddress.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include "radixiplookup19.hh"
#include <stdio.h>
CLICK_DECLS

class RadixIPLookup19::Radix { public:

    static Radix *make_radix(int level);
    static void free_radix(Radix *r, int level);

    int change(uint32_t addr, uint32_t mask, int key, int lookup_key, bool set, int level);

    static inline int lookup(const Radix *r, int cur, uint32_t addr, int level) {
	while (r) {
	    int i1 = (addr >> _bitshift[level]) & (_nbuckets[level] - 1);
	    const Child &c = r->_children[i1];
	    if (c.key)
		cur = c.key;
	    r = c.child;
	    level++;
	}
	return cur;
    }

  private:

    struct Child {
	int key;
	Radix *child;
    } _children[0];

    Radix()			{ }
    ~Radix()			{ }

    int &key_for(int i, int level) {
	int n = _nbuckets[level];
	assert(i >= 2 && i < n * 2);
	if (i >= n)
	    return _children[i - n].key;
	else {
	    int *x = reinterpret_cast<int *>(_children + n);
	    return x[i - 2];
	}
    }
       
    static const int _bitshift [5];
    static const int _nbuckets [5];

    friend class RadixIPLookup19;

};

const int RadixIPLookup19::Radix::_bitshift [5] = {20, 15, 10, 5, 0};
const int RadixIPLookup19::Radix::_nbuckets [5] = {4096, 32, 32, 32, 32};

int
RadixIPLookup19::find_lookup_key(IPAddress gw, int32_t port) {
    for(int i=0; i  < _lookup.size(); i++) {
	if(_lookup[i].gw == gw  &&
	   _lookup[i].port == port) 
	    return (i + 1);
    }
    return 0;
}

RadixIPLookup19::Radix*
RadixIPLookup19::Radix::make_radix(int level)
{
    int n = _nbuckets[level];
    if (Radix* r = (Radix*) new unsigned char[sizeof(Radix) + n * sizeof(Child) + (n - 2) * sizeof(int)]) {
	memset(r->_children, 0, n * sizeof(Child) + (n - 2) * sizeof(int));
	return r;
    } else
	return 0;
}

void
RadixIPLookup19::Radix::free_radix(Radix* r, int level)
{
    int n = _nbuckets[level];
    for (int i = 0; i < n; i++)
	if (r->_children[i].child)
	    free_radix(r->_children[i].child, level+1);
    delete[] (unsigned char *)r;
}

int
RadixIPLookup19::Radix::change(uint32_t addr, uint32_t mask, int key, int lookup_key, bool set, int level)
{
    int shift = _bitshift[level];
    int n = _nbuckets[level];
    int i1 = (addr >> shift) & (n - 1);

    // check if change only affects children
    if (mask & ((1U << shift) - 1)) {
	if (!_children[i1].child
	    && (_children[i1].child = make_radix(level + 1))) {
	    ;
	}
	if (_children[i1].child)
	    return _children[i1].child->change(addr, mask, key, lookup_key, set, level+1);
	else
	    return 0;
    }

    // find current key
    i1 = n + i1;
    int nmasked = n - ((mask >> shift) & (n - 1));
    for (int x = nmasked; x > 1; x /= 2)
	i1 /= 2;
    int replace_key = key_for(i1, level), prev_key = replace_key;
    if (prev_key && i1 > 3 && key_for(i1 / 2, level) == prev_key)
	prev_key = 0;

    // replace previous key with current key, if appropriate
    if (!key && i1 > 3)
	key = key_for(i1 / 2, level);
    if (prev_key != key && (!prev_key || set)) {
	for (nmasked = 1; i1 < n * 2; i1 *= 2, nmasked *= 2)
	    for (int x = i1; x < i1 + nmasked; ++x)
		if (key_for(x, level) == replace_key)
		    key_for(x, level) = key;
    }
    return prev_key;
}


RadixIPLookup19::RadixIPLookup19()
    : _vfree(-1), _default_key(0), _radix(Radix::make_radix(0))
{
}

RadixIPLookup19::~RadixIPLookup19()
{
}


void
RadixIPLookup19::cleanup(CleanupStage)
{
    int level = 0;
    _v.clear();
    Radix::free_radix(_radix, level);
    _radix = 0;
}


String
RadixIPLookup19::dump_routes()
{
    StringAccum sa;
    for (int j = _vfree; j >= 0; j = _v[j].extra)
	_v[j].kill();
    for (int i = 0; i < _v.size(); i++)
	if (_v[i].real())
	    _v[i].unparse(sa, true) << '\n';
    return sa.take_string();
}


int
RadixIPLookup19::add_route(const IPRoute &route, bool set, IPRoute *old_route, ErrorHandler *)
{
    int found = (_vfree < 0 ? _v.size() : _vfree), last_key;
    int lookup_key = find_lookup_key(route.gw, route.port);
    if(!lookup_key) 
	lookup_key = _lookup.size() + 1;
		    
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 0;
	last_key = _radix->change(addr, mask, found + 1, lookup_key, set, level);
    } else {
	last_key = _default_key;
	if (!last_key || set)
	    _default_key = found + 1;
    }

    if (last_key && old_route)
	*old_route = _v[last_key - 1];
    if (last_key && !set)
	return -EEXIST;

    if (lookup_key == (_lookup.size() + 1)) {
	GWPort gw_port = {route.gw, route.port};
	_lookup.push_back(gw_port);
    }

    if (found == _v.size())
	_v.push_back(route);
    else {
	_vfree = _v[found].extra;
	_v[found] = route;
    }
    _v[found].extra = -1;

    if (last_key) {
	_v[last_key - 1].extra = _vfree;
	_vfree = last_key - 1;
    }

    return 0;
}

int
RadixIPLookup19::remove_route(const IPRoute& route, IPRoute* old_route, ErrorHandler*)
{
    int last_key;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 0;
	// NB: this will never actually make changes
	last_key = _radix->change(addr, mask, 0, 0, false, level);
    } else
	last_key = _default_key;

    if (last_key && old_route)
	*old_route = _v[last_key - 1];
    if (!last_key || !route.match(_v[last_key - 1]))
	return -ENOENT;
    _v[last_key - 1].extra = _vfree;
    _vfree = last_key - 1;

    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 0;
	(void) _radix->change(addr, mask, 0, 0, true, level);
    } else
	_default_key = 0;
    return 0;
}

int
RadixIPLookup19::lookup_route(IPAddress addr, IPAddress &gw) const
{
    int level = 0;
    int key = Radix::lookup(_radix, _default_key, ntohl(addr.addr()), level);
    if (key) {
	gw = _v[key - 1].gw;
	return _v[key - 1].port;
    } else {
	gw = 0;
	return -1;
    }
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(IPRouteTable)
EXPORT_ELEMENT(RadixIPLookup19)
