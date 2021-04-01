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
}

static void
FreeDoublyLinkedListNode(DllnT* node, void (*destructor)(void*)) {
  if (destructor != NULL)
    destructor(node->data_);
  free(node);
}

static void
FreeLastElement(DoublyLinkedList* list) {
  FreeDoublyLinkedListNode(list->head_, list->destructor_);
  list->head_ = NULL;
  list->tail_ = NULL;
}

static int
HasOneNode(DoublyLinkedList* list) {
  return list->head_ != NULL && list->head_ == list->tail_;
}

DoublyLinkedList*
DoublyLinkedListNew() {
  return DoublyLinkedListNewFull(NULL);
}

DoublyLinkedList*
DoublyLinkedListNewFull(void (*destructor)(void*)) {
  DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));

  // check whether list is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(list, NULL);

  list->destructor_ = destructor;
  list->head_ = NULL;
  list->tail_ = NULL;

  return list;
}

int
DoublyLinkedListIsEmpty(DoublyLinkedList* list) {
  return list->head_ == NULL;
}

void*
DoublyLinkedListFront(DoublyLinkedList* list) {
  return DoublyLinkedListIsEmpty(list) ? NULL : list->head_->data_;
}

void*
DoublyLinkedListBack(DoublyLinkedList* list) {
  return DoublyLinkedListIsEmpty(list) ? NULL : list->tail_->data_;
}

DllnT*
DoublyLinkedListPrepend(DoublyLinkedList* list, void* data) {
  DllnT* new_node = CreateDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return NULL if NULL
  HYUNDEOK_ASSERT(!new_node, 0, NULL, NULL);

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
  } else {
    new_node->next_ = list->head_;
    list->head_->prev_ = new_node;
    list->head_ = new_node;
  }

  return list->head_;
}

DllnT*
DoublyLinkedListAppend(DoublyLinkedList* list, void* data) {
  DllnT* new_node = CreateDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return -1 if NULL
  HYUNDEOK_ASSERT(!new_node, 0, NULL, NULL);

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
  } else {
    new_node->prev_ = list->tail_;
    list->tail_->next_ = new_node;
    list->tail_ = new_node;
  }

  return list->tail_;
}

DllnT*
DoublyLinkedListPrependNode(DoublyLinkedList* list, DllnT* node) {
  if (node == NULL)
    return NULL;

  node->prev_ = NULL;
  node->next_ = NULL;

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, node);
  } else {
    list->head_->prev_ = node;
    node->next_ = list->head_;
    list->head_ = node;
  }

  return list->head_;
}

DllnT*
DoublyLinkedListAppendNode(DoublyLinkedList* list, DllnT* node) {
  if (node == NULL)
    return NULL;

  node->prev_ = NULL;
  node->next_ = NULL;

  if (DoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, node);
  } else {
    list->tail_->next_ = node;
    node->prev_ = list->tail_;
    list->tail_ = node;
  }

  return list->tail_;
}

void
DoublyLinkedListRemove(DoublyLinkedList* list, DllnT* node, int free_node) {
  if (node == NULL)
    return;

  DllnT* prev = node->prev_;
  DllnT* next = node->next_;

  if (prev != NULL)
    prev->next_ = node->next_;

  if (next != NULL)
    next->prev_ = node->prev_;

  if (free_node)
    FreeDoublyLinkedListNode(node, list->destructor_);

  // if there are no elements after removal
  if (prev == NULL && next == NULL) {
    list->head_ = NULL;
    list->tail_ = NULL;
  }
  // if there is only one element after removal
  else if (list->head_ != list->tail_ && list->head_ != NULL &&
           (prev == NULL || next == NULL)) {
    if (prev != NULL)
      list->tail_ = list->head_;
    if (next != NULL)
      list->head_ = list->tail_;
  }
}

void
DoublyLinkedListRemoveFront(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return;

  if (HasOneNode(list)) {
    FreeLastElement(list);
    return;
  }

  DllnT* tmp = list->head_->next_;
  FreeDoublyLinkedListNode(list->head_, list->destructor_);
  tmp->prev_ = NULL;
  list->head_ = tmp;
}

void
DoublyLinkedListRemoveBack(DoublyLinkedList* list) {
  if (DoublyLinkedListIsEmpty(list))
    return;

  if (HasOneNode(list)) {
    FreeLastElement(list);
    return;
  }

  DllnT* tmp = list->tail_->prev_;
  FreeDoublyLinkedListNode(list->tail_, list->destructor_);
  tmp->next_ = NULL;
  list->tail_ = tmp;
}

void
DoublyLinkedListDestroy(DoublyLinkedList* list) {
  for (DllnT* head = list->head_; head != NULL;) {
    DllnT* next = head->next_;
    FreeDoublyLinkedListNode(head, list->destructor_);
    head = next;
  }

  free(list);
}
