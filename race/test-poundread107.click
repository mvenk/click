Idle
 -> r :: RadixIPLookup107
 -> Idle;

p :: PoundRadixIPLookup107(r);
b :: BashRadixIPLookup107(r);
reader :: ReadRadixIPLookup107(r);

StaticThreadSched(
	p 0,
	b 1,
	reader 2
);

DriverManager(wait 3, print r.table, stop);
