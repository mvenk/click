// 2 readers and 2 writers - each of them using the RCU( with fine grained locking) version
// of RadixIPLookup.
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup106(r);
reader2:: ReadRadixIPLookup106(r);
writer:: PoundRadixIPLookup106(r);
writer2 :: PoundRadixIPLookup106(r);

StaticThreadSched(
	reader 0,
	writer 1,
	reader2 2,
	writer2 3
);

DriverManager(stop);
