// 1 writer - using the read-write lock version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

writer :: PoundRadixIPLookup107(r);

StaticThreadSched(
	writer 1
);

DriverManager(stop);
