#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.hpp"
#include "hash_funcs.hpp"

#define aboba printf("aboba %d\n", __LINE__);

typedef size_t (*functor)(elem_t *key);

const size_t MAX_SCANFED_WORD_LENGTH      = 100;
const size_t MAX_WORD_LENGTH              = 32;

const size_t HASH_TABLE_CORRECT_SIZE      = (1 << 19) - 1;

const size_t TIMES_SEARCH                 = 100;             

const size_t HASH_FUNCS_QUANTITY          = 6;

const char *DELIM                         = " .,)!;\"\n";

const char *FILL_HASH_TABLE_FILE          = "War_and_Peace.txt";
const char *SEEK_HASH_TABLE_FILE          = "dictionary";

const char *LIST_LENGTHS_OUTPUT_FILE_NAME = "Crc32Hash";


enum InputCheck
{
    OK = 0,
    INPUT_ERROR
};

struct hash_function
{
    functor hash_func          = nullptr;
    const char *hash_func_name = nullptr;
};

struct text
{
    size_t file_size      = 0;
    size_t words_quantity = 0;

    char *buffer          = nullptr;
    char **words_array    = nullptr;
    
    const char *file_name = nullptr;
    FILE *file_ptr        = nullptr;
};

struct hash_table 
{
    size_t hash_funcs_quantity  = 0;
    size_t used_hash_func_index = 0; 

    size_t capacity             = 0;    
    size_t size                 = 0;
    list **data                 = nullptr;

    hash_function *hash_funcs   = nullptr;
};


size_t get_file_size(FILE *file);

char *get_text_from_file(text *input_text);

size_t count_words_quantity(text *input_text);

char **do_words_array(text *input_text);

InputCheck text_input(text *input_text);

void free_memory(text *input_text);

size_t hash_table_get_index(hash_table *ht, elem_t *key);

hash_table *hash_table_new(size_t capacity);

void hash_table_insert(hash_table *ht, elem_t *key);

list *hash_table_find(hash_table *ht, elem_t *key);

void hash_table_delete(hash_table *ht, elem_t *key);

size_t hash_table_size(hash_table *ht);

hash_table *hash_table_destroy(hash_table *ht);

void hash_table_dump(hash_table *ht);

void hash_table_get_list_lengths(hash_table *ht);



