#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include "hash.h"

void do_full_test(char *filename, int table_size);

int main(int argc, char* argv[])
{
        
    hash_table_t* ht1 = create_hash_table(10, CHAINED, get_key, compare_words);

    printf("Insertion and Search Example\n==================\n");
    wordcount *wc = create_count("uchicago");
    printf("created word count\n");
    printed_insert(ht1, (void *)wc);
    printf("printed insert succ\n");
    print_hash_table_entries(ht1);
    printf("start searching\n");
    printed_search(ht1, (void*)wc);
   
    
    //Uncomment the following line to do full tests, provide filename and 
    //hash_table size as arguments:
    do_full_test(argv[1], atoi(argv[2]));

    return 0;
}


void do_full_test(char *filename, int table_size)
{

    for(int type=0; type < 3; type++)
    {
        char *type_to_string[] = {"Linear", "Quadratic", "Chained"};
        
	printf("hereeeee\n");
        hash_table_t* ht = create_hash_table(table_size, type, get_key,
		       	compare_words);

        printf("==========================================================");
	printf("===========\n");
        printf("%s Test, Filename: %s, Hash Table Size: %d\n",
		       	type_to_string[type], filename, table_size);
        printf("==========================================================");
	printf("===========\n");
        float average_ops = insert_search_from_file(ht, filename);
        // float average_search = search_from_file(ht, filename);
        printf("----------------------------------------------------------");
	printf("-----------\n");
        printf("%s Average # of Operations: %f\n",
		       	type_to_string[type], average_ops);
        // printf("%s Average Search Operations: %f\n",
	// type_to_string[type], average_search);
        printf("----------------------------------------------------------");
	printf("-----------\n\n");
    }
}





