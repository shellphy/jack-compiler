cmake_minimum_required (VERSION 2.8)

project (jack)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "GCC" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories (include)
add_executable(${PROJECT_NAME} src/jack.cpp src/VM.cpp)
