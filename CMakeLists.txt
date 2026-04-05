cmake_minimum_required(VERSION 3.10)
project(second_semester C)
add_executable(csv_printer src/csv_printer.c src/main.c)
target_include_directories(csv_printer PRIVATE src)
