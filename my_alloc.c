#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"

/* Global variables
 * By declaring these outside of a function:
 *   Scope: Every function inside of this file may use them. 
 *   		No functions in other files may use them.
 *   Lifetime: They exist for the entire program - they are created
 *		when the program begins and exist until the program
 *		ends.
 */   		

/* This is the tree that holds the available memory. */
bst *avail_mem = NULL;
bst *avail_mem_by_addr = NULL;

/* This includes all of the functions for the memory allocator.
 * The last two functions (my_malloc and my_free) are public 
 * functions with prototypes in the header function. The rest of the
 * functions are helper functions only called by my_malloc and 
 * my_free. You must implement these helper functions. If you wish,
 * you may add helper functions we did not define.
 */

/* init_alloc
 *
 * Initializes the data structures. This initializes avail_mem so 
 * that, instead of being a NULL pointer, it points to a valid bst
 * struct whose root pointer is NULL.
 */
void init_alloc()
{
	bst* nroot = bst_new(memory_size_cmp);
	avail_mem = nroot;
}

/* allocate_memory_page
 *
 * allocate one page (4096 bytes) of memory 
 */
memory* allocate_memory_page(){

	memory* ret_val = memory_new((void*)malloc(4096), 4088);
	return ret_val;	
}

/* split_memory
 *
 * split memory into two parts
 * inputs
 *   memory* data - the memory to be split
 *   size_desired - the size of memory to return
 */
void* split_memory(memory* data, unsigned int size_desired){

	if((size_desired + 8) > data->size){
		printf("ERROR: split_memory: NULL pointer returned.\n");
		printf("The size_desired was greater than the available");
		printf(" memory to be split.\n\n");
	}
	if(data == NULL){
		printf("ERROR: split_memory: NULL pointer returned.\n");
		printf("Given NULL pointer. Expected a memory pointer.\n\n");
		return NULL;
	}
	else {
		char* counter = (char*)data->addr;
		counter += (data->size - size_desired);
		*(int*)counter = size_desired;
		counter += 8;
		data->size -= (size_desired + 8);
		return counter;
	}
}

//node_item_or_successor
//void* item is meant to be a memory*
//the function is meant to return a node*
void* node_item_or_successor(node* n, void* item,
		int (*cmp)(const void* x, const void* y))
{
	if(n == NULL){
		return NULL;
	}
	void* lresult = node_item_or_successor(n->left, item, cmp);
	if(lresult != NULL){
		return lresult;
	}
	int signal = cmp(item, n->data);
	switch(signal){
		case -1:
		case 0:
			return n->data;
			break;
	}
	return node_item_or_successor(n->right, item, cmp);
}

//bst_item_or_successor
//void* item is meant to be a memory*
//the function returns a node*
void* bst_item_or_successor(bst* b, void* item)
{
	return node_item_or_successor(b->root, item, b->cmp);
}

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. Returns a pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(unsigned int num_bytes)
{
	if((num_bytes%8) != 0){
		num_bytes += (8 - num_bytes%8);
	}
	memory* num_bytes_mem = memory_new(&num_bytes, num_bytes);
	memory* chunk = bst_item_or_successor(avail_mem, num_bytes_mem);
	if(chunk == NULL){
		//printf("enters null statement in my_malloc\n");
		memory* new_page = allocate_memory_page();
		bst_insert(avail_mem, new_page);
		return my_malloc(num_bytes);
	}
	if(chunk->size >= (num_bytes * 2)){
		//printf("enters split call in my_malloc\n");
		void* res = split_memory(chunk, num_bytes);
		//printf("returns res\n");
		bst_delete(avail_mem, chunk);
		bst_insert(avail_mem, chunk);
		return res;
	}
	if((chunk->size < (num_bytes * 2)) ||
		(chunk->size >= num_bytes)){
		//printf("enters handout statement in my_malloc\n");
		bst_delete(avail_mem, chunk);
		void* res = (chunk->addr + 8);
		return res;
	}
	return NULL;
}

/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address)
{
	char* counter = address;
	counter -= 8;
	unsigned int size = *(int*)counter;
	memory* insertmem = memory_new(counter, size);
	bst_insert(avail_mem, insertmem);
}
