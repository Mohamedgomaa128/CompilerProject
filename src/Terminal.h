//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_TERMINAL_H
#define COMPILERPROJECT_MAIN_TERMINAL_H

#include "Element.h"
#include "vector"

class Terminal : public Element{
private:
    bool epsilon;
public:
    bool isEpsilon() {
        return epsilon;
    }
};


#endif //COMPILERPROJECT_MAIN_TERMINAL_H
