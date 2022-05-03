#include <morec/list.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void list_iterator_next(iterator_t* it)
{
    assert(it->position != NULL);

    list_t* list = it->structure_data;
    list_node_t* node = (list_node_t*)((uint8_t*)it->position - sizeof(list_node_t));
    it->position = node->next ? (uint8_t*)node->next + sizeof(list_node_t) : NULL;
}

static void list_iterator_prev(iterator_t* it)
{
    assert(it->position != NULL);
 
    list_t* list = it->structure_data;
    list_node_t* node = (list_node_t*)((uint8_t*)it->position - sizeof(list_node_t));
    it->position = node->prev ? (uint8_t*)node->prev + sizeof(list_node_t) : NULL;
}

list_t __list_create(size_t elem_size)
{
    list_t list = {
        .elem_size = elem_size,
        .head = NULL
    };
    return list;
}

void list_destroy(list_t* list)
{
    list_clear(list);
}

iterator_t list_begin(const list_t* list)
{
    void* ptr = (uint8_t*)list->head + sizeof(list_node_t);
    return iterator(list_iterator_next, list_iterator_prev, ptr, (void*)list);
}

iterator_t list_end(const list_t* list)
{
    void* ptr = (uint8_t*)list->tail + sizeof(list_node_t);
    return iterator(list_iterator_next, list_iterator_prev, ptr, (void*)list);
}

bool list_empty(const list_t* list)
{
    return list->head == NULL;
}

size_t list_size(const list_t* list)
{
    size_t size = 0;

    list_node_t* node = list->head;
    while (node != NULL)
    {
        size++;
        node = node->next;
    }

    return size;
}

void* __list_front(const list_t* list)
{
    assert(list->head != NULL);
    return list->head->value;
}

void* __list_back(const list_t* list)
{
    assert(list->tail != NULL);
    return list->tail->value;
}

void __list_push_front(list_t* list, const void* value_ptr)
{
    list_node_t* node = malloc(sizeof(list_node_t) + list->elem_size);
    memcpy(node->value, value_ptr, list->elem_size);

    if(list->head == NULL)
    {
        list->head = list->tail = node;
        node->next = node->prev = NULL;
    }
    else
    {
        node->next = list->head;
        list->head->prev = node;
        node->prev = NULL;
        list->head = node;
    }
}

void __list_push_back(list_t* list, const void* value_ptr)
{
    list_node_t* node = malloc(sizeof(list_node_t) + list->elem_size);
    memcpy(node->value, value_ptr, list->elem_size);

    if(list->head == NULL)
    {
        list->head = list->tail = node;
        node->next = node->prev = NULL;
    }
    else
    {
        node->prev = list->tail;
        list->tail->next = node;
        node->next = NULL;
        list->tail = node;
    }
}

void __list_pop_front(list_t* list)
{
    assert(list->head != NULL);

    list_node_t* node = list->head;
    list->head = node->next;

    if(list->head == NULL)
        list->tail = NULL;
    else
        list->head->prev = NULL;

    free(node);
}

void __list_pop_back(list_t* list)
{
    assert(list->tail != NULL);

    list_node_t* node = list->tail;
    list->tail = node->prev;

    if(list->tail == NULL)
        list->head = NULL;
    else
        list->tail->next = NULL;

    free(node);
}

void list_clear(list_t* list)
{
    list_node_t* node = list->tail;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        free(node);
        node = next;
    }
}

void __list_remove(list_t* list, void* value_ptr)
{
    list_node_t* node = list->head;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        if(memcmp((uint8_t*)node + sizeof(list_node_t), value_ptr, list->elem_size) == 0)
        {
            if(node->prev != NULL)
                node->prev->next = node->next;
            else
                list->head = node->next;

            if(node->next != NULL)
                node->next->prev = node->prev;
            else
                list->tail = node->prev;

            free(node);
        }

        node = next;
    }
}

void list_remove_if(list_t* list, bool (*predicate)(const void*))
{
    list_node_t* node = list->head;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        if(predicate(node->value))
        {
            if(node->prev != NULL)
                node->prev->next = node->next;
            else
                list->head = node->next;

            if(node->next != NULL)
                node->next->prev = node->prev;
            else
                list->tail = node->prev;

            free(node);
        }

        node = next;
    }
}

void list_reverse(list_t* list)
{
    list_node_t* node = list->head;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        node->next = node->prev;
        node->prev = next;
        node = next;
    }

    list_node_t* tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

void list_unique(list_t* list)
{
    list_node_t* node = list->head ? list->head->next : NULL;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        if(memcmp((uint8_t*)node + sizeof(list_node_t), (uint8_t*)node->prev + sizeof(list_node_t), list->elem_size) == 0)
        {
            if(node->prev != NULL)
                node->prev->next = node->next;
            else
                list->head = node->next;

            if(node->next != NULL)
                node->next->prev = node->prev;
            else
                list->tail = node->prev;

            free(node);
        }

        node = next;
    }
}

void list_unique_pred(list_t* list, bool (*predicate)(const void*, const void*))
{
    list_node_t* node = list->head ? list->head->next : NULL;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        if(predicate(node->value, node->prev->value))
        {
            if(node->prev != NULL)
                node->prev->next = node->next;
            else
                list->head = node->next;

            if(node->next != NULL)
                node->next->prev = node->prev;
            else
                list->tail = node->prev;

            free(node);
        }

        node = next;
    }
}
