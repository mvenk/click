// 2 writers, 2 readers -each using a readwrite lock version of RadixIPLookup.
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup107(r);
reader2:: ReadRadixIPLookup107(r);
writer :: PoundRadixIPLookup107(r);
writer2 :: PoundRadixIPLookup107(r);

StaticThreadSched(
	reader 0,
	writer 1,
	reader2 2,
	writer2 3
);

DriverManager(stop);
