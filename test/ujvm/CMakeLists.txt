find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(../../src/shared)

add_executable(my_test helloworld_test.cpp)

target_link_libraries(my_test
        ${GTEST_BOTH_LIBRARIES}
        pthread classfile runtime shared classpath bytecode)

gtest_discover_tests(my_test)