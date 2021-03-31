#include "hyundeok/structures/doubly_linked_list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdlib.h>

static int*
CreateIntData(int data) {
  int* ptr = malloc(sizeof(int));
  *ptr = data;
  return ptr;
}

static int
CastVoidPtrToInt(void* vptr) {
  return *(int*)vptr;
}

static int*
CreateRandomArray(int size) {
  int* arr = malloc(size * sizeof(int));

  for (int i = 0; i < size; ++i)
    arr[i] = rand();

  return arr;
}

static void
test_DoublyLinkedListNewAndFree() {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_int_equal(DoublyLinkedListSize(list), 0);
  assert_null(list->head_);
  assert_null(list->tail_);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedLisIsEmpty(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushFront(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 3);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushBack(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 3);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPopFront_Empty(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListPopFront(list);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPopBack_Empty(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListPopFront(list);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushFront_Connection(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 3);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 1);

  assert_null(list->head_->prev_);
  assert_int_equal(CastVoidPtrToInt(list->head_->data_), 3);
  assert_int_equal(CastVoidPtrToInt(list->head_->next_->data_), 2);
  assert_int_equal(CastVoidPtrToInt(list->head_->next_->next_->data_), 1);
  assert_null(list->head_->next_->next_->next_);

  assert_ptr_equal(list->head_, list->head_->next_->prev_);
  assert_ptr_equal(list->head_->next_, list->head_->next_->next_->prev_);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushBack_Connection(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 3);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 1);

  assert_null(list->tail_->next_);
  assert_int_equal(CastVoidPtrToInt(list->tail_->data_), 3);
  assert_int_equal(CastVoidPtrToInt(list->tail_->prev_->data_), 2);
  assert_int_equal(CastVoidPtrToInt(list->tail_->prev_->prev_->data_), 1);
  assert_null(list->tail_->prev_->prev_->prev_);

  assert_ptr_equal(list->tail_, list->tail_->prev_->next_);
  assert_ptr_equal(list->tail_->prev_, list->tail_->prev_->prev_->next_);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPopFront(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPopBack(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushPop1(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);
  DoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 2);

  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPushPop2(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);
  DoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPopFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 2);

  DoublyLinkedListPopBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_DoublyLinkedListNewAndFree),
      cmocka_unit_test(test_DoublyLinkedLisIsEmpty),
      cmocka_unit_test(test_DoublyLinkedListPushFront),
      cmocka_unit_test(test_DoublyLinkedListPushBack),
      cmocka_unit_test(test_DoublyLinkedListPopFront_Empty),
      cmocka_unit_test(test_DoublyLinkedListPopBack_Empty),
      cmocka_unit_test(test_DoublyLinkedListPushFront_Connection),
      cmocka_unit_test(test_DoublyLinkedListPushBack_Connection),
      cmocka_unit_test(test_DoublyLinkedListPopFront),
      cmocka_unit_test(test_DoublyLinkedListPopBack),
      cmocka_unit_test(test_DoublyLinkedListPushPop1),
      cmocka_unit_test(test_DoublyLinkedListPushPop2),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
