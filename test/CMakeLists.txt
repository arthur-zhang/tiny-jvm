find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(my_test data_output_stream_test.cpp)

target_link_libraries(my_test
        ${GTEST_BOTH_LIBRARIES}
        pthread jvm_test_lib)

gtest_discover_tests(my_test)