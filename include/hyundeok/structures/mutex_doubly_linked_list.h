#ifndef HYUNDEOK_STRUCTURES_MUTEX_DOUBLY_LINKED_LIST_H
#define HYUNDEOK_STRUCTURES_MUTEX_DOUBLY_LINKED_LIST_H

#include "hyundeok/types.h"

#include <pthread.h> // pthread_mutex*

// this is used for internal implementation
// it should not be accessed by end users
typedef struct MutexDoublyLinkedListNode {
  void* data_;
  struct MutexDoublyLinkedListNode* prev_;
  struct MutexDoublyLinkedListNode* next_;
} MDllnT;

typedef struct MutexDoublyLinkedList {
  pthread_mutex_t lock_;
  size_t size_;
  MDllnT* head_;
  MDllnT* tail_;
} MutexDoublyLinkedList;

/**
 * Creates a doubly linked list.
 *
 * @return List Doubly linked list
 */
MutexDoublyLinkedList*
MutexDoublyLinkedListNew(void);

/**
 * Returns the size of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Size
 */
size_t
MutexDoublyLinkedListSize(MutexDoublyLinkedList* list);

/**
 * Checks if a doubly linked list is empty.
 *
 * @param list Doubly linked list
 * @return Emptiness 1 if empty; 0 otherwise
 */
int
MutexDoublyLinkedListIsEmpty(MutexDoublyLinkedList* list);

/**
 * Returns data at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
void*
MutexDoublyLinkedListFront(MutexDoublyLinkedList* list);

/**
 * Returns data at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
void*
MutexDoublyLinkedListBack(MutexDoublyLinkedList* list);

/**
 * Inserts an element to the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Data to be inserted
 * @return 0 if success; otherwise, -1
 */
int
MutexDoublyLinkedListPushFront(MutexDoublyLinkedList* list, void* data);

/**
 * Inserts an element to the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Data to be inserted
 * @return 0 if success; otherwise, -1
 */
int
MutexDoublyLinkedListPushBack(MutexDoublyLinkedList* list, void* data);

/**
 * Deletes an element at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data of the removed element
 */
void*
MutexDoublyLinkedListPopFront(MutexDoublyLinkedList* list);

/**
 * Deletes an element at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Data of the removed element
 */
void*
MutexDoublyLinkedListPopBack(MutexDoublyLinkedList* list);

/**
 * Deallocates a doubly linked list.
 *
 * @param list Doubly linked list
 */
void
MutexDoublyLinkedListDestroy(MutexDoublyLinkedList* list);

#endif
