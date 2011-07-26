Idle
 -> r :: RadixIPLookup107(
		1.1.1.0/32  8.1.1.1 0,
		0.0.0.0/0   8.1.1.1 0,
		) 
 -> Idle;

reader :: ReadRadixIPLookup107(r);
writer :: PoundRadixIPLookup107(r);

StaticThreadSched(
	reader 0,
	writer 1,
);

DriverManager(stop);
