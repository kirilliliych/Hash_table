#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.hpp"
#include "hash_funcs.cpp"

struct hash_table 
{
    size_t capacity;
    size_t size;
    list **data;
};