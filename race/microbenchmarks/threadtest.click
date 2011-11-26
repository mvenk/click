//ControlSocket(TCP, 4444);
rt :: RadixIPLookup104(
217.170.115.0/24 193.0.0.56 1,
216.255.39.0/24 4.68.0.243 1,
216.221.5.0/24 4.68.0.243 0,
212.85.129.0/24 134.222.85.45 1,
210.51.225.0/24 193.251.245.6 1,
209.136.89.0/24 12.0.1.63 1,
209.34.243.0/24 12.0.1.63 1,
205.204.1.0/24 4.68.0.243 1,
204.255.51.0/24 144.228.241.81 1,
204.238.34.0/24 216.140.2.59 1,
204.221.17.0/24 12.0.1.63 1,
204.17.221.0/24 216.140.8.59 1,
203.34.233.0/24  1,
);

FromIPSummaryDump("threadtest.dump",STOP true) -> rt;
//Idle() -> rt;
rt[1] ->ToIPSummaryDump("out.dump",VERBOSE true,CONTENTS ip_dst);
rt[0] ->ToIPSummaryDump("out2.dump",VERBOSE true,CONTENTS ip_dst);
//rt[1]->Discard();

m1::Script(TYPE ACTIVE,
write rt.remove 203.34.233.0/24 1,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,);

m2::Script(TYPE ACTIVE,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
write rt.add 203.34.233.0/24 0,
write rt.remove 203.34.233.0/24 0,
);

StaticThreadSched(rt 2, m1 1,m2 0); 
