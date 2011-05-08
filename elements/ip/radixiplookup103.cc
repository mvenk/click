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
#include "radixiplookup103.hh"
CLICK_DECLS

class RadixIPLookup103::Radix { public:

    static Radix *make_radix(int bitshift, int n);
    static void free_radix(Radix *r);

    int change(uint32_t addr, uint32_t mask, int key, bool set);

    static inline int lookup(const Radix *r, int cur, uint32_t addr, int &depth) {	
	while (r) {
	    int i1 = (addr >> r->_bitshift) & (r->_n - 1);
	    const Child &c = r->_children[i1];
	    if (c.key)
		cur = c.key;
	    r = c.child;
	    depth++;
	}
	return cur;
    }

  private:

    int _bitshift;
    int _n;
    int _nchildren;
    struct Child {
	int key;
	Radix *child;
    } _children[0];

    Radix()			{ }
    ~Radix()			{ }

    int &key_for(int i) {
	assert(i >= 2 && i < _n * 2);
	if (i >= _n)
	    return _children[i - _n].key;
	else {
	    int *x = reinterpret_cast<int *>(_children + _n);
	    return x[i - 2];
	}
    }

    friend class RadixIPLookup103;

};

RadixIPLookup103::Radix*
RadixIPLookup103::Radix::make_radix(int bitshift, int n)
{
    if (Radix* r = (Radix*) new unsigned char[sizeof(Radix) + n * sizeof(Child) + (n - 2) * sizeof(int)]) {
	r->_bitshift = bitshift;
	r->_n = n;
	r->_nchildren = 0;
	memset(r->_children, 0, n * sizeof(Child) + (n - 2) * sizeof(int));
	return r;
    } else
	return 0;
}

void
RadixIPLookup103::Radix::free_radix(Radix* r)
{
    if (r->_nchildren)
	for (int i = 0; i < r->_n; i++)
	    if (r->_children[i].child)
		free_radix(r->_children[i].child);
    delete[] (unsigned char *)r;
}

int
RadixIPLookup103::Radix::change(uint32_t addr, uint32_t mask, int key, bool set)
{
    int i1 = (addr >> _bitshift) & (_n - 1);

    // check if change only affects children
    if (mask & ((1U << _bitshift) - 1)) {
	if (!_children[i1].child
	    && (_children[i1].child = make_radix(_bitshift - 4, 16)))
	    ++_nchildren;
	if (_children[i1].child)
	    return _children[i1].child->change(addr, mask, key, set);
	else
	    return 0;
    }

    // find current key
    i1 = _n + i1;
    int nmasked = _n - ((mask >> _bitshift) & (_n - 1));
    for (int x = nmasked; x > 1; x /= 2)
	i1 /= 2;
    int replace_key = key_for(i1), prev_key = replace_key;
    if (prev_key && i1 > 3 && key_for(i1 / 2) == prev_key)
	prev_key = 0;

    // replace previous key with current key, if appropriate
    if (!key && i1 > 3)
	key = key_for(i1 / 2);
    if (prev_key != key && (!prev_key || set)) {
	for (nmasked = 1; i1 < _n * 2; i1 *= 2, nmasked *= 2)
	    for (int x = i1; x < i1 + nmasked; ++x)
		if (key_for(x) == replace_key)
		    key_for(x) = key;
    }
    return prev_key;
}


RadixIPLookup103::RadixIPLookup103()
    : _default_key(0), _radix(Radix::make_radix(24, 256))
{
    for(int i=0; i < MAX_DEPTH; i++) 
	_vfree[i] = -1;
}

RadixIPLookup103::~RadixIPLookup103()
{
}

void
RadixIPLookup103::cleanup(CleanupStage)
{
    for(int i=0; i < MAX_DEPTH; i++)
	_v[i].clear();
    Radix::free_radix(_radix);
    _radix = 0;
}


String
RadixIPLookup103::dump_routes()
{
    StringAccum sa;
    for(int i=0; i < MAX_DEPTH; i++) 
	for (int j = _vfree[i]; j >= 0; j = _v[i][j].extra)
	    _v[i][j].kill();
    for(int i=0; i < MAX_DEPTH; i++) 
	for (int j = 0; j < _v[i].size(); j++)
	    if (_v[i][j].real())
		_v[i][j].unparse(sa, true) << '\n';
    return sa.take_string();
}


int
RadixIPLookup103::add_route(const IPRoute &route, bool set, IPRoute *old_route, ErrorHandler *)
{
    int last_key;
    // we optimistically push back the route onto the vector and don't
    // do any free list management
    int depth = 0;
    if(route.mask)
	depth = get_depth(ntohl(route.mask.addr()));

    int found = insert_into_v(route, depth);

    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	_rlock.acquire();
	last_key = _radix->change(addr, mask, found + 1, set);
	_rlock.release();
    } else {
	last_key = _default_key;
	if (!last_key || set)
	    _default_key = found + 1;
    }

    if (last_key) { 
	if(old_route) {	    	
	    // TODO: locking on read, should not be required
	    // once a lock free vector is implemented.
	    _vlock[depth].acquire();
	    *old_route = _v[depth][last_key - 1];
	    _vlock[depth].release();
	}
	if(set)
	    remove_from_v(last_key, depth);
	else {
	    remove_from_v(found, depth);
	    return -EEXIST;
	}
    }
    return 0;
}

// returns the depth where the route with the given mask
// will be inserted in the radix tree. Depends on the branching
// factor at each level.
uint32_t
RadixIPLookup103::get_depth(uint32_t mask) 
{
    // branching factor at level 0 and subsequent levels.
    const int bf = 8, bfn = 4;
    int nset = 0;
    for(int i=0; i < 32;i++) {
	if(mask & (1 << (i)))
	    nset++;
    }
    if(nset < bf) 
	return 0;
    else 
	return ((nset - bf)/bfn + ((nset-bf)%bfn > 0));
	    
}

// helper function.
// returns the the index into the _v table created
int 
RadixIPLookup103::insert_into_v(const IPRoute &route, uint32_t depth)
{

    Vector<IPRoute> &v = _v[depth];
    int &vfree = _vfree[depth];
    Spinlock &vlock = _vlock[depth];

    vlock.acquire();
    int found = (vfree < 0 ? v.size() : vfree);
    if (found == v.size())
	v.push_back(route);
    else {
	vfree = v[found].extra;
	v[found] = route;
    }
    v[found].extra = -1;
    vlock.release();

    return found;
}

void 
RadixIPLookup103::remove_from_v(int key, uint32_t depth)
{
    _vlock[depth].acquire();
    _v[depth][key - 1].extra = _vfree[depth];
    _vfree[depth] = key - 1;
    _vlock[depth].release();    
}

int
RadixIPLookup103::remove_route(const IPRoute& route, IPRoute* old_route, ErrorHandler*)
{
    int last_key;
    int depth = 0;
    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	depth = get_depth(mask);
	// NB: this will never actually make changes
	_rlock.acquire();
	last_key = _radix->change(addr, mask, 0, false);
	_rlock.release();
    } else
	last_key = _default_key;
    

    Vector<IPRoute> &v = _v[depth];
    Spinlock &vlock = _vlock[depth];

    if(last_key) {
	// TODO: locking on read, 
        IPRoute r;
	vlock.acquire();
	r = v[last_key - 1];
	vlock.release();

	if(old_route)
	    *old_route = r;

	if(!route.match(r))
	    return -ENOENT;
    }

    if (!last_key)
	return -ENOENT;

    remove_from_v(last_key, depth);

    if (route.mask) {
	uint32_t addr = ntohl(route.addr.addr());
	uint32_t mask = ntohl(route.mask.addr());
	_rlock.acquire();
	(void) _radix->change(addr, mask, 0, true);
	_rlock.release();
    } else
	_default_key = 0;
    return 0;
}

int
RadixIPLookup103::lookup_route(IPAddress addr, IPAddress &gw) const
{
    int depth = -1;
    int key = Radix::lookup(_radix, _default_key, ntohl(addr.addr()), depth);
    if(key == _default_key) 
	depth = 0;
    if (key) {
	gw = _v[depth][key - 1].gw;
	return _v[depth][key - 1].port;
    } else {
	gw = 0;
	return -1;
    }
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(IPRouteTable)
EXPORT_ELEMENT(RadixIPLookup103)
