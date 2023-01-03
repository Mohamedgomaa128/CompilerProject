cmake_minimum_required(VERSION 3.24)
project(CompilerProject_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(CompilerProject_main
        src/prototype.cpp "src/Phase 2/ModifyGrammar/LeftRecursion.cpp" "src/Phase 2/ModifyGrammar/LeftRecursion.h" "src/Phase 2/ModifyGrammar/LeftFactoring.cpp" "src/Phase 2/ModifyGrammar/LeftFactoring.h" "src/Phase 2/Element.cpp" "src/Phase 2/Element.h" "src/Phase 2/Terminal.cpp" "src/Phase 2/Terminal.h" "src/Phase 2/Nonterminal.cpp" "src/Phase 2/Nonterminal.h" "src/Phase 2/Production.cpp" "src/Phase 2/Production.h")
