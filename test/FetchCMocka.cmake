cmake_minimum_required(VERSION 2.8.2)

include(FetchContent)

FetchContent_Declare(
  cmocka
  GIT_REPOSITORY    https://git.cryptomilk.org/projects/cmocka.git
  GIT_TAG           cmocka-1.1.5
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/cmocka-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/cmocka-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

set(WITH_STATIC_LIB ON CACHE BOOL "CMocka: Build with a static library" FORCE)
set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "CMocka: Install a cmockery header" FORCE)
set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
set(PICKY_DEVELOPER OFF CACHE BOOL "CMocka: Build with picky developer flags" FORCE)

FetchContent_MakeAvailable(cmocka)
