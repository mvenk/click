Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup106(r);
reader1 :: ReadRadixIPLookup106(r);
reader2 :: ReadRadixIPLookup106(r);
StaticThreadSched(
	reader 0,
	reader1 1,
	reader2 2
);

DriverManager(stop);
