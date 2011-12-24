Idle -> rt :: RadixIPLookup106(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup106(rt);
updater0::PoundRadixIPLookup106(rt);
updater1::PoundRadixIPLookup106(rt);
updater2::PoundRadixIPLookup106(rt);
updater3::PoundRadixIPLookup106(rt);
updater4::PoundRadixIPLookup106(rt);
updater5::PoundRadixIPLookup106(rt);
updater6::PoundRadixIPLookup106(rt);

StaticThreadSched(
reader0 0,
updater0 1,
updater1 2,
updater2 3,
updater3 4,
updater4 5,
updater5 6,
updater6 7,
);
DriverManager(stop);
