cmake_minimum_required(VERSION 3.27)

project(example)

add_executable(example_join src/example_join.cpp)

add_executable(example_detach src/example_detach.cpp)

set_target_properties(example_join example_detach PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

add_subdirectory(lib/repl)

target_include_directories(example_detach SYSTEM PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(example_join SYSTEM PUBLIC ${CMAKE_SOURCE_DIR})