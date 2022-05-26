file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})

message(${SOURCES})
set(INCLUDE_LIST ../include/shared/fs.h ../include/shared/jstring.h)
add_executable(tiny_jvm
        ${SOURCES}
        ${INCLUDE_LIST}
        )

add_library(jvm_test_lib STATIC ${SOURCES})
