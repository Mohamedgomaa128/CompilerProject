//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_LEFTRECURSION_H
#define COMPILERPROJECT_MAIN_LEFTRECURSION_H


#include "Phase 2/Nonterminal.h"

class LeftRecursion {
private:
    Nonterminal start;
public:
    void setStart(Nonterminal startSymbol){
        start = startSymbol;
    }
    vector<Nonterminal> eachNonTerminalLR(Nonterminal nonterminal);
    vector<Nonterminal> removeLeftRecursion(vector<Nonterminal> originalNT, Nonterminal startSymbol);



};


#endif //COMPILERPROJECT_MAIN_LEFTRECURSION_H
