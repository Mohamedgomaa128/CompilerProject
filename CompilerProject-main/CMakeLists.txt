cmake_minimum_required(VERSION 3.24)
project(CompilerProject_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(CompilerProject_main
        src/prototype.cpp "../src/LeftRecursion.cpp" "../src/LeftRecursion.h" "../src/LeftFactoring.cpp" "../src/LeftFactoring.h" "../src/Element.cpp" "../src/Element.h" "../src/Terminal.cpp" "../src/Terminal.h" "../src/Nonterminal.cpp" "../src/Nonterminal.h" "../src/Production.cpp" "../src/Production.h")
