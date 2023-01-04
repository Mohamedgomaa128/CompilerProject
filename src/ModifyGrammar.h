//
// Created by Samab on 1/4/2023.
//

#ifndef COMPILERPROJECT_MODIFYGRAMMAR_H
#define COMPILERPROJECT_MODIFYGRAMMAR_H

#include <vector>
#include "Nonterminal.h"
#include "LeftFactoring.h"
#include "LeftRecursion.h"
using namespace std;
class ModifyGrammar {
public:
    Nonterminal* startSymbol;
    vector<Nonterminal> facade(Nonterminal* start, vector<Nonterminal> nonterminals){
        startSymbol = start;
        LeftFactoring lf;
        lf.setStart(start);
        vector<Nonterminal> r1 = lf.removeLeftFactoring(nonterminals);
        LeftRecursion lr;
        lr.setStart(start);
        return lr.removeLeftRecursion(r1, start);
    }
};


#endif //COMPILERPROJECT_MODIFYGRAMMAR_H
