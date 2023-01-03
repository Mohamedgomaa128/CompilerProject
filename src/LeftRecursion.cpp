//
// Created by Samab on 1/1/2023.
//

#include "LeftRecursion.h"
#include "Terminal.h"

vector<Nonterminal> LeftRecursion::removeLeftRecursion(vector<Nonterminal> originalNT, Nonterminal nonterminal) {
    for (int i = 0; i < originalNT.size(); ++i) {
        vector<Nonterminal> outputNonTerminals = eachNonTerminalLR(originalNT[i]);
        if (outputNonTerminals.size()!= 1)
        {
            originalNT.erase(originalNT.begin() + i);
            for (int j = 0; j < outputNonTerminals.size(); ++j) {
                originalNT.push_back(outputNonTerminals[j]);
            }
            i = -1;
        }
    }
    return vector<Nonterminal>();
}

vector<Nonterminal> LeftRecursion::eachNonTerminalLR(Nonterminal nonterminal) {
    vector<Production*> LRproductions;
    vector<Production*> NLRproductions;
    vector<Nonterminal> outputNonTerminals;
    vector<Production*> productions = nonterminal.getProductions();
    bool LR = false;
    for (int i = 0; i < productions.size() ; ++i) {
        if (productions[i]->getElement(0)->getName() == nonterminal.getElement()->getName())
        {
            LR = true;
            break;
        }
    }
    if (!LR)
    {
        outputNonTerminals.push_back(nonterminal);
        return outputNonTerminals;
    }
    for (int i = 0; i < productions.size() ; ++i) {
        if (productions[i]->getElement(0)->getName() == nonterminal.getElement()->getName())
        {
            productions[i]->removeFirstElement();
            LRproductions.push_back(productions[i]);
        } else
        {
            NLRproductions.push_back(productions[i]);
        }
    }
    Nonterminal* nonterminalDash = new Nonterminal();
    vector<Production*> nonterminalProductions;

    for (int i = 0; i < LRproductions.size(); ++i) {
        LRproductions[i]->pushElements(nonterminalDash->getElement());
        nonterminalDash->pushProductions(LRproductions[i]);
    }

    if (!NLRproductions.empty())
    {
        Terminal* epsilon = new Terminal();
        Production* epsilonProduction = new Production();
        epsilonProduction->pushElements(epsilon);
        nonterminalDash->pushProductions(epsilonProduction);
    }

    for (int i = 0; i < NLRproductions.size(); ++i) {
        NLRproductions[i]->pushElements(nonterminalDash->getElement());
        nonterminalProductions.push_back(NLRproductions[i]);
    }

    if (!NLRproductions.empty())
    {
        Production* production =  new Production();
        production->pushElements(nonterminalDash);
        nonterminalProductions.push_back(production);
    }
    nonterminal.setProductions(nonterminalProductions);
    outputNonTerminals.push_back(nonterminal);
    outputNonTerminals.push_back(*nonterminalDash);

    return outputNonTerminals;
}

