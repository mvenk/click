Idle -> rt :: RadixIPLookup(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup(rt);
reader1::ReadRadixIPLookup(rt);
reader2::ReadRadixIPLookup(rt);
reader3::ReadRadixIPLookup(rt);
reader4::ReadRadixIPLookup(rt);
reader5::ReadRadixIPLookup(rt);
reader6::ReadRadixIPLookup(rt);

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
