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
  int _is_created;
  AllocChunk();
  ~AllocChunk();
public:
  static AllocChunk* Instance();
  AllocChunk(AllocChunk const&){};             // copy constructor is private
  AllocChunk& operator=(AllocChunk const&){};  // assignment operator is private
  static AllocChunk* m_pInstance;
  void init_block_list(void);
  void add_block(void);
  void * alloc(void);
  void free_all(void);
  void create(int size,int multiple);
};
#endif
