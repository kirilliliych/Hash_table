#ifndef LIST_HPP
#define LIST_HPP

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char elem_t;

struct list
{
    elem_t *value;
    list   *next;
};

list *list_new(elem_t *elem);

list *list_insert(list *head, elem_t *elem);

list *list_find(list *head, elem_t *elem);

list *list_erase(list *head, elem_t *elem);

list *list_insert_after(list *head,  list *where, list *what);

list *list_insert_before(list *head, list *where, list *what);

list *list_delete(list *head);

list *list_next(list *curr);

void list_print(list const *head);

#endif
