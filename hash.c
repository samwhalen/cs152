#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "hash.h"
#include "word.h"

#define MAX_BUFFER 150

hash_node_t* new_node(wordcount* wc, unsigned int key)
{
	hash_node_t* nn = (hash_node_t*)malloc(sizeof(hash_node_t));
	nn->key = key;
	nn->value = wc;
	nn->next = NULL;
	return nn;
}

unsigned int generate_hash(hash_table_t *ht, unsigned long int key) 
{
	unsigned int res = key % ht->size;
	return res;
}


hash_table_t* create_hash_table(unsigned int size,
	       	enum table_type type,
	       	unsigned long int (*getkey)(void *),
	       	int (*compare)(void *, void *))
{
	hash_table_t* newhash = (hash_table_t*)malloc(sizeof(hash_table_t));
	newhash->ttype = type;
	newhash->size = size;
	hash_node_t** harray = 
		(hash_node_t**)malloc(sizeof(hash_node_t*)*size);
	unsigned int i;
	for(i=0; i < size; i++){
		harray[i] = new_node(NULL, 0);
	}
	newhash->head = harray;
	newhash->getkey = getkey;
	newhash->compare = compare;
	return newhash;
}


int insert(hash_table_t *ht, void *value, int *opcount)
{
	if(ht->ttype  == 0){
		return insert_linear(ht, value, opcount);
	}
	if(ht->ttype == 1){
		return insert_quadratic(ht, value, opcount);
	}
	else { //if(ht->ttype == 2){
		return insert_chained(ht,value, opcount);
	}
}


hash_node_t* search(hash_table_t *ht, void *value, int *opcount)
{
	if(ht->ttype  == 0){
		return search_linear(ht, value, opcount);
	}
	if(ht->ttype == 1){
		return search_quadratic(ht, value, opcount);
	}
	if(ht->ttype == 2){
		return search_chained(ht,value, opcount);
	}
	return NULL;
}


int insert_chained(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc = (wordcount*)value;
	int index = generate_hash(ht, get_key(wc));

	//print_word(arrayhead[index]->value);
	if(!arrayhead[index]->value){
		arrayhead[index]->value = wc;
		return 1;
	}
	else {
		//print_word(arrayhead[index]->value);
		hash_node_t* nn = new_node(wc, index);
		nn->next = arrayhead[index];
		arrayhead[index] = nn;
		return 1;
	}
	return 0;
}

int insert_linear(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc = (wordcount*)value;
	int index = generate_hash(ht, get_key(wc));
	unsigned int i;
	for(i=index; i < ht->size; i++){
		if(!arrayhead[i%ht->size]->value){
			arrayhead[i%ht->size]->value = wc;
			return 1;
		}
		*opcount += 1;
	}
	return 0;
}

int insert_quadratic(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc = (wordcount*)value;
	int index = generate_hash(ht, get_key(wc));
	unsigned int i;
	unsigned int x = 1;
	for(i=(index+1); i < ht->size; i = (index + (x*x))){
		if(!arrayhead[i%ht->size]->value){
			arrayhead[i%ht->size]->value = wc;
			return 1;
		}
		x++;
		*opcount += 1;
	}
	return 0;
}

hash_node_t* search_chained(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc1 = (wordcount*)value;
	unsigned int index = generate_hash(ht, get_key(wc1));
	if(!arrayhead[index]->value){
		fprintf(stderr, "ERROR: search_chained: NULL node in array\n");
		fprintf(stderr, " at specified index.\n");
		return NULL;
	}
	else { 
		hash_node_t* tmp;
		for(tmp=arrayhead[index]; tmp != NULL; tmp = tmp->next){
			wordcount* wc2 = tmp->value;
			int signal = compare_words(wc1, wc2);
			switch(signal){
				case 0:
					return tmp;
				default:
					break;
			}
			*opcount += 1;
		}
	}
	return NULL;
}


hash_node_t* search_linear(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc1 = (wordcount*)value;
	unsigned int index = generate_hash(ht, get_key(wc1));
	unsigned int i;
	for(i=index; i < ht->size; i++){
		if(arrayhead[i%ht->size]->value){
			wordcount* wc2 = arrayhead[i%ht->size]->value;
			int signal = compare_words(wc1, wc2);
			switch(signal){
				case 0:
					return arrayhead[index];
				default:
					break;
			}
		}
		*opcount += 1;
	}
	return NULL;
}

hash_node_t* search_quadratic(hash_table_t *ht, void *value, int *opcount)
{
	hash_node_t** arrayhead = ht->head;
	wordcount* wc1 = (wordcount*)value;
	unsigned int index= generate_hash(ht, get_key(wc1));
	unsigned int i;
	unsigned int x = 1;
	for(i=index; i < ht->size; i = (index + (x*x))){
		if(arrayhead[i%ht->size]->value){
			wordcount* wc2 = arrayhead[i%ht->size]->value;
			int signal = compare_words(wc1, wc2);
			switch(signal){
				case 0:
					return arrayhead[index];
				default:
					break;
			}
		}
		*opcount += 1;
		x++;
	}
	return NULL;
}



float insert_search_from_file(hash_table_t *ht, char * filename)
{
    FILE *fp;
    char buf[MAX_BUFFER] = "";
    int count=0, total=0;

    // attempt to open the file, then check whether that worked.
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr,"Could not open file %s\n",filename);
        return (1);
    }

    char* op, *word;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf)-1] = '\0';
        op = strtok(buf, " \n");
        word = strtok(NULL, " \n");
        wordcount *wc = create_count(word);
        if (strcmp(op, "insert") == 0)
            total += printed_insert(ht, (void *)wc);
        else if (strcmp(op, "search") == 0)
            total += printed_search(ht, (void *)wc);
        else{
            printf("operation %s not supported\n", op);
        }
        count++;
    }

    fclose(fp);

    return (float) total/count;
}

int printed_insert(hash_table_t* ht, void *value)
{
    int opcount = 0;
    insert(ht, value, &opcount);
    printf("Inserted (%lu,",ht->getkey(value));
    print_word(value);
    printf("), Opcount: %d\n", opcount);

    return opcount;
}

int printed_search(hash_table_t* ht, void *value)
{
    int opcount = 0;
    hash_node_t *search_node = search(ht, value, &opcount);
    printf("Searched value (%lu,",ht->getkey(value));
    print_word(value);
    printf("), Opcount: %d\n", opcount);
    if(search_node)
        printf("Item Found\n");
    else
        printf("Item Not Found\n");

    return opcount;
}

void print_hash_table_entries(hash_table_t *ht)
{
    int i;
    print_word(ht->head[9]->value);
    for(i=0; i < ht->size; i++)
    {
	    printf("i = %d\n", i);
        hash_node_t *ptr = ht->head[i];
        // print the first one
        if (ptr->value != NULL)
        {
            printf("[%d]= (%lu,", i ,ptr->key);
            print_word(ptr->value); // yes, breaking abstraction
            printf(") ");
            ptr=ptr->next;
        } else
            printf("[%d]= NULL ", i);
        // print the rest
        while(ptr && (ptr->value != NULL))
        {
            printf(" -> (%lu,", ptr->key);
            print_word(ptr->value); // yes, breaking abstraction
            printf(") ");
            ptr=ptr->next;
        }
        printf("\n");
    }
}
