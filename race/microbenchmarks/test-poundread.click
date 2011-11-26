Idle
 -> r :: RadixIPLookup106
 -> Idle;

reader :: ReadRadixIPLookup106(r);

StaticThreadSched(
	reader 0
);

DriverManager(stop);
