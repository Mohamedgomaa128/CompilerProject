//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_ELEMENT_H
#define COMPILERPROJECT_MAIN_ELEMENT_H
#include <vector>
#include <string>

using namespace std;

class Symbol {
public:
    Symbol(string name);
    string name;
    string getName(){
        return name;
    }
    void setName(string n){
        name = n;
    }
};


#endif //COMPILERPROJECT_MAIN_ELEMENT_H
