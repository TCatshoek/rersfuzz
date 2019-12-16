cmake_minimum_required(VERSION 3.12)
project(rersfuzz C)

set(CMAKE_C_STANDARD 11)

add_executable(rersfuzz main.c trie.h stringstorage.h util.h)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cwalk)

target_link_libraries(rersfuzz cwalk)