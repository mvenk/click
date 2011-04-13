#include<click/allocchunk.hh>
#include<iostream>
#include<cstring>
using namespace std;

// Global static pointer used to ensure a single instance of the class.
AllocChunk* AllocChunk::m_pInstance = 0;  

/** This function is called to create an instance of the class. 
    Calling the constructor publicly is not allowed. The constructor 
    is private and is only called by this Instance function.
*/

AllocChunk* AllocChunk::Instance()
{
  if (!m_pInstance)   // Only allow one instance of class to be generated.
    m_pInstance = new AllocChunk();

  return m_pInstance;
}


void AllocChunk :: create(int size,int multiple)
{
  this->size = size;
  this->multiple = multiple; 
  head = 0; 
  tail = 0;
}

AllocChunk::AllocChunk()
{

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

