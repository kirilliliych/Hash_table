#ifndef HASH_FUNCS_HPP
#define HASH_FUNCS_HPP

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "list.hpp"

extern "C" size_t RolHash_asm(elem_t *key);

const size_t ROL_SHR_SIZE = sizeof(size_t) * 8 - 1;

size_t ConstHash(elem_t *key);

size_t FirstAsciiHash(elem_t *key);

size_t StrlenAsciiHash(elem_t *key);

size_t StrlenHash(elem_t *key);

size_t RolHash(elem_t *key);

size_t Crc32Hash(elem_t *key);

#endif