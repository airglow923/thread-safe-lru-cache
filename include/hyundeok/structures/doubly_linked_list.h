#ifndef HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H
#define HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H

#include <stddef.h>

typedef struct DoublyLinkedListNode {
  void* data_;
  struct DoublyLinkedListNode* prev_;
  struct DoublyLinkedListNode* next_;
} DllnT;

typedef struct DoublyLinkedList {
  void (*destructor_)(void*);
  DllnT* head_;
  DllnT* tail_;
} DoublyLinkedList;

/**
 * Creates a doubly linked list.
 *
 * @return List Doubly linked list
 */
DoublyLinkedList*
DoublyLinkedListNew(void);

/**
 * Creates a doubly linked list with a destructor.
 *
 * @param destructor Destructor for elements
 * @return List Doubly linked list
 */
DoublyLinkedList*
DoublyLinkedListNewFull(void (*destructor)(void*));

/**
 * Checks if a doubly linked list is empty.
 *
 * @param list Doubly linked list
 * @return Emptiness 1 if empty; 0 otherwise
 */
int
DoublyLinkedListIsEmpty(DoublyLinkedList* list);

/**
 * Returns data at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
void*
DoublyLinkedListFront(DoublyLinkedList* list);

/**
 * Returns data at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
void*
DoublyLinkedListBack(DoublyLinkedList* list);

/**
 * Inserts an element to the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Data to be inserted
 * @return Inserted node if success; otherwise, NULL
 */
DllnT*
DoublyLinkedListPrepend(DoublyLinkedList* list, void* data);

/**
 * Inserts an element to the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Data to be inserted
 * @return Inserted node if success; otherwise, NULL
 */
DllnT*
DoublyLinkedListAppend(DoublyLinkedList* list, void* data);

/**
 * Prepend a node to a doubly linked list.
 *
 * The leftmost node must be NULL.
 *
 * @param list Doubly linked list
 * @param node Node to be prepended
 * @return New head of a list
 */
DllnT*
DoublyLinkedListPrependNode(DoublyLinkedList* list, DllnT* node);

/**
 * Append a node to a doubly linked list.
 *
 * The node should be present on the list. If not, the behaviour is undefined.
 * The leftmost node must be NULL.
 *
 * @param list Doubly linked list
 * @param node Node to be appended
 * @return New tail of a list
 */
DllnT*
DoublyLinkedListAppendNode(DoublyLinkedList* list, DllnT* node);

/**
 * Removes node and decrements the size of a list.
 *
 * The node should be present on the list. If not, the behaviour is undefined.
 * The rightmost node must be NULL.
 *
 * @param list Doubly linked list
 * @param node Doubly linked list node
 * @param free_node Indicates whether to deallocate removed node
 */
void
DoublyLinkedListRemove(DoublyLinkedList* list, DllnT* node, int free_node);

/**
 * Deletes an element at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 */
void
DoublyLinkedListRemoveFront(DoublyLinkedList* list);

/**
 * Deletes an element at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 */
void
DoublyLinkedListRemoveBack(DoublyLinkedList* list);

/**
 * Deallocates a doubly linked list.
 *
 * @param list Doubly linked list
 */
void
DoublyLinkedListDestroy(DoublyLinkedList* list);

#endif
