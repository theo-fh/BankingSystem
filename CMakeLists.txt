cmake_minimum_required(VERSION 4.1)
project(Erstes_Projekt C)

set(CMAKE_C_STANDARD 11)

add_executable(Erstes_Projekt main.c
)

target_link_libraries(Erstes_Projekt m)
