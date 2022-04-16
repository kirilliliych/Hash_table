#include "hash_funcs.hpp"

size_t Crc32Hash(elem_t *key)
{
    assert(key != nullptr);

    elem_t *symbol_ptr = key;
    const size_t polynom = 0x04C11DB7;
    const size_t old_bit_checker = 1 << 26;

    size_t hash = 0x0;

    while (*symbol_ptr != '\0')
    {
        for (int bit = 7; bit >= 0; --bit)
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