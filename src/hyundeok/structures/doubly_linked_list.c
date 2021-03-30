#include "hyundeok/structures/doubly_linked_list.h"
#include "hyundeok/macro.h" // HYUNDEOK_ASSERT, HYUNDEOK_MEMORY_ASSERT

#include <stddef.h> // NULL
#include <stdlib.h> // malloc, free

static DllnT* CreateDoublyLinkedListNode(UllT data) {
  DllnT* new_node = malloc(sizeof(DllnT));

  // check whether new_node is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(new_node, NULL);

  new_node->data_ = data;
  new_node->prev_ = NULL;
  new_node->next_ = NULL;

  return new_node;
}

static void InitialiseHeadAndTail(DoublyLinkedList* list, DllnT* new_node) {
  list->head_ = new_node;
  list->tail_ = new_node;
  ++list->size_;
}

static void FreeLastElement(DoublyLinkedList* list) {
  free(list->head_);
  list->head_ = NULL;
  list->tail_ = NULL;
  --list->size_;
}

DoublyLinkedList* CreateDoublyLinkedList(void) {
  DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));

  // check whether list is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(list, NULL);

  list->size_ = 0;
  list->head_ = NULL;
  list->tail_ = NULL;

  return list;
}

UllT DoublyLinkedListSize(DoublyLinkedList* list) {
  return list->size_;
}

int DoublyLinkedListIsEmpty(DoublyLinkedList* list) {
  return DoublyLinkedListSize(list) == 0;
}

UllT DoublyLinkedListFront(DoublyLinkedList* list) {
  return list->head_->data_;
}

UllT DoublyLinkedListBack(DoublyLinkedList* list) { return list->tail_->data_; }

int DoublyLinkedListPushFront(DoublyLinkedList* list, UllT data) {
  DllnT* new_node = CreateDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return -1 if NULL
  HYUNDEOK_ASSERT(!new_node, 0, -1, NULL);

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
    return 0;
  }

  new_node->next_ = list->head_;
  list->head_->prev_ = new_node;
  list->head_ = new_node;

  ++list->size_;

  return 0;
}

int DoublyLinkedListPushBack(DoublyLinkedList* list, UllT data) {
  DllnT* new_node = CreateDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return -1 if NULL
  HYUNDEOK_ASSERT(!new_node, 0, -1, NULL);

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
    return 0;
  }

  new_node->prev_ = list->tail_;
  list->tail_->next_ = new_node;
  list->tail_ = new_node;

  ++list->size_;

  return 0;
}

void DoublyLinkedListPopFront(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return;

  if (list->size_ == 1) {
    FreeLastElement(list);
    return;
  }

  DllnT* tmp = list->head_->next_;

  free(list->head_);
  tmp->prev_ = NULL;
  list->head_ = tmp;

  --list->size_;
}

void DoublyLinkedListPopBack(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return;

  if (list->size_ == 1) {
    FreeLastElement(list);
    return;
  }

  DllnT* tmp = list->tail_->prev_;

  free(list->tail_);
  tmp->next_ = NULL;
  list->tail_ = tmp;

  --list->size_;
}

void RemoveDoublyLinkedList(DoublyLinkedList* list) {
  for (DllnT* head = list->head_; head;) {
    DllnT* next = head->next_;
    free(head);
    head = next;
  }

  free(list);
}
