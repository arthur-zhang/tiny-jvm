find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(../../src/shared)

add_executable(classfile_test class_reader_test.cpp string_test.cpp)

target_link_libraries(classfile_test
        ${GTEST_BOTH_LIBRARIES}
        pthread ujvm classfile shared)

gtest_discover_tests(classfile_test)