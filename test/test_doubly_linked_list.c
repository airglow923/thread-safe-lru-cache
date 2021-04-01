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
test_DoublyLinkedListPrepend(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPrepend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPrepend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPrepend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 3);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListAppend(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListAppend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 3);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListRemove_NULL(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNew();
  DllnT* node = NULL;

  DoublyLinkedListRemove(list, node);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListRemoveFront_Empty(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListRemoveFront(list);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListRemoveBack_Empty(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListRemoveFront(list);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListPrepend_Connection(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPrepend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPrepend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPrepend(list, input[2]);
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
test_DoublyLinkedListAppend_Connection(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListAppend(list, input[2]);
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
test_DoublyLinkedListRemove_OneElement(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
  };

  DoublyLinkedListPrepend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  DllnT* node = list->head_;

  DoublyLinkedListRemove(list, node);
  assert_true(DoublyLinkedListIsEmpty(list));
  assert_null(list->head_);
  assert_null(list->tail_);

  DoublyLinkedListDestroy(list);
}

void
test_DoublyLinkedListRemove_TwoElements_Prev(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);

  assert_ptr_equal(list->head_->data_, input[0]);
  assert_ptr_equal(list->tail_->data_, input[1]);

  DllnT* node = list->head_->next_;

  DoublyLinkedListRemove(list, node);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_ptr_equal(list->head_->data_, input[0]);
  assert_ptr_equal(list->tail_->data_, input[0]);

  assert_null(list->head_->prev_);
  assert_null(list->head_->next_);
  assert_null(list->tail_->prev_);
  assert_null(list->tail_->next_);

  DoublyLinkedListDestroy(list);
}

void
test_DoublyLinkedListRemove_TwoElements_Next(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);

  assert_ptr_equal(list->head_->data_, input[0]);
  assert_ptr_equal(list->tail_->data_, input[1]);

  DllnT* node = list->head_;

  DoublyLinkedListRemove(list, node);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_ptr_equal(list->head_->data_, input[1]);
  assert_ptr_equal(list->tail_->data_, input[1]);

  assert_null(list->head_->prev_);
  assert_null(list->head_->next_);
  assert_null(list->tail_->prev_);
  assert_null(list->tail_->next_);

  DoublyLinkedListDestroy(list);
}

void
test_DoublyLinkedListRemove_ThreeElements(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListAppend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_ptr_equal(list->head_->data_, input[0]);
  assert_ptr_equal(list->tail_->data_, input[2]);

  DllnT* node = list->head_->next_;

  DoublyLinkedListRemove(list, node);
  assert_int_equal(DoublyLinkedListSize(list), 2);

  assert_ptr_equal(list->head_->data_, input[0]);
  assert_ptr_equal(list->tail_->data_, input[2]);

  assert_null(list->head_->prev_);
  assert_ptr_equal(list->head_->next_->data_, input[2]);
  assert_null(list->tail_->next_);
  assert_ptr_equal(list->tail_->prev_->data_, input[0]);

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListRemoveFront(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListPrepend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPrepend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListPrepend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListRemoveFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListRemoveFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListRemoveFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedListRemoveBack(void** state) {
  DoublyLinkedList* list = DoublyLinkedListNewFull(free);
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListAppend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListRemoveBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListRemoveBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListRemoveBack(list);
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

  DoublyLinkedListAppend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListRemoveFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);
  DoublyLinkedListAppend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListAppend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListRemoveBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 2);

  DoublyLinkedListRemoveFront(list);
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

  DoublyLinkedListPrepend(list, input[0]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListRemoveBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);
  DoublyLinkedListPrepend(list, input[1]);
  assert_int_equal(DoublyLinkedListSize(list), 1);
  DoublyLinkedListPrepend(list, input[2]);
  assert_int_equal(DoublyLinkedListSize(list), 2);
  DoublyLinkedListRemoveFront(list);
  assert_int_equal(DoublyLinkedListSize(list), 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(DoublyLinkedListBack(list)), 2);

  DoublyLinkedListRemoveBack(list);
  assert_int_equal(DoublyLinkedListSize(list), 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListDestroy(list);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_DoublyLinkedListNewAndFree),
      cmocka_unit_test(test_DoublyLinkedLisIsEmpty),
      cmocka_unit_test(test_DoublyLinkedListPrepend),
      cmocka_unit_test(test_DoublyLinkedListAppend),
      cmocka_unit_test(test_DoublyLinkedListRemoveFront_Empty),
      cmocka_unit_test(test_DoublyLinkedListRemoveBack_Empty),
      cmocka_unit_test(test_DoublyLinkedListPrepend_Connection),
      cmocka_unit_test(test_DoublyLinkedListAppend_Connection),
      cmocka_unit_test(test_DoublyLinkedListRemove_OneElement),
      cmocka_unit_test(test_DoublyLinkedListRemove_TwoElements_Prev),
      cmocka_unit_test(test_DoublyLinkedListRemove_TwoElements_Next),
      cmocka_unit_test(test_DoublyLinkedListRemove_ThreeElements),
      cmocka_unit_test(test_DoublyLinkedListRemoveFront),
      cmocka_unit_test(test_DoublyLinkedListRemoveBack),
      cmocka_unit_test(test_DoublyLinkedListPushPop1),
      cmocka_unit_test(test_DoublyLinkedListPushPop2),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
