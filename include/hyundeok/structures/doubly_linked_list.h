#ifndef HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H
#define HYUNDEOK_STRUCTURES_DOUBLY_LINKED_LIST_H

#include "hyundeok/types.h"

// this is used for internal implementation
// it should not be accessed by end users
typedef struct DoublyLinkedListNode {
  UllT data_;
  struct DoublyLinkedListNode* prev_;
  struct DoublyLinkedListNode* next_;
} DllnT;

typedef struct DoublyLinkedList {
  UllT size_;
  DllnT* head_;
  DllnT* tail_;
} DoublyLinkedList;

/**
 * Creates a doubly linked list.
 *
 * @return List Doubly linked list
 */
DoublyLinkedList* DoublyLinkedListNew(void);

/**
 * Returns the size of a doubly linked list.
 *
 * @param list Doubly linked list
 * @return Size
 */
UllT DoublyLinkedListSize(DoublyLinkedList* list);

/**
 * Checks if a doubly linked list is empty.
 *
 * @param list Doubly linked list
 * @return Emptiness 1 if empty; 0 otherwise
 */
int DoublyLinkedListIsEmpty(DoublyLinkedList* list);

/**
 * Returns data at the head of a doubly linked list.
 *
 * This must be called when there is at least one element in a list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
UllT DoublyLinkedListFront(DoublyLinkedList* list);

/**
 * Returns data at the tail of a doubly linked list.
 *
 * This must be called when there is at least one element in a list.
 *
 * @param list Doubly linked list
 * @return Data at the head
 */
UllT DoublyLinkedListBack(DoublyLinkedList* list);

/**
 * Inserts an element to the head of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Integer data
 * @return 0 if success; otherwise, -1
 */
int DoublyLinkedListPushFront(DoublyLinkedList* list, UllT data);

/**
 * Inserts an element to the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 * @param data Integer data
 * @return 0 if success; otherwise, -1
 */
int DoublyLinkedListPushBack(DoublyLinkedList* list, UllT data);

/**
 * Deletes an element at the head of a doubly linked list.
 *
 * @param list Doubly linked list
 */
void DoublyLinkedListPopFront(DoublyLinkedList* list);

/**
 * Deletes an element at the tail of a doubly linked list.
 *
 * @param list Doubly linked list
 */
void DoublyLinkedListPopBack(DoublyLinkedList* list);

/**
 * Deallocates a doubly linked list.
 *
 * @param list Doubly linked list
 */
void DoublyLinkedListDestroy(DoublyLinkedList* list);

#endif
