//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_PRODUCTION_H
#define COMPILERPROJECT_MAIN_PRODUCTION_H
#include "vector"
#include "Symbol.h"

using namespace std;

class Production {

public:
    vector<Symbol*> symbols;
    vector<Symbol*> getSymbols(){
        return symbols;
    }
    Symbol* getSymbol(int i){
        return symbols[i];
    }
    string getSymbolName(int i){
        return symbols[i]->getName();
    }

    void pushSymbols(Symbol* symbol){
        symbols.push_back(symbol);
    }
    void removeFirstSymbol(){
        symbols.erase(symbols.begin());
    }
};


#endif //COMPILERPROJECT_MAIN_PRODUCTION_H
