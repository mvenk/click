// 3 updaters, 1 readers - each using a readwrite lock version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup107(r);
writer :: PoundRadixIPLookup107(r);
writer2 :: PoundRadixIPLookup107(r);
writer3 :: PoundRadixIPLookup107(r);

StaticThreadSched(
	reader 0,
	writer 1,
	writer2 2,
	writer3 3
);

DriverManager(stop);
