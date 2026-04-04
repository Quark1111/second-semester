cmake_minimum_required(VERSION 3.10)
project(states C)
add_executable(states src/graph.c src/heap.c src/readfile.c src/states.c)
target_include_directories(states PRIVATE src)
