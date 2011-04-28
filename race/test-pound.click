Idle
 -> r :: RadixIPLookup
 -> Idle;

p :: PoundRadixIPLookup(r);

DriverManager(wait 0.1, print r.table, stop);
