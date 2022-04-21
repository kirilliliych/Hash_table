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

    size_t sum = 0;
    
    while (*key != '\0')
    {
        sum += (size_t) (*key);

        ++key;
    }

    return sum;
}

size_t StrlenHash(elem_t *key)
{
    assert(key != nullptr);

    return strlen(key);
}

size_t RolHash(elem_t *key)
{
    assert(key != nullptr);

    size_t hash = (size_t) *key;
    ++key;
    
    while (*key != '\0')
    {
        hash = ((hash << 1) + (hash >> ROL_SHR_SIZE)) ^ ((size_t) *key);

        ++key;
    }

    return hash;
}

size_t Crc32Hash(elem_t *key)
{
    assert(key != nullptr);

    size_t hash = 0;

    asm(
       ".intel_syntax noprefix\n\t"
       "mov rcx, 3\n\t"
       "crc32 %0, qword ptr [rdi]\n\t"
       ".decide_if_again:\n\t"
       "    add rdi, 8\n\t"
       "    cmp byte ptr [rdi], 0\n\t"
       "    je .the_end\n\t"
       "    crc32 %0, qword ptr [rdi]\n\t"
       "    loop .decide_if_again\n\t"
       ".the_end:"
       ".att_syntax prefix\n\t"
       : "=ra" (hash)
       : "rD" (key)
       : "rax", "rcx", "rdi"
    );

    return hash;
}