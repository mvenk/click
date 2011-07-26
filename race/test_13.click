// 3 updaters, 1 readers
Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

writer :: PoundRadixIPLookup107(r);
writer2 :: PoundRadixIPLookup107(r);
writer3 :: PoundRadixIPLookup107(r);

StaticThreadSched(
	writer 1,
	writer2 2,
	writer3 3
);

DriverManager(stop);
