// 2 readers - the original RadixIPLookup with no locks
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup107(r);
reader1 :: ReadRadixIPLookup107(r);
StaticThreadSched(
	reader 0,
	reader1 1
);

DriverManager(stop);
