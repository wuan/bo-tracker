cmake_minimum_required(VERSION 3.1)

project(bo-tracker CXX)

project(foobar CXX)
message("Your C++ compiler supports these C++ features:")
foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
    message("${i}")
endforeach()

add_subdirectory(source)

