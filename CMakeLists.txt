cmake_minimum_required (VERSION 3.21)
project (PROJECT)
enable_testing()

add_executable(naive src/naive.c src/naive.h)
add_executable(lin src/lin.c)
add_executable(testing test/tests.c)

