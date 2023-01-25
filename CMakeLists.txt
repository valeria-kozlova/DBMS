cmake_minimum_required(VERSION 3.22)
project(db)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(db record.h cmd.h manager.h database.h record.cpp cmd.cpp manager.cpp database.cpp main.cpp)
