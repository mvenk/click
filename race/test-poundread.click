Idle
 -> r :: RadixIPLookup105
 -> Idle;

p :: PoundRadixIPLookup(r);
b :: BashRadixIPLookup(r);
reader :: ReadRadix(r);

StaticThreadSched(
	p 0,
	b 1,
	reader 2,
);

DriverManager(wait 3, 
		   print r.table,
stop);
