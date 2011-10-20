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
#include "radixiplookup30.hh"
#include <stdio.h>
CLICK_DECLS

class RadixIPLookup30::Radix { public:

    static Radix *make_radix(int level);
    static void free_radix(Radix *r, int level);

    int change(uint32_t addr, uint32_t mask, int key, bool set, int level);

    static inline int lookup(const Radix *r, int cur, uint32_t addr, int level) {
	while (r) {
	    int i1 = (addr >> _bitshift[level]) & (_nbuckets[level] - 1);
	    int key = r->_keys[i1].key;
	    if (key)
		cur = key;
	    if(r->_keys[i1].has_child) {
		r = r->_children[i1];
		level++;
	    }
	    else {
		break;
	    }
	}
	return cur;
    }

  private:
        
    struct Key {
	int key:31;
	int has_child:1;
    };
    
    Radix **_children;
    Key *_keys;
    
    Radix()			{ }
    ~Radix()			{ }

    int key_for(int i, int level) {
	int n = _nbuckets[level];
	assert(i >= 2 && i < n * 2);
	if(i >= n) 
	    return _keys[i - n].key;
	else {
	    return _keys[n + i - 2].key;
	}
    }


    Key *get_key(int i, int level) {
	int n = _nbuckets[level];
	assert(i >= 2 && i < n * 2);
	if(i >= n) 
	    return &_keys[i - n];
	else {
	    return &_keys[n + i - 2];
	}
    }
       
    static const int _bitshift [5];
    static const int _nbuckets [5];

    friend class RadixIPLookup30;

};



const int RadixIPLookup30::Radix::_bitshift [5] = {20, 15, 10, 5, 0};
const int RadixIPLookup30::Radix::_nbuckets [5] = {4096, 32, 32, 32, 32};

RadixIPLookup30::Radix*
RadixIPLookup30::Radix::make_radix(int level) 
{
    int n = _nbuckets[level];
    if(Radix* r = new Radix()) {
	unsigned char * block = new unsigned char[n * sizeof(Radix *) + n * sizeof(Key) + (n-2) * sizeof(Key)];
	if(!block) 
	    return 0;
	r->_children = (Radix **) block;
	r->_keys = (Radix::Key *) &block[n *sizeof(Radix *)];
	memset(r->_children, 0, n * sizeof(Radix*));
	memset(r->_keys, 0, n * sizeof(Key) + (n - 2) *sizeof(Key));
	return r;
    }
    else 
	return 0;
}


void
RadixIPLookup30::Radix::free_radix(Radix* r, int level) 
{
    int n = _nbuckets[level];
    for(int i=0; i < n; i++) 
	if(r->_children[i])
	    free_radix(r->_children[i], level+1);
    delete[] (unsigned char *) r->_children;
    delete[] (unsigned char *)r;
}

int
RadixIPLookup30::Radix::change(uint32_t addr, uint32_t mask, int key, bool set, int level)
{
    int shift = _bitshift[level];
    int n = _nbuckets[level];
    int i1 = (addr >> shift) & (n - 1);

    // check if change only affects children
    if (mask & ((1U << shift) - 1)) {
	if (!_children[i1]
	    && (_children[i1] = make_radix(level + 1))) {
	    ;
	}
	if (_children[i1]) {
	    _keys[i1].has_child = 1;
	    return _children[i1]->change(addr, mask, key, set, level+1);
	}
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
		    get_key(x, level)->key = key;
    }
    return prev_key;
} 

RadixIPLookup30::RadixIPLookup30()
    : _vfree(-1), _default_key(0), _radix(Radix::make_radix(0))
{
}

RadixIPLookup30::~RadixIPLookup30()
{
}


void
RadixIPLookup30::cleanup(CleanupStage)
{
    int level = 0;
    _v.clear();
    Radix::free_radix(_radix, level);
    _radix = 0;
}


String
RadixIPLookup30::dump_routes()
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
RadixIPLookup30::add_route(const IPRoute &route, bool set, IPRoute *old_route, ErrorHandler *)
{
    int found = (_vfree < 0 ? _v.size() : _vfree), last_key;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 0;
	last_key = _radix->change(addr, mask, found + 1, set, level);
    } else {
	last_key = _default_key;
	if (!last_key || set)
	    _default_key = found + 1;
    }

    if (last_key && old_route)
	*old_route = _v[last_key - 1];
    if (last_key && !set)
	return -EEXIST;

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
RadixIPLookup30::remove_route(const IPRoute& route, IPRoute* old_route, ErrorHandler*)
{
    int last_key;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 0;
	// NB: this will never actually make changes
	last_key = _radix->change(addr, mask, 0, false, level);
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
	(void) _radix->change(addr, mask, 0, true, level);
    } else
	_default_key = 0;
    return 0;
}

int
RadixIPLookup30::lookup_route(IPAddress addr, IPAddress &gw) const
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
EXPORT_ELEMENT(RadixIPLookup30)
