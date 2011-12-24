Idle -> rt :: RadixIPLookup107(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup107(rt);
reader1::ReadRadixIPLookup107(rt);
updater0::PoundRadixIPLookup107(rt);

StaticThreadSched(
reader0 0,
reader1 1,
updater0 2,
);
DriverManager(stop);
