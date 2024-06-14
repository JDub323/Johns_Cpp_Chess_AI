cmake_minimum_required(VERSION 3.27)
project(Johns_Cpp_Chess_AI)

set(CMAKE_CXX_STANDARD 20)

add_executable(Johns_Cpp_Chess_AI main.cpp
        Position.cpp
        Position.h
        Utilities.cpp
        UCIHandler.cpp
        UCIHandler.h
        Utilities.h
        Pieces.h
        ChessConstants.h
        Zobrists.h
        Movement.cpp
        Movement.h
        unittests.cpp
        unittests.h
        Magics.h)

add_executable(unittests unittests.cpp)