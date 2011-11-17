// 3 readers which use the original clean RadixIPLookup
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader0 :: ReadRadixIPLookup107(r);
reader1 :: ReadRadixIPLookup107(r);
reader2 :: ReadRadixIPLookup107(r);
StaticThreadSched(
	reader0 0,
	reader1 1,
	reader2 2,
);

DriverManager(stop);
