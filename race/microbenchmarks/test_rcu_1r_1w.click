// 1 reader and 1 writer each of them using the RCU (fine grained version of RadixIPLookup).
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup106(r);
writer :: PoundRadixIPLookup106(r);

StaticThreadSched(
	reader 0,
	writer 1,
);

DriverManager(stop);
