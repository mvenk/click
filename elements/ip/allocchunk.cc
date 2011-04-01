#include "allocchunk.hh"
#include<iostream>
#include<cstring>
using namespace std;

AllocChunk :: AllocChunk(int size,int multiple)
{
  this->size = size;
  this->multiple = multiple; 
  head = 0; 
  tail = 0;
}

AllocChunk :: ~AllocChunk()
{
}


void AllocChunk :: add_block(void) {

  struct block_list *list;
  list = new block_list();
  if(list) {
    list->node.block = new char[multiple * size];
    list->node.blocks_left = multiple;
    if(tail)
      tail->next = list;
    tail = list;
    if(head == 0)
      head = tail;
  }
}

void * AllocChunk :: alloc(void) {
  void * block = 0;
  if(!tail || !tail->node.blocks_left) {
    add_block();
  }
  block = (void *) tail->node.block;
  tail->node.block += size;
  tail->node.blocks_left--;
  return block;
}

void AllocChunk :: free_all(void) {
  if(head) {
    struct block_list * current = head;
    struct block_list * temp = head->next;
    while(current) {
      // we need to rewind the node's block address as we may have allotted some
      // blocks already
      char * block_ptr = current->node.block - (multiple - current->node.blocks_left)*size;
      delete block_ptr;
      delete current;
      current = temp;
      if(temp)
	temp = temp->next;
    }
    head = tail = 0;
  }
}

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

void TestChunkWrite()
{
  /* allocate 10 chunks of 16 */
  AllocChunk chunk = AllocChunk(16,10);
  // now we have 10 blocks of size 16 bytes
  // get 1 block
  void* ptr=chunk.alloc();
  if(TestWriteToAChunk(ptr,2))
     cout<<" Allocation and write successful";
   else
     cout<<" Allocation and write test failed";
  
  chunk.free_all();
}

int TestAllocMoreBlocks()
{
  /* allocate 10 chunks of 16 */
  AllocChunk chunk = AllocChunk(256,10);
  
  /* array of 32 pointers */
 void* ptr[32];
 for(int i=0; i<32;i++)
   {
   ptr[i]=chunk.alloc();
   if(!TestWriteToAChunk(ptr[i],256/8))
     return 0;
   }
 chunk.free_all();
 return 1;
}
int main()
{
  TestChunkWrite();
  if(!TestAllocMoreBlocks())
    cout<<"more blocks failed";
}

