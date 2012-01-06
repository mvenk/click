#!/usr/sbin/dtrace -s 

#pragma D option quiet 

BEGIN 
{ 
  start = timestamp; 
} 

pid$target:click:__ZNK16RadixIPLookup10712lookup_routeE9IPAddressRS0_:entry,
pid$target:click:__ZN16RadixIPLookup1079add_routeERK7IPRoutebPS0_P12ErrorHandler:entry,
pid$target:click:__ZN12RouterThread10lock_tasksEv:entry,
pid$target:click:__ZN6Master12check_driverEv:entry
{ 
  @add[probefunc] = count(); 
  self->in = vtimestamp; 
} 

pid$target:click:__ZNK16RadixIPLookup10712lookup_routeE9IPAddressRS0_:return,
pid$target:click:__ZN16RadixIPLookup1079add_routeERK7IPRoutebPS0_P12ErrorHandler:return,
pid$target:click:__ZN12RouterThread10lock_tasksEv:return,
pid$target:click:__ZN6Master12check_driverEv:return
{ 
  @sum[probefunc] = sum(vtimestamp - self->in); 
} 

END 
{ 
  totaltime = (timestamp - start);
  printf( "\n\nCount by operation type :" ); 
  printa(@add);
  printf("\n Ran for %u ns",totaltime); 
  printf( "\nTotal time by operation type :" ); 
  printa(@sum); 
}