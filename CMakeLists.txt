cmake_minimum_required(VERSION 3.26)
project(search_image C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(search_image
        fileUtils.c
        fileUtils.h
        generateID.c
        generateID.h
        main.c
        summary.c
        summary.h)
