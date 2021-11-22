# cmake_minimum_required(VERSION <specify CMake version here>)
project(synergy)

set(CMAKE_CXX_STANDARD 14)

add_executable(synergy main.cpp Huffman.h MinHeapWrapper.h Entry.h Node.h VectorExtension.h Serializer.h Serializable.h)