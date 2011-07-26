// 1 reader - the RCU version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;
reader :: ReadRadixIPLookup106(r);
StaticThreadSched(
	reader 0
);
DriverManager(stop);