Idle
 -> r :: RadixIPLookup104
 -> Idle;

p :: PoundRadixIPLookup(r);
b :: BashRadixIPLookup(r);

StaticThreadSched(
	p 0,
	b 1,
);

DriverManager(wait 3, 
		   print r.table,
stop);
