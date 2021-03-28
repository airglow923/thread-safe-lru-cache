include(FetchCMocka.cmake)

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_doubly_linked_list.c)

add_executable(LRU_Cache_Test "")

target_sources(LRU_Cache_Test PRIVATE ${TEST_FILES})

target_compile_features(LRU_Cache_Test PRIVATE c_std_99)

target_link_libraries(
    LRU_Cache_Test
        PRIVATE ${PROJECT_NAME}
        PRIVATE cmocka-static)

set_target_properties(LRU_Cache_Test PROPERTIES LINKER_LANGUAGE C)

add_test(
    NAME
        CTestBuild
    COMMAND
        "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target LRU_Cache_Test
)

add_test(NAME CTestRun COMMAND LRU_Cache_Test)

set_tests_properties(CTestRun PROPERTIES DEPENDS CTestBuild)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS LRU_Cache_Test)
