//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_TERMINAL_H
#define COMPILERPROJECT_MAIN_TERMINAL_H

#include "Symbol.h"
#include "vector"

class Terminal : public Symbol{
private:
    bool epsilon;
public:
    Terminal(string name): Symbol(name){};
    bool isEpsilon() {
        return epsilon;
    }
};


#endif //COMPILERPROJECT_MAIN_TERMINAL_H
