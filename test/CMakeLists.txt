include(FetchCMocka.cmake)

SET(CMOCKA_TEST_ABORT 1)

add_executable(test_doubly_linked_list "")

target_sources(test_doubly_linked_list
    PRIVATE test_doubly_linked_list.c)

target_compile_features(test_doubly_linked_list
    PRIVATE c_std_99)

target_link_libraries(test_doubly_linked_list
    PRIVATE ${PROJECT_NAME}
    PRIVATE cmocka-static)

set_target_properties(test_doubly_linked_list
    PROPERTIES
        LINKER_LANGUAGE C)

add_test(NAME CTestRun1 COMMAND test_doubly_linked_list)

set_tests_properties(CTestRun1 PROPERTIES DEPENDS CTestBuild)

add_executable(test_mutex_doubly_linked_list "")

target_sources(test_mutex_doubly_linked_list
    PRIVATE test_mutex_doubly_linked_list.c)

target_compile_features(test_mutex_doubly_linked_list
    PRIVATE c_std_99)

target_link_libraries(test_mutex_doubly_linked_list
    PRIVATE ${PROJECT_NAME}
    PRIVATE cmocka-static)

set_target_properties(test_mutex_doubly_linked_list
    PROPERTIES
        LINKER_LANGUAGE C)

add_test(
    NAME
        CTestBuild
    COMMAND ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR}
        --target
            test_doubly_linked_list
            test_mutex_doubly_linked_list)

add_test(NAME CTestRun2 COMMAND test_mutex_doubly_linked_list)

set_tests_properties(CTestRun2 PROPERTIES DEPENDS CTestBuild)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS
        test_doubly_linked_list
        test_mutex_doubly_linked_list)
