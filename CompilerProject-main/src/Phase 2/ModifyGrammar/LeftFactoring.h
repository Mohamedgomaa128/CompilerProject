//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_LEFTFACTORING_H
#define COMPILERPROJECT_MAIN_LEFTFACTORING_H
#include "../Nonterminal.h"

class LeftFactoring {
private:
    Nonterminal start;
public:
    void setStart(Nonterminal startSymbol){
        start = startSymbol;
    }
    vector<Nonterminal> eachNonTerminalLF(Nonterminal nonterminal);
    vector<Nonterminal> removeLeftFactoring(vector<Nonterminal> originalNT);
};


#endif //COMPILERPROJECT_MAIN_LEFTFACTORING_H
