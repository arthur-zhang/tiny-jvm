file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})

message(${SOURCES})
add_library(shared SHARED ${SOURCES})