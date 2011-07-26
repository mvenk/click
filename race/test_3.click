// 3 readers which use the original clean RadixIPLookup
Idle
 -> r :: RadixIPLookup(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader0 :: ReadRadixIPLookup(r);
reader1 :: ReadRadixIPLookup(r);
reader2 :: ReadRadixIPLookup(r);
StaticThreadSched(
	reader0 0,
	reader1 1,
	reader2 2,
);

DriverManager(stop);
