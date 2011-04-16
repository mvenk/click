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
#include <click/allocchunk.hh>
#include "radixiplookup18.hh"
#include <stdio.h>

CLICK_DECLS

class RadixIPLookup18::Radix { public:

	static Radix *make_radix(int level, HashMap<Radix*,int*>& super_map);
    static void free_radix(Radix *r, int level);

    int change(uint32_t addr, uint32_t mask, int key, bool set, int level, HashMap<Radix*,int*>& super_map);

    static inline int lookup(const Radix *r, int cur, uint32_t addr, int level) {
	while (r) {
	    int i1 = (addr >> bitshift(level)) & (nbuckets(level) - 1);
	    const Child &c = r->_children[i1];
	    if (c.key)
		cur = c.key;
	    r = c.child;
	    level++;
	}
	return cur;
    }

  private:
    //int * _superchildren;
    struct Child {
	int key;
	Radix *child;
    } _children[0];

    Radix()			{ }
    ~Radix()			{ }

    int &key_for(int i, int level,HashMap<Radix*,int*>& super_map) {
	int n = nbuckets(level);
	assert(i >= 2 && i < n * 2);
	if (i >= n)
	    return _children[i - n].key;
	else {
	    //  int *x = reinterpret_cast<int *>(_children + _n);
	    int *x= super_map[this];
	    return x[i - 2];
	}
    }

    static inline int bitshift1(void) {
	return (32 - lglvl1);
    }
    
    static inline int bitshiftn(int i) {
	return (bitshift1() - (i -1) * lglvln);
    }

    static inline int bitshift(int i) {
	return ((i == 1) ? (bitshift1()) : bitshiftn(i));
    }

    static inline int nbuckets(int i) {
	return (( i == 1) ? (1 << lglvl1) : (1 << lglvln));
    }
	

    friend class RadixIPLookup18;

};

RadixIPLookup18::Radix*
RadixIPLookup18::Radix::make_radix(int level, HashMap<Radix*,int*> & super_map)
{
    int n = nbuckets(level);
    int level1_size = sizeof(Radix) + n * sizeof(Child);
    Radix* r;
    // allocchunk currently does not allow for variable sized chunks.
    // so the allocchunk is not used for the first level,
    // it is used for all subsequent levels since they are of the same size.
    if(level == 1) {
	r = (Radix*)new unsigned char[level1_size];
    }
    else {
	r = (Radix*)AllocChunk::Instance()->alloc();
	//r = (Radix*)new unsigned char[level1_size];
	assert(r);
    }
    
    if(r) {
	    // We have only allotted a pointer to the array of
	    // superchildren. Now we allot space for all the 
	    // superchildren
	    memset(r, 0, level1_size);
	    int *superchildren = (int *)new unsigned char[(n - 2) * sizeof(int)];
	    assert(superchildren);
	    memset(r->_children, 0, n * sizeof(Child));
	    memset(superchildren,0,(n - 2) * sizeof(int));
	    super_map.insert(r,superchildren);
	    return r;
    } 
    else {
	return 0;
    }

}

void
RadixIPLookup18::Radix::free_radix(Radix* r, int level)
{
    // Nothing to be done
}

int
RadixIPLookup18::Radix::change(uint32_t addr, uint32_t mask, int key, bool set, int level, HashMap<Radix*,int*>& super_map )
{
    int shift = bitshift(level);
    int n = nbuckets(level);
    int i1 = (addr >> shift) & (n - 1);

    // check if change only affects children
    if (mask & ((1U << shift) - 1)) {
	if (!_children[i1].child
	    && (_children[i1].child = make_radix(level + 1,super_map))) {
	    ;
	}
	if (_children[i1].child)
	    return _children[i1].child->change(addr, mask, key, set, level+1,super_map);
	else
	    return 0;
    }

    // find current key
    i1 = n + i1;
    int nmasked = n - ((mask >> shift) & (n - 1));
    for (int x = nmasked; x > 1; x /= 2)
	i1 /= 2;
    int replace_key = key_for(i1, level,super_map), prev_key = replace_key;
    if (prev_key && i1 > 3 && key_for(i1 / 2, level,super_map) == prev_key)
	prev_key = 0;

    // replace previous key with current key, if appropriate
    if (!key && i1 > 3)
	key = key_for(i1 / 2, level,super_map);
    if (prev_key != key && (!prev_key || set)) {
	for (nmasked = 1; i1 < n * 2; i1 *= 2, nmasked *= 2)
	    for (int x = i1; x < i1 + nmasked; ++x)
		if (key_for(x, level,super_map) == replace_key)
		    key_for(x, level,super_map) = key;
    }
    return prev_key;
}


RadixIPLookup18::RadixIPLookup18()
    : _vfree(-1), _default_key(0), _radix(Radix::make_radix(1,_map))
{
    // the number of children = 2^braching factor which is defined by lglvln
    int num_children = 1<<lglvln;
    int leveln_size = sizeof(int*) + num_children * (sizeof(RadixIPLookup18::Radix::Child)); 
    AllocChunk::Instance()->create(leveln_size,4096);
}

RadixIPLookup18::~RadixIPLookup18()
{

}


void
RadixIPLookup18::cleanup(CleanupStage)
{
    int level = 1;
    _v.clear();
    Radix::free_radix(_radix, level);
    AllocChunk::Instance()->free_all();
    delete[] (unsigned char*)_radix;
    _radix = 0;
}


String
RadixIPLookup18::dump_routes()
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
RadixIPLookup18::add_route(const IPRoute &route, bool set, IPRoute *old_route, ErrorHandler *)
{
    int found = (_vfree < 0 ? _v.size() : _vfree), last_key;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 1;
	last_key = _radix->change(addr, mask, found + 1, set, level, _map);
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
RadixIPLookup18::remove_route(const IPRoute& route, IPRoute* old_route, ErrorHandler*)
{
    int last_key;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	int level = 1;
	// NB: this will never actually make changes
	last_key = _radix->change(addr, mask, 0, false, level,_map);
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
	int level = 1;
	(void) _radix->change(addr, mask, 0, true, level,_map);
    } else
	_default_key = 0;
    return 0;
}

int
RadixIPLookup18::lookup_route(IPAddress addr, IPAddress &gw) const
{
    int level = 1;
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
EXPORT_ELEMENT(RadixIPLookup18)
