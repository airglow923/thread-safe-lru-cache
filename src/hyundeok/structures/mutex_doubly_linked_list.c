#include "hyundeok/structures/mutex_doubly_linked_list.h"
#include "hyundeok/macro.h" // HYUNDEOK_ASSERT, HYUNDEOK_MEMORY_ASSERT

#include <stdlib.h> // malloc, free

static MDllnT*
CreateMutexDoublyLinkedListNode(void* data) {
  MDllnT* new_node = malloc(sizeof(MDllnT));

  // check whether new_node is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(new_node, NULL);

  new_node->data_ = data;
  new_node->prev_ = NULL;
  new_node->next_ = NULL;

  return new_node;
}

static void
InitialiseHeadAndTail(MutexDoublyLinkedList* list, MDllnT* new_node) {
  list->head_ = new_node;
  list->tail_ = new_node;
  ++list->size_;
}

static void*
FreeLastElement(MutexDoublyLinkedList* list) {
  void* data = MutexDoublyLinkedListFront(list);

  free(list->head_);
  list->head_ = NULL;
  list->tail_ = NULL;
  --list->size_;

  return data;
}

MutexDoublyLinkedList*
MutexDoublyLinkedListNew(void) {
  MutexDoublyLinkedList* list = malloc(sizeof(MutexDoublyLinkedList));

  // check whether list is NULL or not; return NULL if NULL
  HYUNDEOK_MEMORY_ASSERT(list, NULL);

  int result = pthread_mutex_init(&list->lock_, NULL);

  // if pthread_mutex_init fails, deallocate list
  HYUNDEOK_ASSERT(result, 0, NULL, free(list));

  list->size_ = 0;
  list->head_ = NULL;
  list->tail_ = NULL;

  return list;
}

size_t
MutexDoublyLinkedListSize(MutexDoublyLinkedList* list) {
  return list->size_;
}

int
MutexDoublyLinkedListIsEmpty(MutexDoublyLinkedList* list) {
  return MutexDoublyLinkedListSize(list) == 0;
}

void*
MutexDoublyLinkedListFront(MutexDoublyLinkedList* list) {
  return MutexDoublyLinkedListIsEmpty(list) ? NULL : list->head_->data_;
}

void*
MutexDoublyLinkedListBack(MutexDoublyLinkedList* list) {
  return MutexDoublyLinkedListIsEmpty(list) ? NULL : list->tail_->data_;
}

int
MutexDoublyLinkedListPushFront(MutexDoublyLinkedList* list, void* data) {
  pthread_mutex_lock(&list->lock_);

  MDllnT* new_node = CreateMutexDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return -1 if NULL
  HYUNDEOK_ASSERT(!new_node, 0, -1, NULL);

  if (MutexDoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
    pthread_mutex_unlock(&list->lock_);
    return 0;
  }

  new_node->next_ = list->head_;
  list->head_->prev_ = new_node;
  list->head_ = new_node;

  ++list->size_;

  pthread_mutex_unlock(&list->lock_);

  return 0;
}

int
MutexDoublyLinkedListPushBack(MutexDoublyLinkedList* list, void* data) {
  pthread_mutex_lock(&list->lock_);

  MDllnT* new_node = CreateMutexDoublyLinkedListNode(data);

  // check whether new_node is NULL or not; return -1 if NULL
  HYUNDEOK_ASSERT(!new_node, 0, -1, NULL);

  if (MutexDoublyLinkedListIsEmpty(list)) {
    InitialiseHeadAndTail(list, new_node);
    pthread_mutex_unlock(&list->lock_);
    return 0;
  }

  new_node->prev_ = list->tail_;
  list->tail_->next_ = new_node;
  list->tail_ = new_node;

  ++list->size_;

  pthread_mutex_unlock(&list->lock_);

  return 0;
}

void*
MutexDoublyLinkedListPopFront(MutexDoublyLinkedList* list) {
  pthread_mutex_lock(&list->lock_);

  if (MutexDoublyLinkedListIsEmpty(list)) {
    pthread_mutex_unlock(&list->lock_);
    return NULL;
  }

  if (MutexDoublyLinkedListSize(list) == 1) {
    void* data = FreeLastElement(list);
    pthread_mutex_unlock(&list->lock_);
    return data;
  }

  MDllnT* tmp = list->head_->next_;
  void* data = MutexDoublyLinkedListFront(list);

  free(list->head_);
  tmp->prev_ = NULL;
  list->head_ = tmp;

  --list->size_;

  pthread_mutex_unlock(&list->lock_);

  return data;
}

void*
MutexDoublyLinkedListPopBack(MutexDoublyLinkedList* list) {
  pthread_mutex_lock(&list->lock_);

  if (MutexDoublyLinkedListIsEmpty(list)) {
    pthread_mutex_unlock(&list->lock_);
    return NULL;
  }

  if (MutexDoublyLinkedListSize(list) == 1) {
    void* data = FreeLastElement(list);
    pthread_mutex_unlock(&list->lock_);
    return data;
  }

  MDllnT* tmp = list->tail_->prev_;
  void* data = MutexDoublyLinkedListBack(list);

  free(list->tail_);
  tmp->next_ = NULL;
  list->tail_ = tmp;

  --list->size_;

  pthread_mutex_unlock(&list->lock_);

  return data;
}

void
MutexDoublyLinkedListDestroy(MutexDoublyLinkedList* list) {
  pthread_mutex_lock(&list->lock_);

  for (MDllnT* head = list->head_; head;) {
    MDllnT* next = head->next_;
    free(head);
    head = next;
  }

  pthread_mutex_unlock(&list->lock_);
  pthread_mutex_destroy(&list->lock_);
  free(list);
}
