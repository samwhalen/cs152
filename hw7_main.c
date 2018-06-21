#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "llist.h"
#include "my_alloc.h"
#include "memory.h"

//test functions

//test split_memory
void test_split_memory(memory* data,
	       	unsigned int size_desired,
		unsigned int testnum){

	printf("Test %d:\n", testnum);
	printf("Split %d bytes away from the available memory.\n",
			size_desired);
	memory* testmem = allocate_memory_page();
	printf("Initial address (of memory to be split): %p\n", 
			testmem->addr);
	void* testres = split_memory(testmem, size_desired);
	printf("Resultant address (after split): %p\n", testres);
	printf("Int value of testmem%d's initial address: %d\n",
		       	testnum, (int)testmem->addr);
	printf("Int value of resultant address: %d\n", (int)testres);
	int diff = (int)testres - (int)testmem->addr;
	printf("Int differnce: %d\n", diff);
	int actual = 4096 - diff;
	int expected = size_desired;
	printf("Actual size split off: %d\n", actual);
	printf("Expected: %d\n", expected);
	if(actual == expected){
		printf("SUCCESS!\n");
	}
	else {
		printf("Test failed.\n");
	}
	printf("\n");
}

int main(int argc, char* argv[]){

	//header
	printf("\n\n====== Beginning of hw7 ================");
	printf("========================================\n\n");
	
	if(argc < 2){
		printf("ERROR: hw7: program requires at least");
		printf(" 2 argc values on command line.\n");
		exit(1);
	}
	if(atoi(argv[1]) > 7){
		printf("ERROR: hw7: no test for given argv[1] value");
		printf(" in command line.\n");
		exit(2);
	}
	
	//testing
	printf("====== Testing ======\n\n");

	if(atoi(argv[1]) == 0){
		
		//testing split_memory
		printf("Testing split_memory:\n\n");
		
		//test1
		memory* testmem1 = allocate_memory_page();
		test_split_memory(testmem1, 128, 1);

		//test2
		memory* testmem2 = allocate_memory_page();
		test_split_memory(testmem2, 1000, 2);

		//test3 -- this should fail on purpose
		memory* testmem3 = allocate_memory_page();
		test_split_memory(testmem3, 7, 3);
	}
	if(atoi(argv[1]) == 1){
		
		//testing bst_iterator
		printf("Testing bst_iterator:\n\n");

		//initialize avail_mem
		extern bst* avail_mem;
		init_alloc();
	
		int testaddr1 = 123123;
		memory* testmem1 = memory_new(&testaddr1, 24);
		bst_insert(avail_mem, testmem1);

		int testaddr2 = 123141;
		memory* testmem2 = memory_new(&testaddr2, 80);
		bst_insert(avail_mem, testmem2);

		int testaddr3 = 121515;
		memory* testmem3 = memory_new(&testaddr3, 1024);
		bst_insert(avail_mem, testmem3);

		//print avail_mem using inorder_traversal
		printf("Printing avail_mem after adding three nodes");
		printf(" using inorder_traversal with memory_print:\n");     
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");

		//print avail_mem using iterator
		printf("Printing avail_mem after adding three nodes");
		printf(" using bst_iterate:\n");
		void* item;
		for(item = bst_iterate(avail_mem); 
				item != NULL; item = bst_iterate(NULL)){
			memory_print((memory*) item);
		}
		
		//free bst	
		bst_free(avail_mem);
		printf("\n");
	}
	if(atoi(argv[1]) == 2){
		
		//testing my_malloc and my_free
	        //printf("(atoi(argv[1]) == 2)\n");
		printf("Testing my_malloc and my_free by requesting");
		printf(" 7 bytes and then freeing it:\n\n");
		
		extern bst* avail_mem;
		init_alloc();
		
		int testaddr1 = 123123;
		memory* testmem1 = memory_new(&testaddr1, 32);
		bst_insert(avail_mem, testmem1);

		int testaddr2 = 123141;
		memory* testmem2 = memory_new(&testaddr2, 80);
		bst_insert(avail_mem, testmem2);

		int testaddr3 = 121515;
		memory* testmem3 = memory_new(&testaddr3, 1024);
		bst_insert(avail_mem, testmem3);

		//print avail_mem before my_alloc call
		printf("Printing avail_mem after adding three nodes");
		printf(" using inorder_traversal with memory_print:\n");     
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");
		
		char* res = my_malloc(7);
		printf("Address of memory split off: %p\n\n", res);
		
		//print avail_mem after my_alloc call
		printf("Printing avail_mem after my_alloc call:\n");
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");

		//my_free call
		my_free(res);

		//print avail_mem after my_free call
		printf("Printing avail_mem after my_free call");
		printf(" on %p:\n", res);
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");

		//free avail_mem
		bst_free(avail_mem);
	}
	if(atoi(argv[1]) == 3){
		
		//testing my_malloc and my_free
		printf("Testing my_malloc and my_free by requesting");
		printf(" 1000 bytes from an empty tree\nand");
	        printf(" then freeing it:\n\n");
		
		extern bst* avail_mem;
		init_alloc();
		
		//print avail_mem before my_alloc call
		printf("Printing initial avail_mem:\n");     
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");
	
		char* res = my_malloc(1000);
		printf("Address of memory handed out: %p\n\n", res);
		
		//print avail_mem after my_alloc call
		printf("Printing avail_mem after my_malloc call:\n");
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");

		//my_free call
		my_free(res);

		//print avail_mem after my_free call
		printf("Printing avail_mem after my_free call");
		printf(" on %p:\n", res);
		bst_inorder_traversal(avail_mem, memory_print);
		printf("\n");

		//free avail_mem
		bst_free(avail_mem);
	}
	if(atoi(argv[1]) == 4){
		
		//test merge_memory
		printf("Testing merge_memory:\n\n");

		extern bst* avail_mem;
		init_alloc();

		memory* testmem1 = memory_new(malloc(800), 792);
		memory_print(testmem1);
		char* testaddr2 = (char*)split_memory(testmem1, 256);
		testaddr2 -= 8;
		memory* testmem2 = memory_new((void*)testaddr2, 256);
		memory_print(testmem2);
		printf("Int value of testmem1: %d\n", (int)testmem1->addr);
		printf("Int value of testmem2: %d\n", (int)testmem2->addr);
		memory* res = merge_memory(testmem1, testmem2);
		printf("Printing the merged memory:\n");
		memory_print(res);

		//free bsts
		bst_free(avail_mem);

	}
	if(atoi(argv[1]) == 5){
		
		//test compact
		printf("Testing compact -- test 1:\n\n");

		extern bst* avail_mem;
		init_alloc();
		extern bst* avail_mem_by_addr;
		
		//add three nodes to avail_mem (non adjacent)
		memory* testmem1 = memory_new(malloc(40), 32);
		bst_insert(avail_mem, testmem1);

		memory* testmem2 = memory_new(malloc(88), 80);
		bst_insert(avail_mem, testmem2);

		memory* testmem3 = memory_new(malloc(1032), 1024);
		bst_insert(avail_mem, testmem3);
	
		//create and insert two adjacent mem blocks
		
		//first block
		memory* testmem4 = memory_new(malloc(800), 792);
		printf("First consecutive address to insert:\n");
		memory_print(testmem4);
		
		//second block
		char* testaddr5 = (char*)split_memory(testmem4, 256);
		testaddr5 -= 8;
		memory* testmem5 = memory_new((void*)testaddr5, 256);
		printf("Second consecutive address to insert:\n");
		memory_print(testmem5);

		printf("\n");
		
		//insert them into avail_mem
		bst_insert(avail_mem, testmem4);
		bst_insert(avail_mem, testmem5);
	
		compact();

		//print avail_mem after compact
		printf("Printing avail_mem after compact call:\n");
		bst_inorder_traversal(avail_mem, memory_print);

		//print avail_mem_by_addr after compact
		printf("Printing avail_mem_by_addr after compact call:\n");
		bst_inorder_traversal(avail_mem_by_addr, memory_print);

		//free the two bsts
		bst_free(avail_mem);
		bst_free(avail_mem_by_addr);

		printf("\n");
	}
	if(atoi(argv[1]) == 6){
		
		//test compact
		printf("Testing compact -- test 2:\n\n");

		extern bst* avail_mem;
		init_alloc();
		extern bst* avail_mem_by_addr;
		
		//add three nodes to avail_mem (non adjacent)
		memory* testmem1 = memory_new(malloc(40), 32);
		bst_insert(avail_mem, testmem1);

		memory* testmem2 = memory_new(malloc(102), 96);
		bst_insert(avail_mem, testmem2);

		memory* testmem3 = memory_new(malloc(1032), 1024);
		bst_insert(avail_mem, testmem3);
	
		memory* testmem4 = memory_new(malloc(808), 800);
		bst_insert(avail_mem, testmem4);
		
		memory* testmem5 = memory_new(malloc(16), 8);
		bst_insert(avail_mem, testmem5);
	
		compact();

		//print avail_mem after compact
		printf("Printing avail_mem after compact call:\n");
		bst_inorder_traversal(avail_mem, memory_print);

		//print avail_mem_by_addr after compact
		printf("Printing avail_mem_by_addr after compact call:\n");
		bst_inorder_traversal(avail_mem_by_addr, memory_print);

		//free the two bsts
		bst_free(avail_mem);
		bst_free(avail_mem_by_addr);

		printf("\n");
	}
	if(atoi(argv[1]) == 7){
		
		//test compact
		printf("Testing compact -- test 1:\n\n");

		extern bst* avail_mem;
		init_alloc();
		extern bst* avail_mem_by_addr;
		
		//add three nodes to avail_mem (non adjacent)
		memory* testmem1 = memory_new(malloc(40), 32);
		bst_insert(avail_mem, testmem1);

		memory* testmem2 = memory_new(malloc(88), 80);
		bst_insert(avail_mem, testmem2);

		memory* testmem3 = memory_new(malloc(1032), 1024);
		bst_insert(avail_mem, testmem3);
	
		//create and insert three adjacent mem blocks
		
		//first block
		memory* testmem4 = memory_new(malloc(1000), 992);
		
		//second block
		char* testaddr5 = (char*)split_memory(testmem4, 256);
		testaddr5 -= 8;
		memory* testmem5 = memory_new((void*)testaddr5, 256);
		
		//third block
		char* testaddr6 = (char*)split_memory(testmem5, 96);
		testaddr6 -= 8;
		memory* testmem6 = memory_new((void*)testaddr6, 96);

		printf("First consecutive address to insert:\n");
		memory_print(testmem4);
		
		printf("Second consecutive address to insert:\n");
		memory_print(testmem5);
		
		printf("Third consecutive address to insert:\n");
		memory_print(testmem6);
		
		printf("\n");
		
		//insert them into avail_mem
		bst_insert(avail_mem, testmem4);
		bst_insert(avail_mem, testmem5);
		bst_insert(avail_mem, testmem6);
	
		compact();

		//print avail_mem after compact
		printf("Printing avail_mem after compact call:\n");
		bst_inorder_traversal(avail_mem, memory_print);

		//print avail_mem_by_addr after compact
		printf("Printing avail_mem_by_addr after compact call:\n");
		bst_inorder_traversal(avail_mem_by_addr, memory_print);

		//free the two bsts
		bst_free(avail_mem);
		bst_free(avail_mem_by_addr);

		printf("\n");
	}

	//footer
	printf("====== End of hw7 ==========================");
	printf("====================================\n\n");

}


