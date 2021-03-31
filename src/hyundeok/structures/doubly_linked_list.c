#include "hyundeok/structures/doubly_linked_list.h"
#include "hyundeok/macro.h" // HYUNDEOK_ASSERT, HYUNDEOK_MEMORY_ASSERT

#include <stdlib.h> // malloc, free

static DllnT*
CreateDoublyLinkedListNode(void* data) {
  DllnT* new_node = malloc(sizeof(DllnT));

  // check whether new_node is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(new_node, NULL);

  new_node->data_ = data;
  new_node->prev_ = NULL;
  new_node->next_ = NULL;

  return new_node;
}

static void
InitialiseHeadAndTail(DoublyLinkedList* list, DllnT* new_node) {
  list->head_ = new_node;
  list->tail_ = new_node;
  ++list->size_;
}

static void
FreeDoublyLinkedListNode(void (*destructor)(void*), DllnT* node) {
  destructor(node->data_);
  free(node);
}

static void*
FreeLastElement(DoublyLinkedList* list) {
  void* data = DoublyLinkedListFront(list);

  free(list->head_);
  list->head_ = NULL;
  list->tail_ = NULL;
  --list->size_;

  return data;
}

static void
EmptyDestructor(void* data) {}

DoublyLinkedList*
DoublyLinkedListNew() {
  return DoublyLinkedListNewFull(EmptyDestructor);
}

DoublyLinkedList*
DoublyLinkedListNewFull(void (*destructor)(void*)) {
  DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));

  // check whether list is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(list, NULL);

  list->size_ = 0;
  list->desetructor_ = destructor;
  list->head_ = NULL;
  list->tail_ = NULL;

  return list;
}

size_t
DoublyLinkedListSize(DoublyLinkedList* list) {
  return list->size_;
}

int
DoublyLinkedListIsEmpty(DoublyLinkedList* list) {
  return DoublyLinkedListSize(list) == 0;
}

void*
DoublyLinkedListFront(DoublyLinkedList* list) {
  return DoublyLinkedListIsEmpty(list) ? NULL : list->head_->data_;
}

void*
DoublyLinkedListBack(DoublyLinkedList* list) {
  return DoublyLinkedListIsEmpty(list) ? NULL : list->tail_->data_;
}

int
DoublyLinkedListPushFront(DoublyLinkedList* list, void* data) {
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

int
DoublyLinkedListPushBack(DoublyLinkedList* list, void* data) {
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

void*
DoublyLinkedListPopFront(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return NULL;

  if (DoublyLinkedListSize(list) == (unsigned)1)
    return FreeLastElement(list);

  DllnT* tmp = list->head_->next_;
  void* data = DoublyLinkedListFront(list);

  FreeDoublyLinkedListNode(list->desetructor_, list->head_);
  tmp->prev_ = NULL;
  list->head_ = tmp;

  --list->size_;

  return data;
}

void*
DoublyLinkedListPopBack(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return NULL;

  if (DoublyLinkedListSize(list) == (unsigned)1)
    return FreeLastElement(list);

  DllnT* tmp = list->tail_->prev_;
  void* data = DoublyLinkedListBack(list);

  FreeDoublyLinkedListNode(list->desetructor_, list->tail_);
  tmp->next_ = NULL;
  list->tail_ = tmp;

  --list->size_;

  return data;
}

void
DoublyLinkedListDestroy(DoublyLinkedList* list) {
  for (DllnT* head = list->head_; head != NULL;) {
    DllnT* next = head->next_;
    FreeDoublyLinkedListNode(list->desetructor_, head);
    head = next;
  }

  free(list);
}
