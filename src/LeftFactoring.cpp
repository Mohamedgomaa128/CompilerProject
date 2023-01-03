//
// Created by Samab on 1/1/2023.
//

#include "LeftFactoring.h"
#include "Terminal.h"


vector<Nonterminal> LeftFactoring::removeLeftFactoring(vector<Nonterminal> originalNT) {
    for (int i = 0; i < originalNT.size(); ++i) {
        vector<Nonterminal> outputNonTerminals = eachNonTerminalLF(originalNT[i]);
        if (outputNonTerminals.size()!= 1)
        {
            originalNT.erase(originalNT.begin() + i);
            for (int j = 0; j < outputNonTerminals.size(); ++j) {
                originalNT.push_back(outputNonTerminals[j]);
            }
            i = -1;
        }
    }

    return originalNT;
}

vector<Nonterminal> LeftFactoring::eachNonTerminalLF(Nonterminal nonterminal) {
    vector<Production*> productions = nonterminal.getProductions();
    vector<Nonterminal> outputNonTerminals;
    for (int i = 0; i < productions.size(); ++i) {
        int factorSize = INT64_MAX;
        vector<Production*> matchingProductions;
        matchingProductions.push_back(productions[i]);
        //get matching productions
        for (int j = i + 1; j < productions.size(); ++j) {
            vector<Element*> iElements = productions[i]->getElements();
            vector<Element*> jElements = productions[j]->getElements();
            int size = 0;
            for(; size < min(iElements.size(), jElements.size()); size++) {
                if (iElements[size]->getName() != jElements[size]->getName())
                    break;
            }
            if (size != 0)
            {
                //get the minimum factor size
                factorSize = min(factorSize, size);
                matchingProductions.push_back(productions[j]);
                productions.erase(productions.begin() + j);
                j = i;
            }
        }
        if (matchingProductions.size() != 1)
        {
            Production* factor = new Production;
            vector<Element*> commonElements = productions[i]->getElements();
            //get the factor
            for (int j = 0; j < factorSize; ++j) {
                factor->pushElements(productions[i]->getElement(j));
            }

            productions.erase(productions.begin() + i);

            //remove the factor from the matching productions
            for (int j = 0; j < matchingProductions.size(); ++j) {
                for (int k = 0; k < factorSize; ++k) {
                    matchingProductions[j]->removeFirstElement();
                }
                //epsilon case
                if (matchingProductions[j]->getElements().empty())
                {
                    Terminal* epislon = new Terminal();        ///////<================================
                    matchingProductions[j]->pushElements(epislon);
                }
            }
            Nonterminal* nonterminalDash = new Nonterminal();
            for (int j = 0; j < matchingProductions.size(); ++j) {
                nonterminalDash->pushProductions(matchingProductions[j]);
            }
            outputNonTerminals.push_back(*nonterminalDash);
            factor->pushElements(nonterminalDash->getElement());  ////////////<===================
            productions.push_back(factor);
            ///
            i = -1;
        }
    }
    nonterminal.setProductions(productions);
    outputNonTerminals.push_back(nonterminal);
    return outputNonTerminals;
}
