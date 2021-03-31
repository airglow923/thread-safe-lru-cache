#ifndef HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H
#define HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H

#include "hyundeok/types.h"

// this is used for internal implementation
// it should not be accessed by end users
typedef struct DoublyLinkedListNode {
  void* data_;
  struct DoublyLinkedListNode* prev_;
  struct DoublyLinkedListNode* next_;
} DllnT;

typedef struct DoublyLinkedList {
  size_t size_;
  void (*desetructor_)(void*);
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
 * Returns the size of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Size
 */
size_t
DoublyLinkedListSize(DoublyLinkedList* list);

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
 * @return 0 if success; otherwise, -1
 */
int
DoublyLinkedListPushFront(DoublyLinkedList* list, void* data);

/**
 * Inserts an element to the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Data to be inserted
 * @return 0 if success; otherwise, -1
 */
int
DoublyLinkedListPushBack(DoublyLinkedList* list, void* data);

/**
 * Deletes an element at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data of the removed element
 */
void*
DoublyLinkedListPopFront(DoublyLinkedList* list);

/**
 * Deletes an element at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data of the removed element
 */
void*
DoublyLinkedListPopBack(DoublyLinkedList* list);

/**
 * Deallocates a doubly linked list.
 *
 * @param list Doubly linked list
 */
void
DoublyLinkedListDestroy(DoublyLinkedList* list);

#endif
