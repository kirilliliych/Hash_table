#include "hash_funcs.hpp"

size_t ConstHash(elem_t *key)
{
    assert(key != nullptr);

    return 1;
}

size_t FirstAsciiHash(elem_t *key)
{
    assert(key != nullptr);

    return *key;
}

size_t StrlenAsciiHash(elem_t *key)
{
    assert(key != nullptr);

    elem_t *cur_symb = key;
    size_t sum = 0;

    while (*cur_symb != '\0')
    {
        sum += (size_t) (*cur_symb);

        ++cur_symb;
    }

    return sum;
}

size_t StrlenHash(elem_t *key)
{
    assert(key != nullptr);

    elem_t *cur_symb = key;
    while (*cur_symb != '\0')
    {
        ++cur_symb;
    }

    return cur_symb - key;
}

size_t RolHash(elem_t *key)
{
    assert(key != nullptr);

    elem_t *cur_symb = key;
    size_t hash = (size_t) *cur_symb;
    ++cur_symb;
    
    while (*cur_symb != '\0')
    {
        hash = ((hash << 1) + (hash >> (sizeof(size_t) * 8 - 1))) ^ ((size_t) *cur_symb);

        ++cur_symb;
    }

    return hash;
}

size_t Crc32Hash(elem_t *key)
{
    assert(key != nullptr);

    elem_t *symbol_ptr = key;
    const size_t polynom = 0x04C11DB7;
    const size_t old_bit_checker = 1 << 26;

    size_t hash = 0;

    while (*symbol_ptr != '\0')
    {
        for (int bit = 7; bit >= 0; --bit)                              // 8 bits in one byte
        {
            hash = (hash << 1) + ((*symbol_ptr >> bit) & 1);
            
            if (hash & old_bit_checker)
            {
                hash ^= polynom;
            }
        }
        ++symbol_ptr;
    }

    return hash;
}