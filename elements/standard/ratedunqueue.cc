// -*- c-basic-offset: 4 -*-
/*
 * ratedunqueue.{cc,hh} -- element pulls as many packets as possible from
 * its input, pushes them out its output
 * Eddie Kohler
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2010 Meraki, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "ratedunqueue.hh"
#include "bwratedunqueue.hh"
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/straccum.hh>
#include <click/standard/scheduleinfo.hh>
CLICK_DECLS

RatedUnqueue::RatedUnqueue()
    : _task(this), _timer(&_task), _runs(0), _pushes(0), _failed_pulls(0), _empty_runs(0), _active(true)
{
}

RatedUnqueue::~RatedUnqueue()
{
}

int
RatedUnqueue::configure(Vector<String> &conf, ErrorHandler *errh)
{
    unsigned r;
    CpVaParseCmd cmd = (is_bandwidth() ? cpBandwidth : cpUnsigned);
    if (cp_va_kparse(conf, this, errh,
		     "RATE", cpkP+cpkM, cmd, &r, cpEnd) < 0)
	return -1;
    _rate_raw = r;
    _rate.assign(r, is_bandwidth() ? (r + BandwidthRatedUnqueue::RATE_FILL_MIN) : r);
    return 0;
}

int
RatedUnqueue::initialize(ErrorHandler *errh)
{
    ScheduleInfo::initialize_task(this, &_task, errh);
    _signal = Notifier::upstream_empty_signal(this, 0, &_task);
    _timer.initialize(this);
    return 0;
}

bool
RatedUnqueue::run_task(Task *)
{
    bool worked = false;
    _runs++;
    if (!_active)
	return false;
    _rate.fill();
    if (_rate.contains(1)) {
	if (Packet *p = input(0).pull()) {
	    _rate.remove(1);
	    output(0).push(p);
            _pushes++;
	    worked = true;
	} else { // no Packet available
            _failed_pulls++;
	    if (!_signal)
		return false; // without rescheduling
        }
    } else {
	_timer.schedule_after(Timestamp::make_jiffies(_rate.epochs_until_contains(1)));
	_empty_runs++;
	return false;
    }
    _task.fast_reschedule();
    if (!worked)
        _empty_runs++;
    return worked;
}

String
RatedUnqueue::read_handler(Element *e, void *thunk)
{
    RatedUnqueue *ru = (RatedUnqueue *)e;
    switch ((uintptr_t) thunk) {
      case h_rate:
	if (ru->is_bandwidth())
	    return cp_unparse_bandwidth(ru->_rate_raw);
	else
	    return String(ru->_rate_raw);
      case h_calls: {
	  StringAccum sa;
	  sa << ru->_runs << " calls to run_task()\n"
	     << ru->_empty_runs << " empty runs\n"
	     << ru->_pushes << " pushes\n"
	     << ru->_failed_pulls << " failed pulls\n";
	  return sa.take_string();
      }
    }
    return String();
}

void
RatedUnqueue::add_handlers()
{
    add_read_handler("calls", read_handler, h_calls);
    add_read_handler("rate", read_handler, h_rate);
    add_write_handler("rate", reconfigure_keyword_handler, "0 RATE");
    add_data_handlers("active", Handler::OP_READ | Handler::OP_WRITE | Handler::CHECKBOX, &_active);
    add_task_handlers(&_task);
    add_read_handler("config", read_handler, h_rate);
    set_handler_flags("config", 0, Handler::CALM);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RatedUnqueue)
ELEMENT_MT_SAFE(RatedUnqueue)
