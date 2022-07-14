file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})
set(CMAKE_MODULE_PATH "${CMCMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake")

find_package(FFI REQUIRED)
include_directories(${FFI_INCLUDE_DIRS})
link_libraries(${FFI_LIBRARIES})

message(${SOURCES})
set(INCLUDE_LIST ../include/shared/fs.h ../include/shared/jstring.h ../include/shared/types.h)
add_library(ujvm SHARED ${SOURCES} ${INCLUDE_LIST})

target_link_libraries(ujvm ffi)
#add_library(jvm_test_lib STATIC ${SOURCES})
