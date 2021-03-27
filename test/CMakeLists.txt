set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_executable(LRU-Cache-Test "")

target_sources(LRU-Cache-Test PRIVATE ${TEST_FILES})

target_link_libraries(LRU-Cache-Test PRIVATE ${PROJECT_NAME})

set_target_properties(LRU-Cache-Test PROPERTIES LINKER_LANGUAGE C)

add_test(
    NAME
        CTestBuild
    COMMAND
        "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target LRU-Cache-Test)

add_test(NAME CTestRun COMMAND LRU-Cache-Test)

set_tests_properties(CTestRun PROPERTIES DEPENDS CTestBuild)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS LRU-Cache-Test)
