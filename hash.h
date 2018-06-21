#ifndef _HASH_H_
#define _HASH_H_
#include "word.h"


enum table_type { LINEAR, QUADRATIC, CHAINED };

typedef struct _hash_node_t hash_node_t;

struct _hash_node_t {
    unsigned long int key;
    void *value;
    hash_node_t *next;  //For chaining only
};


typedef struct _hash_table_t hash_table_t;
struct _hash_table_t{
    enum table_type ttype;
    unsigned int size;
    hash_node_t **head;
    unsigned long int (*getkey)(void *);
    // Function pointer to compare items
    int (*compare)(void *, void *);
};

//added function to create a new hash_table node
hash_node_t* new_node(wordcount* wc, unsigned int key);

unsigned int generate_hash(hash_table_t *ht, unsigned long int key);

// type is either chained, linear, quadratic
hash_table_t* create_hash_table(unsigned int size, enum table_type type,
				unsigned long int (*getkey)(void *),
				int (*compare)(void *, void *));

int insert(hash_table_t *ht, void *value, int *opcount);

hash_node_t* search(hash_table_t *ht, void *value, int *opcount);

int insert_chained(hash_table_t *ht, void *value, int *opcount);

hash_node_t* search_chained(hash_table_t *ht, void *value, int *opcount);

int insert_linear(hash_table_t *ht, void *value, int *opcount);

hash_node_t* search_linear(hash_table_t *ht, void *value, int *opcount);

int insert_quadratic(hash_table_t *ht, void *value, int *opcount);

hash_node_t* search_quadratic(hash_table_t *ht, void *value, int *opcount);

void print_hash_table_entries(hash_table_t *ht);

float insert_search_from_file(hash_table_t *ht, char * filename);


int printed_insert(hash_table_t* ht, void *value);

int printed_search(hash_table_t* ht, void *value);

#endif
