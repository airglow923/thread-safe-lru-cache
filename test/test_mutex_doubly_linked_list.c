#include "hyundeok/structures/mutex_doubly_linked_list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <unistd.h>
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

static void
FreeIntArray(int** arr, int size) {
  for (int i = 0; i < size; ++i)
    free(arr[i]);
}

static int*
CreateRandomArray(int size) {
  int* arr = malloc(size * sizeof(int));

  for (int i = 0; i < size; ++i)
    arr[i] = rand();

  return arr;
}

static void
test_MutexDoublyLinkedListNewAndFree() {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();

  assert_int_equal(MutexDoublyLinkedListSize(list), 0);
  assert_null(list->head_);
  assert_null(list->tail_);

  MutexDoublyLinkedListDestroy(list);
}

static void
test_DoublyLinkedLisIsEmpty(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();

  assert_true(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListDestroy(list);
}

static void
test_MutexDoublyLinkedListPushFront(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListFront(list)), 3);

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPushBack(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListBack(list)), 3);

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPopFront_Empty(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();

  assert_true(MutexDoublyLinkedListIsEmpty(list));
  MutexDoublyLinkedListPopFront(list);

  MutexDoublyLinkedListDestroy(list);
}

static void
test_MutexDoublyLinkedListPopBack_Empty(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();

  assert_true(MutexDoublyLinkedListIsEmpty(list));
  MutexDoublyLinkedListPopFront(list);

  MutexDoublyLinkedListDestroy(list);
}

static void
test_MutexDoublyLinkedListPushFront_Connection(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListFront(list)), 3);
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListBack(list)), 1);

  assert_null(list->head_->prev_);
  assert_int_equal(CastVoidPtrToInt(list->head_->data_), 3);
  assert_int_equal(CastVoidPtrToInt(list->head_->next_->data_), 2);
  assert_int_equal(CastVoidPtrToInt(list->head_->next_->next_->data_), 1);
  assert_null(list->head_->next_->next_->next_);

  assert_ptr_equal(list->head_, list->head_->next_->prev_);
  assert_ptr_equal(list->head_->next_, list->head_->next_->next_->prev_);

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPushBack_Connection(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListBack(list)), 3);
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListFront(list)), 1);

  assert_null(list->tail_->next_);
  assert_int_equal(CastVoidPtrToInt(list->tail_->data_), 3);
  assert_int_equal(CastVoidPtrToInt(list->tail_->prev_->data_), 2);
  assert_int_equal(CastVoidPtrToInt(list->tail_->prev_->prev_->data_), 1);
  assert_null(list->tail_->prev_->prev_->prev_);

  assert_ptr_equal(list->tail_, list->tail_->prev_->next_);
  assert_ptr_equal(list->tail_->prev_, list->tail_->prev_->prev_->next_);

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPopFront(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);

  assert_true(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPopBack(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 3);

  assert_false(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);

  assert_true(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPushPop1(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushBack(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);
  MutexDoublyLinkedListPushBack(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushBack(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);

  assert_false(MutexDoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListBack(list)), 2);

  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);

  assert_true(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

static void
test_MutexDoublyLinkedListPushPop2(void** state) {
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int* input[] = {
      CreateIntData(1),
      CreateIntData(2),
      CreateIntData(3),
  };

  MutexDoublyLinkedListPushFront(list, input[0]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);
  MutexDoublyLinkedListPushFront(list, input[1]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);
  MutexDoublyLinkedListPushFront(list, input[2]);
  assert_int_equal(MutexDoublyLinkedListSize(list), 2);
  MutexDoublyLinkedListPopFront(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 1);

  assert_false(MutexDoublyLinkedListIsEmpty(list));

  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListFront(list)), 2);
  assert_int_equal(CastVoidPtrToInt(MutexDoublyLinkedListBack(list)), 2);

  MutexDoublyLinkedListPopBack(list);
  assert_int_equal(MutexDoublyLinkedListSize(list), 0);

  assert_true(MutexDoublyLinkedListIsEmpty(list));

  MutexDoublyLinkedListDestroy(list);
  FreeIntArray(input, 3);
}

struct ListInsertArgs {
  MutexDoublyLinkedList* list_;
  int no_of_elements_;
};

static void*
ListInsertFront(void* arguments) {
  struct ListInsertArgs* args = arguments;

  for (int i = 0; i < args->no_of_elements_; ++i)
    MutexDoublyLinkedListPushFront(args->list_, NULL);

  return NULL;
}

static void*
ListInsertBack(void* arguments) {
  struct ListInsertArgs* args = arguments;

  for (int i = 0; i < args->no_of_elements_; ++i)
    MutexDoublyLinkedListPushBack(args->list_, NULL);

  return NULL;
}

static void
test_MutexDoublyLinkedList_ConcurrentInsertion(void** state) {
  int no_of_threads = 10;
  pthread_t tid[no_of_threads];
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int no_of_elements = 1000000;
  struct ListInsertArgs args = {list, no_of_elements / no_of_threads};

  for (int i = 0; i < no_of_threads; ++i) {
    if (i < no_of_threads / 2)
      pthread_create(&tid[i], NULL, ListInsertFront, &args);
    else
      pthread_create(&tid[i], NULL, ListInsertBack, &args);

    sleep(2);
  }

  for (int i = 0; i < no_of_threads; ++i)
    pthread_join(tid[i], NULL);

  assert_int_equal(MutexDoublyLinkedListSize(list), no_of_elements);

  MutexDoublyLinkedListDestroy(list);
}

static void*
ListInsertAndRead(void* arguments) {
  struct ListInsertArgs* args = arguments;

  for (int i = 0; i < args->no_of_elements_; ++i) {
    MutexDoublyLinkedListPushBack(args->list_, NULL);
    MutexDoublyLinkedListFront(args->list_);
  }

  return NULL;
}

static void
test_MutexDoublyLinkedList_ConcurrentInsertionRead(void** state) {
  int no_of_threads = 10;
  pthread_t tid[no_of_threads];
  MutexDoublyLinkedList* list = MutexDoublyLinkedListNew();
  int no_of_elements = 1000000;
  struct ListInsertArgs args = {list, no_of_elements / no_of_threads};

  for (int i = 0; i < no_of_threads; ++i) {
    int result = pthread_create(&tid[i], NULL, ListInsertAndRead, &args);

    // if thread is not created properly, clear all previous threads
    if (result != 0) {
      for (int j = i - 1; j >= 0; --j)
        pthread_join(tid[i], NULL);

      assert_int_equal(result, 0);
      sleep(2);
    }
  }

  for (int i = 0; i < no_of_threads; ++i)
    pthread_join(tid[i], NULL);

  assert_int_equal(MutexDoublyLinkedListSize(list), no_of_elements);

  MutexDoublyLinkedListDestroy(list);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_MutexDoublyLinkedListNewAndFree),
      cmocka_unit_test(test_DoublyLinkedLisIsEmpty),
      cmocka_unit_test(test_MutexDoublyLinkedListPushFront),
      cmocka_unit_test(test_MutexDoublyLinkedListPushBack),
      cmocka_unit_test(test_MutexDoublyLinkedListPopFront_Empty),
      cmocka_unit_test(test_MutexDoublyLinkedListPopBack_Empty),
      cmocka_unit_test(test_MutexDoublyLinkedListPushFront_Connection),
      cmocka_unit_test(test_MutexDoublyLinkedListPushBack_Connection),
      cmocka_unit_test(test_MutexDoublyLinkedListPopFront),
      cmocka_unit_test(test_MutexDoublyLinkedListPopBack),
      cmocka_unit_test(test_MutexDoublyLinkedListPushPop1),
      cmocka_unit_test(test_MutexDoublyLinkedListPushPop2),
      cmocka_unit_test(test_MutexDoublyLinkedList_ConcurrentInsertion),
      cmocka_unit_test(test_MutexDoublyLinkedList_ConcurrentInsertionRead),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
