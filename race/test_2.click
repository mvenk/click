// 2 readers - the original RadixIPLookup with no locks
Idle
 -> r :: RadixIPLookup(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup(r);
reader1 :: ReadRadixIPLookup(r);
StaticThreadSched(
	reader 0,
	reader1 1
);

DriverManager(stop);
