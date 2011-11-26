// 2 readers and 1 writer - each of them using the RCU( with fine grained locking) version
// of RadixIPLookup.
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup106(r);
reader1:: ReadRadixIPLookup106(r);
writer :: PoundRadixIPLookup106(r);

StaticThreadSched(
	reader 0,
	reader1 1,
	writer  2);

DriverManager(stop);
