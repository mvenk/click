Idle
 -> r :: RadixIPLookup23
 -> Idle;

p0 :: PoundRadixIPLookup(r);
p1 :: PoundRadixIPLookup(r);
DriverManager(wait 5, print r.table, stop);
StaticThreadSched(p0 0,p1 1);