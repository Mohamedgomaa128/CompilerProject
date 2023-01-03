//
// Created by Samab on 1/1/2023.
//

#ifndef COMPILERPROJECT_MAIN_PRODUCTION_H
#define COMPILERPROJECT_MAIN_PRODUCTION_H
#include "vector"
#include "Element.h"

using namespace std;

class Production {
private:
    vector<Element*> elements;
public:
    vector<Element*> getElements(){
        return elements;
    }
    Element* getElement(int i){
        return elements[i];
    }
    string getElementName(int i){
        return elements[i]->getName();
    }
    void pushElements(Element* element){
        elements.push_back(element);
    }
    void removeFirstElement(){
        elements.erase(elements.begin());
    }
};


#endif //COMPILERPROJECT_MAIN_PRODUCTION_H
