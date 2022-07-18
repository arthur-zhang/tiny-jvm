file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp)
set(SOURCES ${SOURCES})
set(CMAKE_MODULE_PATH "${CMCMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake")

find_package(FFI REQUIRED)
include_directories(${FFI_INCLUDE_DIRS})
link_libraries(${FFI_LIBRARIES})

message(${SOURCES})
set(INCLUDE_LIST
        shared/fs.h
        shared/jstring.h
        shared/types.h
        shared/vm_utils.h
        )
add_library(java SHARED ${SOURCES} ${INCLUDE_LIST})

#target_link_libraries(java classfile shared ujvm ffi)
#add_library(jvm_test_lib STATIC ${SOURCES})
add_subdirectory(classfile)
add_subdirectory(shared)
add_subdirectory(bytecode)
add_subdirectory(runtime)
add_subdirectory(native)
add_subdirectory(jni)
add_subdirectory(classpath)
