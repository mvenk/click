#ifndef ALLOC_CHUNK_HH
#define ALLOC_CHUNK_HH
struct block_node { 
  char * block;
  int blocks_left;
};


struct block_list {
  struct block_node node;
  struct block_list *next;
};

class AllocChunk{

private:

  char * blocks;
  int blocks_left;
  int size;
  int multiple;
  struct block_list *head;
  struct block_list *tail;
public:
  AllocChunk(int size,int multiple);
  ~AllocChunk();
  void init_block_list(void);
  void add_block(void);
  void * alloc(void);
  void free_all(void);
};
#endif
