cmake_minimum_required(VERSION 3.24)
project(CompilerProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(CompilerProject
        CompilerProject-main/src/prototype.cpp
        src/Element.cpp
        src/Element.h
        src/LeftFactoring.cpp
        src/LeftFactoring.h
        src/LeftRecursion.cpp
        src/LeftRecursion.h
        src/Nonterminal.cpp
        src/Nonterminal.h
        src/parser.cpp
        src/Production.cpp
        src/Production.h
        src/prototype.cpp
        src/Terminal.cpp
        src/Terminal.h)
