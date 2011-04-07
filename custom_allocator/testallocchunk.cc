#include "allocchunk.hh"
#include<iostream>
#include<cstring>
using namespace std;

int TestWriteToAChunk(void*ptr, int size)
{
  const char *str ="the quick brown fox jumped over the lazy dog";
  int strsize = strlen(str);
  char read_byte;
  for(int i =0;i<size;i++)
    memset((char*)ptr+i,str[i%strsize],8);
  for(int i=0;i<size;i++)
    {
      read_byte = * ((char*)ptr+i);
      if(!(read_byte==str[i%strsize]))
	return 0;
    }
  cout<<endl<<"Wrote and read "<<size<<" bytes successfully."<<endl;
  return 1;
}

int TestChunkWrite()
{
  /* allocate 10 chunks of 16 */
  AllocChunk::Instance()->create(16,10);
  // now we have 10 blocks of size 16 bytes
  // get 1 block
  void* ptr= AllocChunk::Instance()->alloc();
  int pass=0;
  if(TestWriteToAChunk(ptr,2)) {
     cout<<" Allocation and write successful";
     pass=1; 
  }
  else
     cout<<" Allocation and write test failed";
  
  AllocChunk::Instance()->free_all();
  return pass;
}

int TestAllocMoreBlocks()
{
  /* allocate 10 chunks of 16 */
  AllocChunk::Instance()->create(256,10);
  
  /* array of 32 pointers */
 void* ptr[32];
 for(int i=0; i<32;i++)
   {
   ptr[i]= AllocChunk::Instance()->alloc();
   if(!TestWriteToAChunk(ptr[i],256/8))
     return 0;
   }
 AllocChunk::Instance()->free_all();
 return 1;
}
int main()
{
  if(!TestChunkWrite()) {
    cout<<"TestChunkWritee failed"<<endl;
    return -1;
  }
  else 
    cout<<"TestChunkWrite passed"<<endl;
  
  if(!TestAllocMoreBlocks())  {
    cout<<"TestMoreBlocks failed"<<endl;
    return -1;
  }
  else
    cout<<"TestMoreBlocks passed"<<endl;
  cout<<"All Tests Passed Successfully."<<endl;
}
