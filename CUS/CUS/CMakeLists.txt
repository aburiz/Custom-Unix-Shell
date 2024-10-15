cmake_minimum_required(VERSION 3.10)
project(my_shell)

set(CMAKE_C_STANDARD 11)

add_executable(shell main.c parser.c)

