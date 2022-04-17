#ifndef HASH_FUNCS_HPP
#define HASH_FUNCS_HPP

#include <assert.h>
#include <stdio.h>
#include "list.hpp"

size_t ConstHash(elem_t *key);

size_t FirstAsciiHash(elem_t *key);

size_t StrlenAsciiHash(elem_t *key);

size_t StrlenHash(elem_t *key);

size_t RolHash(elem_t *key);

size_t Crc32Hash(elem_t *key);

#endif