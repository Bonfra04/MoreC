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

iterator_t list_rbegin(const list_t* list)
{
    void* ptr = (uint8_t*)list->tail + sizeof(list_node_t);
    return iterator(list_iterator_prev, list_iterator_next, ptr, (void*)list);
}

iterator_t list_rend(const list_t* list)
{
    void* ptr = (uint8_t*)list->head + sizeof(list_node_t);
    return iterator(list_iterator_prev, list_iterator_next, ptr, (void*)list);
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

void list_swap(list_t* list1, list_t* list2)
{
    assert(list1->elem_size == list2->elem_size);

    list_node_t* tmp = list1->head;
    list1->head = list2->head;
    list2->head = tmp;

    tmp = list1->tail;
    list1->tail = list2->tail;
    list2->tail = tmp;
}

void list_assign_range(list_t* list, iterator_t first, iterator_t last)
{
    list_clear(list);

    while(iterator_distance(&first, &last) != 0)
    {
        __list_push_back(list, __iterator_get(&first));
        iterator_advance(&first, 1);
    }
}

void __list_assign_fill(list_t* list, size_t n, const void* val_ptr)
{
    list_clear(list);

    for(size_t i = 0; i < n; i++)
        __list_push_back(list, val_ptr);
}

void list_erase(list_t* list, iterator_t position)
{
    assert(position.structure_data == list);

    list_node_t* node = (uint8_t*)__iterator_get(&position) - sizeof(list_node_t);
    list_node_t* prev = node->prev;
    list_node_t* next = node->next;

    if(prev != NULL)
        prev->next = next;
    else
        list->head = next;

    if(next != NULL)
        next->prev = prev;
    else
        list->tail = prev;

    free((uint8_t*)__iterator_get(&position) - sizeof(list_node_t));
}

void list_erase_range(list_t* list, iterator_t first, iterator_t last)
{
    assert(first.structure_data == list);
    assert(last.structure_data == list);

    while(iterator_distance(&first, &last) != 0)
    {
        list_node_t* node = (uint8_t*)__iterator_get(&first) - sizeof(list_node_t);
        list_node_t* prev = node->prev;
        list_node_t* next = node->next;

        if(prev != NULL)
            prev->next = next;
        else
            list->head = next;

        if(next != NULL)
            next->prev = prev;
        else
            list->tail = prev;

        free(node);
        iterator_advance(&first, 1);
    }
}

void __list_insert_single(list_t* list, iterator_t position, const void* val_ptr)
{
    assert(position.structure_data == list);

    list_node_t* new_node = malloc(sizeof(list_node_t) + list->elem_size);
    memcpy((uint8_t*)new_node + sizeof(list_node_t), val_ptr, list->elem_size);

    list_node_t* node = (uint8_t*)__iterator_get(&position) - sizeof(list_node_t);
    list_node_t* prev = node->prev;
    list_node_t* next = node->next;

    new_node->prev = prev;
    new_node->next = next;

    if(prev != NULL)
        prev->next = new_node;
    else
        list->head = new_node;

    if(next != NULL)
        next->prev = new_node;
    else
        list->tail = new_node;
}

void __list_insert_fill(list_t* list, iterator_t position, size_t n, const void* val_ptr)
{
    assert(position.structure_data == list);

    while(n--)
        __list_insert_single(list, position, val_ptr);
}

void list_insert_range(list_t* list, iterator_t position, iterator_t first, iterator_t last)
{
    assert(position.structure_data == list);

    while(iterator_distance(&first, &last) != 0)
    {
        __list_insert_single(list, position, __iterator_get(&first));
        iterator_advance(&first, 1);
    }
}

void list_resize(list_t* list, size_t length)
{
    if(length == 0)
    {
        list_clear(list);
        return;
    }

    if(length < list_size(list))
    {
        iterator_t it = list_end(list);
        iterator_advance(&it, -(int64_t)(list_size(list) - length));
        list_erase_range(list, list_begin(list), it);
    }
    else if (length > list_size(list))
    {
        iterator_t it = list_end(list);
        iterator_advance(&it, -(int64_t)(list_size(list) - length));
        list_insert_fill(list, it, length - list_size(list), 0);
    }
}

void list_splice_entire(list_t* list, iterator_t position, list_t* source)
{
    assert(position.structure_data == list);
    assert(source->elem_size == list->elem_size);

    list_node_t* node = (uint8_t*)__iterator_get(&position) - sizeof(list_node_t);
    list_node_t* prev = node->prev;
    list_node_t* next = node->next;

    if(prev != NULL)
        prev->next = source->head;
    else
        list->head = source->head;

    if(next != NULL)
        next->prev = source->tail;
    else
        list->tail = source->tail;

    source->head = NULL;
    source->tail = NULL;
}

void list_splice_single(list_t* list, iterator_t position, list_t* source, iterator_t it)
{
    assert(position.structure_data == list);
    assert(source->elem_size == list->elem_size);
    assert(it.structure_data == source);

    list_node_t* node = (uint8_t*)__iterator_get(&position) - sizeof(list_node_t);
    list_node_t* prev = node->prev;
    list_node_t* next = node->next;

    list_node_t* node_it = (uint8_t*)__iterator_get(&it) - sizeof(list_node_t);
    list_node_t* prev_it = node_it->prev;
    list_node_t* next_it = node_it->next;

    if(prev != NULL)
        prev->next = next_it;
    else
        list->head = next_it;

    if(next != NULL)
        next->prev = prev_it;
    else
        list->tail = prev_it;

    if(prev_it != NULL)
        prev_it->next = next;
    else
        source->head = next;

    if(next_it != NULL)
        next_it->prev = prev;
    else
        source->tail = prev;
}

void list_splice_range(list_t* list, iterator_t position, iterator_t first, iterator_t last)
{
    assert(position.structure_data == list);
    assert(first.structure_data == list);
    assert(last.structure_data == list);

    while(iterator_distance(&first, &last) != 0)
    {
        list_splice_single(list, position, first.structure_data, first);
        iterator_advance(&first, 1);
    }
}
