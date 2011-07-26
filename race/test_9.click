// 3 updaters, 1 reader each using the RCU (fine grained lock) version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup106(r);
writer:: PoundRadixIPLookup106(r);
writer2 :: PoundRadixIPLookup106(r);
writer3 :: PoundRadixIPLookup106(r);

StaticThreadSched(
	reader 0,
	writer 1,
	writer2 2,
	writer3 3
);

DriverManager(stop);
