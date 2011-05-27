Idle
 -> r :: RadixIPLookup106
 -> Idle;

p :: PoundRadixIPLookup106(r);
b :: BashRadixIPLookup106(r);
reader :: ReadRadixIPLookup106(r);

StaticThreadSched(
	p 0,
	b 1,
	reader 2
);

DriverManager(stop);
