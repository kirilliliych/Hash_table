#include "list.hpp"

list *list_new(elem_t *elem)
{
    list *node = (list *) calloc(1, sizeof(list));

    node->value =  elem;
    node->next  = nullptr;

    return node;
}

list *list_insert(list *head, elem_t *elem)
{
    if (head == nullptr)
    {
        head = list_new(elem);
        return head;    
    }

    list *temp = head;
    
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }

    temp->next = list_new(elem);

    return head;      
}

list *list_find(list *head, elem_t *elem)
{
    if (head == nullptr)
    {
        return nullptr;
    }

    list *temp = head;
    
    while (strcmp(elem, temp->value) != 0)
    {
        if (temp->next == nullptr)
        {
            return nullptr;
        }

        temp = temp->next;
    }

    return temp;    
}

list *list_erase(list *head, elem_t *elem)
{
    if (head == nullptr)
    {
        return head;
    }

    list *temp_prev = nullptr;
    list *temp = head;

    while (strcmp(elem, temp->value) != 0)
    {
        if (temp->next == nullptr)
        {
            return head;
        }

        temp_prev = temp;
        temp = temp->next;
    }

    if (temp_prev == nullptr)
    {   
        list *tmp = head; 
        head = head->next;
        free(tmp);
        
        return head;
    }

    if (temp->next == nullptr)
    {
        free(temp);
        temp_prev->next = nullptr;

        return head;
    }

    temp_prev->next = temp->next;
    free(temp);

    return head;
}

list *list_insert_after(list *head, list *where, list *what)
{   
    if ((where == nullptr) || (what == nullptr))
    {
        return head;
    }

    if (head == nullptr)
    {
        head = list_new(what->value);
        
        return head;
    }
    
    list *place = list_find(head, where->value);
    if (place == nullptr)
    {
        return head;
    }

    if (place->next == nullptr)
    {
        place->next = what;
        what->next  = nullptr;
        
        return head;
    }

    what->next  = place->next;
    place->next = what;

    return head;
}

list *list_insert_before(list *head, list *where, list *what)
{
    if (head == nullptr)
    {
        head = list_new(what->value);

        return head;
    }

    list *place = list_find(head, where->value);
    if (place == nullptr)
    {
        return head;
    }

    list *temp = head;
    if (place == head)
    {
        what->next = head;
        
        return what;
    }

    while (temp->next != place)
    {
        temp = temp->next;
    }

    temp->next = what;
    what->next = place;

    return head;
}

list *list_delete(list *head)
{
    while (head != nullptr)
    {
        list *temp = head;
        head = head->next;
        free(temp);
    }

    return nullptr;
}

list *list_next(list *curr)
{
    if (curr == nullptr)
    {
        return nullptr;
    }

    return curr->next;
}

void list_print(list const *head)
{
    printf("[");

    if (head != nullptr)
    {
        list const *temp = head;

        while (temp->next != nullptr)
        {
            printf("%s, ", temp->value);
            temp = temp->next;
        }

        printf("%s", temp->value);
    }
    printf("]\n");
}