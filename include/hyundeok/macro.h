#ifndef HYUNDEOK_MACRO_H
#define HYUNDEOK_MACRO_H

#include <stdio.h> // fprintf

#undef HYUNDEOK_STRINGIFY_IMPL
#define HYUNDEOK_STRINGIFY_IMPL(x) #x

#undef HYUNDEOK_STRINGIFY
#define HYUNDEOK_STRINGIFY(x) HYUNDEOK_STRINGIFY_IMPL(x)

#undef HYUNDEOK_FILE_LINE
#define HYUNDEOK_FILE_LINE __FILE__ ":" HYUNDEOK_STRINGIFY(__LINE__)

#undef HYUNDEOK_LOG
#define HYUNDEOK_LOG(format, ...)                                              \
  "%s:%s " format "\n", __func__, HYUNDEOK_FILE_LINE, ##__VA_ARGS__

#undef HYUNDEOK_PRTN_ERR
#define HYUNDEOK_PRTN_ERR(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#undef HYUNDEOK_ASSERT_FORMAT
#define HYUNDEOK_ASSERT_FORMAT(value, target, ret, callback, format, ...)      \
  do {                                                                         \
    if ((value) != (target)) {                                                 \
      callback;                                                                \
      HYUNDEOK_PRTN_ERR(HYUNDEOK_LOG(format, ##__VA_ARGS__));                  \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#undef HYUNDEOK_ASSERT
#define HYUNDEOK_ASSERT(value, target, ret, callback)                          \
  HYUNDEOK_ASSERT_FORMAT(value, target, ret, callback,                         \
                         "%s is not equal to %s\n", HYUNDEOK_STRINGIFY(value), \
                         HYUNDEOK_STRINGIFY(target))

#undef HYUNDEOK_MEMORY_ASSERT
#define HYUNDEOK_MEMORY_ASSERT(memory, callback)                               \
  HYUNDEOK_ASSERT_FORMAT(!!(memory), !NULL, NULL, callback,                    \
                         "Failed to allocate %s\n",                            \
                         HYUNDEOK_STRINGIFY(memory))

#endif
