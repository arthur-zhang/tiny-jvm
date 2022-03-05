find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(my_test class_reader_test.cpp)

target_link_libraries(my_test
        ${GTEST_BOTH_LIBRARIES}
        pthread jvm_test_lib)

gtest_discover_tests(my_test)