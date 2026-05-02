cmake_minimum_required(VERSION 3.10)
project(avl_tree C)

add_executable(avl_tree src/avl_tree.c src/main.c)
target_include_directories(avl_tree PRIVATE src)

add_executable(test_avl_tree src/avl_tree.c test/test.c)
target_include_directories(test_avl_tree PRIVATE src test)

enable_testing()
add_test(NAME avl_tests COMMAND test_avl_tree)
