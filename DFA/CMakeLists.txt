cmake_minimum_required(VERSION 3.10)
project(dfa C)
add_executable(dfa src/dfa.c src/main.c)
target_include_directories(dfa PRIVATE src)
