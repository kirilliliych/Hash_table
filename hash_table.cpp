#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.hpp"
#include "hash_funcs.hpp"

#define aboba printf("aboba %d\n", __LINE__);

const size_t HASH_TABLE_CORRECT_SIZE = (1 << 10) - 5;
const size_t HASH_TABLE_WRONG_SIZE   =  1 << 10;

enum InputCheck
{
    OK = 0,
    INPUT_ERROR
};

struct word
{
    size_t length = 0;
    const char *word = nullptr;    
};

struct text
{
    size_t file_size      = 0;
    size_t words_quantity = 0;

    char *buffer          = nullptr;
    word *words_array     = nullptr;
    
    const char *file_name = nullptr;
    FILE *file_ptr        = nullptr;
};

struct hash_table 
{
    size_t capacity = 0;    
    size_t size     = 0;
    list **data     = nullptr;
};

size_t GetFileSize(FILE *file)
{
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

char *GetTextFromFile(text *input_text)
{
    assert(input_text != nullptr);

    input_text->file_size = GetFileSize(input_text->file_ptr) + 2;             // +2 to get sure that there will be place for final '\n' and '\0'                                       

    char *buffer = (char *) calloc(input_text->file_size, sizeof(char));

    input_text->file_size = fread(buffer, sizeof(char), input_text->file_size, input_text->file_ptr) + 2;

    if (buffer[input_text->file_size - 3] != '\n')
    {
        buffer[input_text->file_size - 2] = '\n';                              // if there is no '\n' at the end
        buffer[input_text->file_size - 1] = '\0';                              // EOF marker
    }
    
    buffer[input_text->file_size - 3] = '\n';
    buffer[input_text->file_size - 2] = '\0';

    return buffer;
}

size_t CountWordsQuantity(text *input_text)
{
    assert(input_text != nullptr);

    size_t words_quantity = 0;
    const char *symbol_ptr = input_text->buffer;
    while (*symbol_ptr != '\0')
    {
        if ((*symbol_ptr == '\n') || (*symbol_ptr == ' '))
        {
            ++words_quantity;
        }

        ++symbol_ptr;
    }

    return words_quantity;
}

word *DoWordsArray(text *input_text)
{
    assert(input_text != nullptr);

    word *words_array = (word *) calloc(input_text->words_quantity, sizeof(word));

    size_t word_index = 0;

    const char *new_lexem = strtok(input_text->buffer, " .,)!;\"\n");

    while (new_lexem != nullptr)
    {
        words_array[word_index].word   = new_lexem;
        words_array[word_index].length = strlen(new_lexem);
        
        ++word_index;

        new_lexem = strtok(nullptr, " .,)!;\"\n");
    }
    
    return words_array;
}

InputCheck TextInput(text *input_text)
{
    assert(input_text != nullptr);

    input_text->file_name = "War_and_Peace.txt";

    input_text->file_ptr = fopen(input_text->file_name, "r");
    
    if (input_text->file_ptr == nullptr)
    {
        printf("ERROR int TextInput(): file was not found");
        return InputCheck::INPUT_ERROR;
    }

    input_text->buffer         = GetTextFromFile(input_text);
    input_text->words_quantity = CountWordsQuantity(input_text); 
    input_text->words_array    = DoWordsArray(input_text);

    fclose(input_text->file_ptr);

    return InputCheck::OK;
}

void FreeMemory(text *input_text) 
{
    assert(input_text != nullptr);

    free(input_text->buffer);
    free(input_text->words_array);

    input_text->buffer      = nullptr;
    input_text->words_array = nullptr;
}

hash_table *hash_table_new(size_t capacity)
{
    hash_table *new_ht = (hash_table *) calloc(1, sizeof(hash_table));
    assert(new_ht != nullptr);

    new_ht->data = (list **) calloc(capacity, sizeof(list *));
    assert(new_ht->data != nullptr);

    new_ht->capacity = capacity;

    return new_ht;
}

void hash_table_insert(hash_table *ht, elem_t *key)
{
    assert(ht != nullptr);
    
    //printf("ht->size: %lu\n", ht->size);
    size_t index = Crc32Hash(key) % ht->capacity;
    
    list *elem_place = list_find(ht->data[index], key);
    if (elem_place == nullptr)
    { 
        ++ht->size;
        ht->data[index] = list_insert(ht->data[index], key);

        return;
    }
    
    elem_place->value = key;
}

list *hash_table_find(hash_table *ht, elem_t *key)
{
    assert(ht != nullptr);

    size_t index = Crc32Hash(key) % ht->capacity;

    return list_find(ht->data[index], key);
}

void hash_table_delete(hash_table *ht, elem_t *key)
{
    assert(ht != nullptr);

    size_t index = Crc32Hash(key) % ht->capacity;

    if (list_find(ht->data[index], key) == nullptr)
    {
        return;
    }

    ht->data[index] = list_erase(ht->data[index], key);
    --ht->size;
}

size_t hash_table_size(hash_table *ht)
{
    assert(ht != nullptr);

    return ht->size;
}

hash_table *hash_table_destroy(hash_table *ht)
{
    assert(ht != nullptr);

    for (size_t list_index = 0; list_index < ht->capacity; ++list_index)
    {
        ht->data[list_index] = list_delete(ht->data[list_index]);
    }
    free(ht->data);
    free(ht);

    return nullptr;
}

void hash_table_dump(hash_table *ht)
{
    assert(ht != nullptr);

    printf("hash_table_mem_address: %p\n", ht);

    for (size_t list_index = 0; list_index < ht->capacity; ++list_index)
    {
        if (ht->data[list_index] != nullptr)
        {
            printf("\tht->data[%lu]: ", list_index);    
            list_print(ht->data[list_index]);
        }
    }

    printf("\n");
}

void hash_table_get_list_lengths(hash_table *ht)
{
    assert(ht != nullptr);

    FILE *file_with_lengths = fopen("lengths.txt", "w+");

    for (size_t list_index = 0; list_index < ht->capacity; ++list_index)
    {
        int list_length = 0;

        list *cur_list = ht->data[list_index];
        if (ht->data[list_index] != nullptr)
        {
            list_length = 1;

            while (cur_list != nullptr)
            {
                cur_list = cur_list->next;
                ++list_length;
            }
        }

        fprintf(file_with_lengths, "list %lu: %d\n", list_index, list_length);
    }

    fclose(file_with_lengths);
}

int main()
{
    hash_table *ht = hash_table_new(HASH_TABLE_CORRECT_SIZE);

    text input_text = {};
    InputCheck input_result = TextInput(&input_text);
    if (input_result != OK)
    {
        return input_result;
    }

    for (size_t word_index = 0; word_index < input_text.words_quantity; ++word_index)
    {
        if (input_text.words_array[word_index].word != nullptr)
        {
            hash_table_insert(ht, input_text.words_array[word_index].word);
            //printf("word_index: %lu\n", word_index);
        }
    }

    //hash_table_dump(ht);

    hash_table_get_list_lengths(ht);

    ht = hash_table_destroy(ht);
}