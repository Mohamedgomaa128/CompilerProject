//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_NONTERMINAL_H
#define COMPILERPROJECT_MAIN_NONTERMINAL_H


#include "Symbol.h"
#include "vector"
#include "Production.h"
using namespace std;
class Nonterminal : public Symbol{
private:
    vector<Production*> productions;
public:
    Nonterminal(string name): Symbol(name){};
    Production* getProduction(int i){
        return productions[i];
    }
    vector<Production*> getProductions(){
        return productions;
    }
    void pushProductions(Production* production){
        productions.push_back(production);
    }
    void setProductions(vector<Production*> newProductions){
        productions.clear();
        productions = newProductions;
    }
};


#endif //COMPILERPROJECT_MAIN_NONTERMINAL_H
