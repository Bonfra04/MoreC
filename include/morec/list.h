#pragma once

#include <morec/iterator.h>

#include <stddef.h>
#include <stdbool.h>

typedef struct list_node list_node_t;
typedef struct list_node
{
    list_node_t* prev;
    list_node_t* next;
    void* value[];
} list_node_t;

typedef struct list
{
    size_t elem_size;
    list_node_t* head;
    list_node_t* tail;
} list_t;

/**
 * @brief creates a list
 * @param[in] elem_size size in byte of each element
 * @return the list
 */
list_t __list_create(size_t elem_size);
#define list(type) __list_create(sizeof(type))

/**
 * @brief destroys a list
 * @param[in] list list to destroy
 */
void list_destroy(list_t* list);

/**
 * @brief returns an iterator to the beginning of the list
 * @param[in] list list to iterate
 * @return iterator to the beginning of the list
 */
iterator_t list_begin(const list_t* list);

/**
 * @brief returns an iterator to the end of the list
 * @param[in] list list to iterate
 * @return iterator to the end of the list
 */
iterator_t list_end(const list_t* list);

/**
 * @brief returns a reverse iterator to the beginning of the list
 * @param[in] list list to iterate
 * @return reverse iterator to the beginning of the list
 */
iterator_t list_rbegin(const list_t* list);

/**
 * @brief returns a reverse iterator to the end of the list
 * @param[in] list list to iterate
 * @return reverse iterator to the end of the list
 */
iterator_t list_rend(const list_t* list);

/**
 * @brief checks if the list is empty
 * @param[in] list list to check
 * @return true if the list is empty, false otherwise
 */
bool list_empty(const list_t* list);

/**
 * @brief returns the number of elements in a list
 * @param[in] list list to get the size from
 * @return number of elements
 */
size_t list_size(const list_t* list);

/**
 * @brief returns a pointer to the first element in the list
 * @param[in] list list to get the first element from
 * @return pointer to the first element in the list
 */
void* __list_front(const list_t* list);
#define list_front(type, list) (*(type*)__list_front(list))

/**
 * @brief returns a pointer to the last element in the list
 * @param[in] list list to get the last element from
 * @return pointer to the last element in the list
 */
void* __list_back(const list_t* list);
#define list_back(type, list) (*(type*)__list_back(list))

/**
 * @brief inserts an element at the beginning of the list
 * @param[in] list list to insert the element into
 * @param[in] value_ptr pointer to the value to insert
 */
void __list_push_front(list_t* list, const void* value_ptr);
#define list_push_front(list, value) { typeof(value) __tmp = value; __list_push_front(list, &__tmp); }

/**
 * @brief inserts an element at the end of the list
 * @param[in] list list to insert the element into
 * @param[in] value_ptr pointer to the value to insert
 */
void __list_push_back(list_t* list, const void* value_ptr);
#define list_push_back(list, value) { typeof(value) __tmp = value; __list_push_back(list, &__tmp); }

/**
 * @brief removes the first element from the list
 * @param[in] list list to remove the element from
 */
void __list_pop_front(list_t* list);
#define list_pop_front(list) __list_pop_front(list)

/**
 * @brief removes the last element from the list
 * @param[in] list list to remove the element from
 */
void __list_pop_back(list_t* list);
#define list_pop_back(list) __list_pop_back(list)

/**
 * @brief removes all elements from the list
 * @param[in] list list to clear
 */
void list_clear(list_t* list);

/**
 * @brief deletes all elements from the list that match the provided value
 * @param[in] list list to delete the element from
 * @param[in] value_ptr pointer to the value to delete (must be the same as the value in the list)
 */
void __list_remove(list_t* list, void* value_ptr);
#define list_remove(list, value) { typeof(value) __tmp = value; __list_remove(list, &__tmp); }

/**
 * @brief removes all elements that fulfill the provided condition
 * @param[in] list list to delete the elements from
 * @param[in] predicate function that returns true if the element should be deleted
 */
void list_remove_if(list_t* list, bool (*predicate)(const void*));

/**
 * @brief reverses the order of the elements in the list
 * @param[in] list list to reverse
 */
void list_reverse(list_t* list);

/**
 * @brief removes all consecutive duplicate elements from the list leaving ony one of each
 * @param[in] list list to remove the duplicates from
 */
void list_unique(list_t* list);

/**
 * @brief removes all consecutive duplicate elements from the list leaving ony one of each
 * @param[in] list list to remove the duplicates from
 * @param[in] predicate function that returns true if the two elements are equal
 */
void list_unique_pred(list_t* list, bool (*predicate)(const void*, const void*));

/**
 * @brief swaps the contents of two lists
 * @param[in] list1 first list to swap
 * @param[in] list2 second list to swap
 */
void list_swap(list_t* list1, list_t* list2);

/**
 * @brief assigns new contents to the list, replacing its current contents, and modifying its size accordingly
 * @param[in] list list to assign new contents to
 * @param[in] begin iterator to the beginning of the new contents
 * @param[in] end iterator to the end of the new contents
 */
void list_assign_range(list_t* list, iterator_t first, iterator_t last);

/**
 * @brief assigns new contents to the list, replacing its current contents, and modifying its size accordingly
 * @param[in] list list to assign new contents to
 * @param[in] n number of elements to assign
 * @param[in] val_ptr pointer to the value to assign
 */
void __list_assign_fill(list_t* list, size_t n, const void* val_ptr);
#define list_assign_fill(list, n, value) { typeof(value) __tmp = value; __list_assign_fill(list, n, &__tmp); }

/**
 * @brief removes an element from a list
 * @param[in] list list to remove the element from
 * @param[in] position iterator to the element to remove
 */
void list_erase(list_t* list, iterator_t position);

/**
 * @brief removes a range of elements from a list
 * @param[in] list list to remove the elements from
 * @param[in] first iterator to the first element in the range to remove
 * @param[in] last iterator to the end of the range to remove
 */
void list_erase_range(list_t* list, iterator_t first, iterator_t last);

/**
 * @brief extends the list inserting a new element at the specified position
 * @param[in] list list to extend
 * @param[in] position iterator to the position to insert the new element
 * @param[in] val_ptr a pointer to the value to insert
 */
void __list_insert_single(list_t* list, iterator_t position, const void* val_ptr);
#define list_insert_single(list, position, value) { typeof(value) __tmp = value; __list_insert_single(list, position, &__tmp); }

/**
 * @brief extends the list inserting n new elements at the specified position
 * @param[in] list list to extend
 * @param[in] position iterator to the position to insert the new element
 * @param[in] n number of elements to insert
 * @param[in] val_ptr a pointer to the value to insert
 */
void __list_insert_fill(list_t* list, iterator_t position, size_t n, const void* val_ptr);
#define list_insert_fill(list, position, n, value) { typeof(value) __tmp = value; __list_insert_fill(list, position, n, &__tmp); }

/**
 * @brief extends the list inserting a range of elements at the specified position
 * @param[in] list list to extend
 * @param[in] position iterator to the position to insert the new element
 * @param[in] first iterator to the first element in the range to insert
 * @param[in] last iterator to the end of the range to insert
 */
void list_insert_range(list_t* list, iterator_t position, iterator_t first, iterator_t last);

/**
 * @brief resizes a list so that it contains n elements
 * @param[in] list list to resize
 * @param[in] length new length
 */
void list_resize(list_t* list, size_t length);

/**
 * @brief transfers the contents of the source list to the destination list at the specified position
 * @param[in] list list to transfer the contents to
 * @param[in] position iterator to the position to insert the new elements
 * @param[in] source source list to transfer the contents from
 */
void list_splice_entire(list_t* list, iterator_t position, list_t* source);

/**
 * @brief transfers the contents of the source list to the destination list at the specified position
 * @param[in] list list to transfer the contents to
 * @param[in] position iterator to the position to insert the new elements
 * @param[in] source source list to transfer the contents from
 * @param[in] it iterator to the single element to transfer
 */
void list_splice_single(list_t* list, iterator_t position, list_t* source, iterator_t it);

/**
 * @brief transfers the contents of the source list to the destination list at the specified position
 * @param[in] list list to transfer the contents to
 * @param[in] position iterator to the position to insert the new elements
 * @param[in] first iterator to the first element in the range to transfer
 * @param[in] last iterator to the end of the range to transfer
 */
void list_splice_range(list_t* list, iterator_t position, iterator_t first, iterator_t last);

