#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"

/* create_count
 * creates a wordcount struct, copies the word to its memory,
 * and initializes the count to 1.
 */
wordcount *create_count(char *word)
{
	wordcount* res = (wordcount*)malloc(sizeof(wordcount));
	res->count = 1;
	strcpy(res->word, word);
	return res;
}

/* This frees all of the dynamically allocated fields (just word) */
void free_count(wordcount *wc)
{
	free(wc);
}

/* increment_count
 * increments the count field of a wordcount struct
 */
void increment_count(wordcount *wc)
{
	++wc->count;
}

/* get_count
 * this function is provided to test your accumulator function
 * for your BST.
 * This returns the count in a particular word.
 */
int get_count(void *v)
{
	wordcount* newwc = (wordcount*)v;
	return newwc->count;	
}

void print_word(void* v)
{
	wordcount *wc = (wordcount*)v;
	if(wc == NULL){
		printf("NULL pointer\n");
	}
	else {
		printf("%s",wc->word);
	}
}

/* compare_words
 * This function compares two wordcount structs. It compares by comparing
 * the words being stored. The order is the same order as in strcmp.
 * It returns -1 if v1 < v2, 0 if v1 == v2, and 1 if v1 > v2 as defined
 * by alphabetical order.
 */
int compare_words(void *v1, void *v2)
{
	int res = strcmp(v1, v2);
	return res;
}


/* compare_counts
 * This function compares two wordcount structs. It compares by comparing
 * the counts being stored. 
 * It returns -1 if v1 < v2, 0 if v1 == v2, and 1 if v1 > v2 as defined
 * by the value of the count fields.
 */
int compare_counts(void *v1, void *v2)
{
	wordcount* n1 = (wordcount*)v1;
	wordcount* n2 = (wordcount*)v2;
	if(n1->count > n2->count){
		return 1;
	}
	if(n1->count < n2->count){
		return -1;
	}
	else {
		return 0;
	}
}

/* get_key
 * transforms the word into an integer key
 */
unsigned long int get_key(void *v){
	
	wordcount* nwc = (wordcount*)v;
	unsigned long int res = 17;
	unsigned int i;
	for(i=0; i < 100; i++){
		if(nwc->word[i] == '\0'){
			break;
		}
		else {
			res = res*37+i;
		}
	}
	return res;
}




