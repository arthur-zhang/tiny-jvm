find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(INCLUDE_LIST ../src/shared/fs.h ../src/shared/jstring.h)

add_executable(my_test ${INCLUDE_LIST} class_reader_test.cpp string_test.cpp)

target_link_libraries(my_test
        ${GTEST_BOTH_LIBRARIES}
        pthread java ujvm classfile shared)

gtest_discover_tests(my_test)