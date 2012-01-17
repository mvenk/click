Idle -> rt :: RadixIPLookup107(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
updater0::PoundRadixIPLookup107(rt);
updater1::PoundRadixIPLookup107(rt);
updater2::PoundRadixIPLookup107(rt);
updater3::PoundRadixIPLookup107(rt);
updater4::PoundRadixIPLookup107(rt);
updater5::PoundRadixIPLookup107(rt);
updater6::PoundRadixIPLookup107(rt);
updater7::PoundRadixIPLookup107(rt);

StaticThreadSched(
updater0 0,
updater1 1,
updater2 2,
updater3 3,
updater4 4,
updater5 5,
updater6 6,
updater7 7,
);
DriverManager(stop);
