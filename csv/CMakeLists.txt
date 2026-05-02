cmake_minimum_required(VERSION 3.10)
project(csv_printer C)

add_executable(csv_printer src/csv_printer.c src/main.c)
target_include_directories(csv_printer PRIVATE src)

add_executable(test_csv_printer src/csv_printer.c Tests/test.c)
target_include_directories(test_csv_printer PRIVATE src Tests)

enable_testing()
add_test(NAME csv_tests COMMAND test_csv_printer)
