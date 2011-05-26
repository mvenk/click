Idle
 -> r :: RadixIPLookup106
 -> Idle;

p :: PoundRadixIPLookup(r);
b :: BashRadixIPLookup(r);
p2 :: PoundRadixIPLookup(r);
reader :: ReadRadix(r);

StaticThreadSched(
	p 0,
	b 1,
	reader 2,
	p2 3,
);

DriverManager(wait 3, 
		   print r.table,
stop);
