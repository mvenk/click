// 2 writers - each using the RCU (fine grained locking version of RadixIPLookup).
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

writer:: PoundRadixIPLookup106(r);
writer2 :: PoundRadixIPLookup106(r);

StaticThreadSched(
	writer 0,
	writer2 1
);

DriverManager(stop);
