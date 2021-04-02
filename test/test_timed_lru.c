#include "hyundeok/cache/timed_lru.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char*
CreateString(char* str) {
  const size_t len = strlen(str) + 1;
  char* ptr = malloc(len);
  snprintf(ptr, len, "%s", str);
  return ptr;
}

static void
test_LRUCacheNewAndFree(void** state) {
  LRUCache* cache = LRUCacheNew(10, 10, free, free);
  LRUCacheDestroy(cache);
}

static void
test_LRUCacheInsert(void** state) {
  LRUCache* cache = LRUCacheNew(10, 10, free, free);
  char* firstnames = {
      CreateString("Hyundeok"),
      CreateString("Bjarne"),
      CreateString("Herb"),
      CreateString("Scott"),
  };
  char* lastnames = {
      CreateString("Park"),
      CreateString("Stroutstrup"),
      CreateString("Sutters"),
      CreateString("Meyers"),
  };

  LRUCacheInsert(cache, firstnames[0], lastnames[0]);
  LRUCacheInsert(cache, firstnames[1], lastnames[1]);
  LRUCacheInsert(cache, firstnames[2], lastnames[2]);
  LRUCacheInsert(cache, firstnames[3], lastnames[3]);

  LRUCacheDestroy(cache);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_LRUCacheNewAndFree),
      cmocka_unit_test(test_LRUCacheInsert),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
