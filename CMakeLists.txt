cmake_minimum_required(VERSION 3.13.0)

set(THREAD_SAFE_LRU_CACHE_VERSION_MAJOR 1)
set(THREAD_SAFE_LRU_CACHE_VERSION_MINOR 0)
set(THREAD_SAFE_LRU_CACHE_VERSION_PATCH 0)

string(CONCAT PRJ_VER
    "${THREAD_SAFE_LRU_CACHE_VERSION_MAJOR}"
    ".${THREAD_SAFE_LRU_CACHE_VERSION_MINOR}"
    ".${THREAD_SAFE_LRU_CACHE_VERSION_PATCH}")

project(thread-safe-lru-cache
    VERSION ${PRJ_VER}
    DESCRIPTION "Thread-safe time-based LRU cache"
    LANGUAGES C) 

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)

# find_program(CLANG_TIDY_FOUND NAMES clang-tidy)
# if (CLANG_TIDY_FOUND)
#   set(CMAKE_C_CLANG_TIDY clang-tidy)
# endif()

# vim YouCompleteMe
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
      ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()

set(PROJECT_COMPILE_FLAGS
    -fsanitize=thread
    -fPIC
    -g
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wdiv-by-zero)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")

add_library(${PROJECT_NAME} SHARED)

target_compile_options(${PROJECT_NAME}
    PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_COMPILE_FLAGS}>)

add_subdirectory(include)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${GLIB_INCLUDE_DIRS}
        src)

target_sources(${PROJECT_NAME}
    PUBLIC ${INCLUDE_FILES}
    PRIVATE ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${THREAD_SAFE_LRU_CACHE_VERSION_MAJOR}
    PUBLIC_HEADER "${INCLUDE_FILES}")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

target_link_libraries(main ${PROJECT_NAME} PkgConfig::deps)

enable_testing()
add_subdirectory(test)
