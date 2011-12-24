Idle -> rt :: RadixIPLookup107(
                1.1.1.0/32  8.1.1.1 0,
                 0.0.0.0/0   8.1.1.1 0,
                 ) 
  -> Idle;
reader0::ReadRadixIPLookup107(rt);
reader1::ReadRadixIPLookup107(rt);
reader2::ReadRadixIPLookup107(rt);
reader3::ReadRadixIPLookup107(rt);
reader4::ReadRadixIPLookup107(rt);
reader5::ReadRadixIPLookup107(rt);
reader6::ReadRadixIPLookup107(rt);

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
