Idle
 -> r :: RadixIPLookup106
 -> Idle;

p :: PoundRadixIPLookup106(r);
b :: BashRadixIPLookup106(r);
p2 :: PoundRadixIPLookup106(r);
reader :: ReadRadixIPLookup106(r);

StaticThreadSched(
	p 0,
	b 1,
	reader 2,
	p2 3,
);

DriverManager(stop);
