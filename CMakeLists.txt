cmake_minimum_required(VERSION 3.29)
project(ArquiII_Proyecto2)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArquiII_Proyecto2 main.cpp
        Cache.cpp
        Cache.h
        SharedMemory.cpp
        SharedMemory.h
        BusInterconnect.cpp
        BusInterconnect.h
)
