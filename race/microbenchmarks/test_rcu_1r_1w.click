Idle -> rt :: RadixIPLookup106(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup106(rt);
updater0::PoundRadixIPLookup106(rt);

StaticThreadSched(
reader0 0,
updater0 1,
);
DriverManager(stop);
