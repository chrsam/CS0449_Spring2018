#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mymalloc.h" 

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

#define LARGEST_BIN_SIZE 512 
#define SIZE_OF_USED offsetof(BlockHeader, prev_free_block)
#define MIN_SIZE (MINIMUM_ALLOCATION + SIZE_OF_USED)
#define NUMBER_OF_BINS (2 + ((LARGEST_BIN_SIZE - MINIMUM_ALLOCATION)/SIZE_MULTIPLE))
#define OVERFLOW (NUMBER_OF_BINS - 1)

struct Node {
	struct Node* previous; 	// pointer to the previous block
	struct Node* next;		// pointer to the next block 
	int required_size; 		// the size of the data part 
	int used;				// represents availibility of a single block; 0 == free, 1 == not free
};

struct Node *head;						// keeps track of head node 
struct Node *tail;						// keeps track of tail node
struct Node *current_node;				// keeps track of current node 
struct Node *search_node;
struct Node *bins[NUMBER_OF_BINS] = {}; //array to keep track of bins

// Prototypes
void *insert_alloc(unsigned int);
void *add_to_heap(unsigned int);
void *find_best_fit(unsigned int);

unsigned int round_up_size(unsigned int data_size) {
	if(data_size == 0)
		return 0;
	else if(data_size < MINIMUM_ALLOCATION)
		return MINIMUM_ALLOCATION;
	else
		return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

void* split(unsigned int data_size)
{
	current_node->used = 0;

	unsigned int free_space = current_node->required_size - sizeof(struct Node);
	if(free_space > (data_size + sizeof(struct Node)))
	{
		struct Node *new_node = ((void*)current_node) + data_size + sizeof(struct Node);
		new_node->used = 1;
		new_node->required_size = free_space - data_size;
		new_node->next = current_node->next;
		new_node->previous = current_node;

		current_node->next = new_node;
		current_node->required_size = data_size;

		return ((void*)current_node + sizeof(struct Node));
	} else {
		return ((void*)current_node + sizeof(struct Node));
	}
}

/*	my_malloc() function logic:
 *	1) Rounds up the size of the requested allocation to a certain size (given).
 *	2) Tries to find a block to reuse, using "best-fit" allocation.
 *	3) If a block is found, and that block can be split into two smaller blocks, do so.
 *	4) If it's reusing a block, mark it as used. 
 *	5) If it couldn't find a block, expand the heap by moving the break with sbrk().
 */
void* my_malloc(unsigned int size) {
	if(size == 0)	// checks for valid size request 
		return NULL;

	size = round_up_size(size); // (1) 

	// ------- Don't remove anything above this line. -------
	// Here's where you'll put your code!
	// AT LEAST SOME OF IT
	// USE FUNCTIONS
	// PLEASE

	// If the head node points to NULL, then no allocation needs to be made. 
	// Initializes by creating first allocation made. 
	// Returns a pointer to address of Node's data segment. 
	if(head == NULL) {
		head = sbrk(size + sizeof(struct Node));
    	head->used = 1; // head->free_value = 0? 
    	head->required_size = size;
    	head->next = NULL;
    	head->previous = NULL;
    	tail = head;
    	return ((void *)head + sizeof(struct Node));
	} else {
		current_node = find_best_fit(size);
  		if(current_node == NULL) {
  			// printf("expanding heap...\n");
    		return(add_to_heap(size)); 
 		} else if (current_node->required_size == size) {
 			current_node->used = 1; 
     		return ((void *)current_node + sizeof(struct Node));
 		} else {
      		return insert_alloc(size);
  		}
	}  
}

void coalesce(void* node) {
	struct Node* coalescing_node = (struct Node*) node; 
	if(coalescing_node == NULL) 
	{
		return;
	}

	if(coalescing_node->previous != NULL) {
		if(coalescing_node->previous->used == 0)
		{
			coalescing_node->previous->required_size = (coalescing_node->required_size + sizeof(struct Node));
			coalescing_node->previous->next = coalescing_node->next;
			if(coalescing_node->next != NULL)
			{
				coalescing_node->next->previous = coalescing_node->previous;
			} else {
				tail = coalescing_node->previous;
			}
		}
	}
	if(coalescing_node->next != NULL) {
		if(coalescing_node->next->used == 0)
		{
			coalescing_node->required_size += (coalescing_node->next->required_size + sizeof(struct Node));
			coalescing_node->next = coalescing_node->next->next;
			if(coalescing_node->next != NULL)
			{
				coalescing_node->next->previous = coalescing_node;
			} else {
				tail = coalescing_node;
			}
		}
	}
}

// and here's where you free stuff.
void my_free(void* ptr)
{
	if(ptr == NULL)
		return;

	// create a node that points to ptr, minus the size of the header
  	// struct Node *free_node = (struct Node*)(ptr - sizeof(struct Node));
  	struct Node *free_node = ptr - sizeof(struct Node); 
  	// set the node to NOT in use 
  	free_node->used = 0;

  	while(free_node != NULL && free_node == tail) 
  	{
  		if(free_node->used != 0)
  		{
  			break;
  		}
  		tail->next = NULL;
  		tail = tail->previous;
  		sbrk(-(((void*)sbrk(0)) - ((void*)free_node)));
  		free_node = tail;
  		current_node = tail;
  		if(free_node == NULL)
  		{
  			head = NULL;
  			tail = NULL;
  			current_node = NULL;
  			break; 
  		}
  		coalesce(free_node);
  	}
}

void *add_to_heap(unsigned int size) {
  struct Node *add_node = sbrk(size + sizeof(struct Node));

  add_node->required_size = size;
  add_node->used = 1;
  add_node->previous = tail;
  add_node->next = NULL;
  tail->next = add_node;
  tail = add_node;

  return ((void *)tail + sizeof(struct Node));
}

void *insert_alloc(unsigned int size) {
  
  struct Node *insert_node = current_node + (size + sizeof(struct Node));

  insert_node->required_size = current_node->required_size - size - sizeof(struct Node);
  insert_node->used = 0;
  // insert_node->required_size = current_node->required_size - size;
  insert_node->next = current_node->next;
  insert_node->previous = current_node;
  // current_node->next->previous = insert_node;
  insert_node->next->previous = insert_node;
  insert_node->previous->next = current_node;

  current_node->required_size = size + sizeof(struct Node);
  current_node->next = insert_node;
  current_node->next->previous = current_node;
  // current_node->required_size = size;
  current_node->used = 1;
  
  return ((void *)current_node + sizeof(struct Node));
}

void *find_best_fit(unsigned int size)	{
  // search_node = head->next;
  search_node = head;

  while(search_node != NULL){
    if(search_node->used == 0) {
      if(search_node->required_size == size) {
		search_node->used = 1;
		current_node = search_node; 
		return (char*)current_node;
      } else if (search_node->required_size > size && current_node == NULL) {
		current_node = search_node;
      } else if (search_node->required_size > size && current_node->required_size > search_node->required_size) {
		current_node = search_node;
      }
    }
    search_node = search_node->next;
  } 
  
  return (char*)current_node;
}
