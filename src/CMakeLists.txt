file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})

message(${SOURCES})
add_executable(tiny_jvm
        classfile/jvm_main.cpp
        ${SOURCES}
        )

add_library(jvm_test_lib STATIC ${SOURCES})
