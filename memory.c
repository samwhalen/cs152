#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "bst.h"
#include "llist.h"
#include "my_alloc.h"

extern bst* avail_mem;
extern bst* avail_mem_by_addr;

/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void* addr, unsigned int size){
  memory* m = (memory*)malloc(sizeof(memory));
  m->addr = addr;
  m->size = size;
  return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
  memory* m = (memory*)p;
  free(m);
}

/* compare two memory variables x and y by address 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y){

	memory* nx = (memory*)x;
	memory* ny = (memory*)y;
	
	if((nx->addr)<(ny->addr)){
		return -1;
	}
	if((nx->addr)>(ny->addr)){
		return 1;
	}
	if((nx->addr)==(ny->addr)){
		return 0;
	}
	else{
		printf("error\n");
		return 2;
	}
}

/* compare two memory variables x and y by size 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y){

	memory* nx = (memory*)x;
	memory* ny = (memory*)y;

	if((nx->size)<(ny->size)){
		return -1;
	}
	if((nx->size)>(ny->size)){
		return 1;
	}
	else{ //if((nx->size)==(ny->size)){
		return 0;
	}
}

/* print the memory address and size
 */
void memory_print(void* data){
  if (data == NULL) return;
  memory* m = (memory*)data;
  printf("address: %lu, size: %u\n", (unsigned long)m->addr, m->size);
}

/* merge_memory
 *
 * Given two memory structs, check to see if the two can be 
 * merged. They can be merged if the two are next to each other 
 * in memory with no break in between. If they can be merged,
 * return a memory struct pointer to a struct containing the information
 * for a single memory chunk containing the old two chunks.
 * If they cannot be merged (there is space between them), then
 * return NULL; 
 *
 * Make sure that you free any memory structs that you need to.
 */
memory* merge_memory(memory* first, memory* second)
{
	memory* res = NULL;
	char* firstaddr = (char*)first->addr;
	char* secondaddr = (char*)second->addr;

	if((first == NULL) || (second == NULL)){
		return res;
	}
	if((firstaddr + first->size + 8) == secondaddr){
		bst_delete(avail_mem, first);
		bst_delete(avail_mem, second);
		unsigned int newsize = (first->size + second->size + 8);
		res = memory_new(first->addr, newsize);
		bst_insert(avail_mem, res);
		return res;
	}
	if((secondaddr + second->size + 8) == firstaddr){
		bst_delete(avail_mem, first);
		bst_delete(avail_mem, second);
		unsigned int newsize = (first->size + second->size + 8);
		res = memory_new(second->addr, newsize);
		bst_insert(avail_mem, res);
		return res;
	}
	return res;
}

//compact
void compact(){

	//turn avail_mem into a llist
	avail_mem_by_addr = bst_new(memory_addr_cmp);
	llist* list = create_llist();
	inorder_traversal_insert_llist(avail_mem->root, list);
	
	//print list to check it
	printf("Printing avail_mem in llist form:\n");
	llist_node* checkcurrent = list->head;
	for(checkcurrent = list->head; checkcurrent != NULL;
			checkcurrent = checkcurrent->next){
		memory_print(checkcurrent->item);
	}

	//insert llist into tree that is sorted by addr
	llist_node* current = list->head;
	for(current = list->head; current != NULL; current = current->next){
		memory* mem = current->item;
		bst_insert(avail_mem_by_addr, mem);
	}
	
	//turn avail_mem_by_addr into a linked list
	llist* addrlist = create_llist();
	inorder_traversal_insert_llist(avail_mem_by_addr->root, addrlist);
	printf("Printing avail_mem_by_addr in llist form:\n");
	current = addrlist->head;
	for(current = addrlist->head; current != NULL;
			current = current->next){
		memory_print(current->item);
	}

	current = NULL;

	llist_node* curr = addrlist->head;
	llist_node* succ;
	for(succ=addrlist->head->next; succ != NULL; succ= succ->next){
		memory* res = merge_memory(curr->item, succ->item);
		if(res != NULL){
			((memory*)(curr->item))->size +=
				((memory*)(succ->item))->size + 8;
			bst_delete(avail_mem_by_addr, curr->item);
			bst_delete(avail_mem_by_addr, succ->item);
			bst_insert(avail_mem_by_addr, res);
			}
		if(res == NULL){
			curr= succ;
		}
	}
}
















