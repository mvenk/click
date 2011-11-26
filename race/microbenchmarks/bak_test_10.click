// 4 writers each using a readwrite lock version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup106(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;


writer :: PoundRadixIPLookup106(r);
writer2 :: PoundRadixIPLookup106(r);
writer3 :: PoundRadixIPLookup106(r);
writer4 :: PoundRadixIPLookup106(r);

StaticThreadSched(
	writer 1,
	writer2 2,
	writer3 3,
	writer4 0
);

DriverManager(stop);
