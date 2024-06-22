cmake_minimum_required(VERSION 3.28)
project(Jwatrio C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-fexec-charset=CP949")

add_executable(Jwatrio main.c
        render/render.h
        render/render.c
        render/text.c
        screen/network.c
        screen/home.c)
