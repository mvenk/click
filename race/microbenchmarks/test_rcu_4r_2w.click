Idle -> rt :: RadixIPLookup106(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup106(rt);
reader1::ReadRadixIPLookup106(rt);
reader2::ReadRadixIPLookup106(rt);
reader3::ReadRadixIPLookup106(rt);
updater0::PoundRadixIPLookup106(rt);
updater1::PoundRadixIPLookup106(rt);

StaticThreadSched(
reader0 0,
reader1 1,
reader2 2,
reader3 3,
updater0 4,
updater1 5,
);
DriverManager(stop);
