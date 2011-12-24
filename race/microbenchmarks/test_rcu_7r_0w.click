Idle -> rt :: RadixIPLookup106(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup106(rt);
reader1::ReadRadixIPLookup106(rt);
reader2::ReadRadixIPLookup106(rt);
reader3::ReadRadixIPLookup106(rt);
reader4::ReadRadixIPLookup106(rt);
reader5::ReadRadixIPLookup106(rt);
reader6::ReadRadixIPLookup106(rt);

StaticThreadSched(
reader0 0,
reader1 1,
reader2 2,
reader3 3,
reader4 4,
reader5 5,
reader6 6,
);
DriverManager(stop);
