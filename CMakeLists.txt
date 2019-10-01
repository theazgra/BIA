cmake_minimum_required(VERSION 3.10)
project(bia)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(AzgraCppLibrary)

add_executable(bia main.cpp)

target_link_libraries(bia sub::azgra)
set_property(TARGET bia PROPERTY CXX_STANDARD 17)

if(MATPLOTLIB_CPP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATPLOTLIB_CPP")
endif()