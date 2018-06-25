cmake_minimum_required (VERSION 2.8)
project(jackc)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "GCC" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories (include)
aux_source_directory (src JACKC_LIB_SRC)
add_library (jackclib "${JACKC_LIB_SRC}")
add_executable (${PROJECT_NAME} driver/jackc.cpp)
target_link_libraries (${PROJECT_NAME} jackclib)
