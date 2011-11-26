Idle
 -> r :: RadixIPLookup(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup(r);
reader1 :: ReadRadixIPLookup(r);
reader2 :: ReadRadixIPLookup(r);
reader3 :: ReadRadixIPLookup(r);

StaticThreadSched(
	reader 0,
	reader1 1,
	reader2 2,
	reader3 3
);

DriverManager(stop);
