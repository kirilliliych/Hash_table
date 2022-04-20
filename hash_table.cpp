#include "hash_table.hpp"

size_t get_file_size(FILE *file)
{
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

char *get_text_from_file(text *input_text)
{
    assert(input_text != nullptr);

    input_text->file_size = get_file_size(input_text->file_ptr) + 1;             // +1 to get sure that there will be place for '\0'                                       

    char *buffer = (char *) calloc(input_text->file_size, sizeof(char));

    input_text->file_size = fread(buffer, sizeof(char), input_text->file_size, input_text->file_ptr) + 1;

    return buffer;
}

size_t count_words_quantity(text *input_text)
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

char **do_words_array(text *input_text)
{
    assert(input_text != nullptr);

    char **words_array = (char **) calloc(input_text->words_quantity, sizeof(char *));

    size_t word_index = 0;

    const char *new_lexem = strtok(input_text->buffer, DELIM);

    while (new_lexem != nullptr)
    {
        words_array[word_index] = (char *) aligned_alloc(MAX_WORD_LENGTH, 
                                                        (MAX_WORD_LENGTH + 1) * sizeof(char));
        strncpy(words_array[word_index], new_lexem, MAX_WORD_LENGTH);

        ++word_index;

        new_lexem = strtok(nullptr, DELIM);
    }
    
    input_text->words_quantity = word_index;

    return words_array;
}

InputCheck text_input(text *input_text)
{
    assert(input_text != nullptr);

    input_text->file_name = FILL_HASH_TABLE_FILE;
    input_text->file_ptr = fopen(input_text->file_name, "r");
    
    if (input_text->file_ptr == nullptr)
    {
        printf("ERROR int text_input(): file was not found");
        return InputCheck::INPUT_ERROR;
    }

    input_text->buffer         = get_text_from_file(input_text);
    input_text->words_quantity = count_words_quantity(input_text); 
    input_text->words_array    = do_words_array(input_text);

    fclose(input_text->file_ptr);

    return InputCheck::OK;
}

void free_memory(text *input_text) 
{
    assert(input_text != nullptr);

    for (size_t word_index = 0; word_index < input_text->words_quantity; ++word_index)
    {
        free(input_text->words_array[word_index]);
        input_text->words_array[word_index] = nullptr;
    }
    free(input_text->words_array);

    free(input_text->buffer);

    input_text->buffer      = nullptr;
    input_text->words_array = nullptr;
}

size_t hash_table_get_index(hash_table *ht, elem_t *key)
{
    assert(ht  != nullptr);
    assert(key != nullptr);

    return (ht->hash_funcs[ht->used_hash_func_index].hash_func)(key) % ht->capacity;
}

hash_table *hash_table_new(size_t capacity, size_t hash_func_index)
{
    hash_table *new_ht = (hash_table *) calloc(1, sizeof(hash_table));
    assert(new_ht != nullptr);

    new_ht->data = (list **) calloc(capacity, sizeof(list *));
    assert(new_ht->data != nullptr);

    new_ht->capacity = capacity;

    new_ht->hash_funcs_quantity = HASH_FUNCS_QUANTITY;
    new_ht->hash_funcs = (hash_function *) calloc(new_ht->hash_funcs_quantity, sizeof(hash_function));

    new_ht->hash_funcs[0].hash_func = ConstHash;
    new_ht->hash_funcs[1].hash_func = FirstAsciiHash;
    new_ht->hash_funcs[2].hash_func = StrlenHash;
    new_ht->hash_funcs[3].hash_func = StrlenAsciiHash;
    new_ht->hash_funcs[4].hash_func = RolHash;
    new_ht->hash_funcs[5].hash_func = Crc32Hash;

    new_ht->hash_funcs[0].hash_func_name = "ConstHash";
    new_ht->hash_funcs[1].hash_func_name = "FirstAsciiHash";
    new_ht->hash_funcs[2].hash_func_name = "StrlenAsciiHash";
    new_ht->hash_funcs[3].hash_func_name = "StrlenHash";
    new_ht->hash_funcs[4].hash_func_name = "RolHash";
    new_ht->hash_funcs[5].hash_func_name = "Crc32Hash";

    new_ht->used_hash_func_index = hash_func_index;

    return new_ht;
}

void hash_table_insert(hash_table *ht, elem_t *key)
{
    assert(ht != nullptr);
    
    size_t index = hash_table_get_index(ht, key);
    
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

    size_t index = hash_table_get_index(ht, key);

    return list_find(ht->data[index], key);
}

void hash_table_delete(hash_table *ht, elem_t *key)
{
    assert(ht != nullptr);

    size_t index = hash_table_get_index(ht, key);

    list *place = list_find(ht->data[index], key);
    if (place == nullptr)
    {
        return;
    }

    ht->data[index] = list_erase(place, key);
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

    free(ht->hash_funcs);

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

    FILE *file_with_lengths = fopen(LIST_LENGTHS_OUTPUT_FILE_NAME, "w+");

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

        fprintf(file_with_lengths, "list %lu %d\n", list_index, list_length);
    }

    fclose(file_with_lengths);
}

int main()
{       
    hash_table *ht = hash_table_new(HASH_TABLE_CORRECT_SIZE, 5);

    text input_text = {};
    InputCheck input_result = text_input(&input_text);
    if (input_result != OK)
    {
        return input_result;
    }
    
    for (size_t word_index = 0; word_index < input_text.words_quantity; ++word_index)
    {
        if (input_text.words_array[word_index] != nullptr)
        {
            hash_table_insert(ht, input_text.words_array[word_index]);
        }
    }

    FILE *search_from = fopen(SEEK_HASH_TABLE_FILE, "r");
    if (search_from == nullptr)
    {
        printf("File search from was not found\n");
        
        return 1;
    }

    clock_t start = clock();

    char new_word[MAX_SCANFED_WORD_LENGTH] = {};
    while (fscanf(search_from, "%s", new_word) != EOF)
    {
        for (size_t searching_iter = 0; searching_iter < TIMES_SEARCH; ++searching_iter)
        {
            hash_table_find(ht, new_word);
        }
    }

    clock_t end = clock();

    printf("Took %Lf seconds\n", ((long double) (end - start)) / CLOCKS_PER_SEC);

    fclose(search_from);
    
    free_memory(&input_text);
    ht = hash_table_destroy(ht);

    return 0;
}