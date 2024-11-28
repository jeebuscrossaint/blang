cmake_minimum_required(VERSION 3.30.5)
project(KiddieScript)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(skid 
    src/main.c
    src/handler.c
    src/lexer.c)
