file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})
#set(INCLUDE_LIST
#        fs.h
#        jstring.h
#        vm_utils.h
#        )

include_directories(../shared)
message(${SOURCES})
#add_library(ujvm SHARED ${SOURCES} ${INCLUDE_LIST})
add_library(ujvm SHARED ${SOURCES})
target_link_libraries(ujvm classfile shared)