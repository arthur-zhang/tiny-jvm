file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})
set(CMAKE_MODULE_PATH "${CMCMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake")

include_directories(../shared)
message(${SOURCES})
add_library(classfile SHARED ${SOURCES})
target_link_libraries(classfile shared )