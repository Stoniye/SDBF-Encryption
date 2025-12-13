cmake_minimum_required(VERSION 4.0)
project(SDBF_Encryption C)

set(CMAKE_C_STANDARD 11)

add_executable(SDBF_Encryption
        main.c
        xxhash/xxhash.c
)