#include "hyundeok/structures/doubly_linked_list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

static void test_CreateDoublyLinkedListAndFree(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  assert_int_equal(list->size_, 0);
  assert_null(list->head_);
  assert_null(list->tail_);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedLisIsEmpty(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushFront(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushFront(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushFront(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushFront(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(DoublyLinkedListFront(list), 3);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushBack(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushBack(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushBack(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushBack(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(DoublyLinkedListBack(list), 3);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPopFront_Empty(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListPopFront(list);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPopBack_Empty(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  assert_true(DoublyLinkedListIsEmpty(list));
  DoublyLinkedListPopFront(list);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushFront_Connection(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushFront(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushFront(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushFront(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(DoublyLinkedListFront(list), 3);
  assert_int_equal(DoublyLinkedListBack(list), 1);

  assert_null(list->head_->prev_);
  assert_int_equal(list->head_->data_, 3);
  assert_int_equal(list->head_->next_->data_, 2);
  assert_int_equal(list->head_->next_->next_->data_, 1);
  assert_null(list->head_->next_->next_->next_);

  assert_ptr_equal(list->head_, list->head_->next_->prev_);
  assert_ptr_equal(list->head_->next_, list->head_->next_->next_->prev_);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushBack_Connection(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushBack(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushBack(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushBack(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));
  assert_int_equal(DoublyLinkedListBack(list), 3);
  assert_int_equal(DoublyLinkedListFront(list), 1);

  assert_null(list->tail_->next_);
  assert_int_equal(list->tail_->data_, 3);
  assert_int_equal(list->tail_->prev_->data_, 2);
  assert_int_equal(list->tail_->prev_->prev_->data_, 1);
  assert_null(list->tail_->prev_->prev_->prev_);

  assert_ptr_equal(list->tail_, list->tail_->prev_->next_);
  assert_ptr_equal(list->tail_->prev_, list->tail_->prev_->prev_->next_);

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPopFront(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushFront(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushFront(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushFront(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPopBack(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushBack(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushBack(list, 2);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPushBack(list, 3);
  assert_int_equal(list->size_, 3);

  assert_false(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushPop1(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushBack(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 0);
  DoublyLinkedListPushBack(list, 2);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushBack(list, 3);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(DoublyLinkedListFront(list), 2);
  assert_int_equal(DoublyLinkedListBack(list), 2);

  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListFree(list);
}

static void test_DoublyLinkedListPushPop2(void** state) {
  DoublyLinkedList* list = CreateDoublyLinkedList();

  DoublyLinkedListPushFront(list, 1);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 0);
  DoublyLinkedListPushFront(list, 2);
  assert_int_equal(list->size_, 1);
  DoublyLinkedListPushFront(list, 3);
  assert_int_equal(list->size_, 2);
  DoublyLinkedListPopFront(list);
  assert_int_equal(list->size_, 1);

  assert_false(DoublyLinkedListIsEmpty(list));

  assert_int_equal(DoublyLinkedListFront(list), 2);
  assert_int_equal(DoublyLinkedListBack(list), 2);

  DoublyLinkedListPopBack(list);
  assert_int_equal(list->size_, 0);

  assert_true(DoublyLinkedListIsEmpty(list));

  DoublyLinkedListFree(list);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_CreateDoublyLinkedListAndFree),
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
