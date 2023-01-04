//
// Created by Samab on 1/1/2023.
//

#include "LeftRecursion.h"
#include "Terminal.h"

vector<Nonterminal> LeftRecursion::removeLeftRecursion(vector<Nonterminal> originalNT, Nonterminal* nonterminal) {
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
    return originalNT;
}

vector<Nonterminal> LeftRecursion::eachNonTerminalLR(Nonterminal nonterminal) {
    vector<Production*> LRproductions;
    vector<Production*> NLRproductions;
    vector<Nonterminal> outputNonTerminals;
    vector<Production*> productions = nonterminal.getProductions();
    bool epsilon = false;
    bool LR = false;
    for (int i = 0; i < productions.size() ; ++i) {
        if (productions[i]->getSymbol(0)->getName() == nonterminal.getName())
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
        if (productions[i]->getSymbol(0)->getName() == nonterminal.getName())
        {
            productions[i]->removeFirstSymbol();
            LRproductions.push_back(productions[i]);
        } else
        {
            if (productions[i]->getSymbol(0)->getName() == "eps")
                epsilon = true;
            else
                NLRproductions.push_back(productions[i]);
        }
    }
    Nonterminal* nonterminalDash = new Nonterminal(nonterminal.getName() + "DashLR");
    vector<Production*> nonterminalProductions;

    for (int i = 0; i < LRproductions.size(); ++i) {
        LRproductions[i]->pushSymbols(nonterminalDash);
        nonterminalDash->pushProductions(LRproductions[i]);
    }

    if (!NLRproductions.empty() || epsilon)
    {
        Terminal* epsilon = new Terminal("eps");
        Production* epsilonProduction = new Production();
        epsilonProduction->pushSymbols(epsilon);
        nonterminalDash->pushProductions(epsilonProduction);
    }

    for (int i = 0; i < NLRproductions.size(); ++i) {
        NLRproductions[i]->pushSymbols(nonterminalDash);
        nonterminalProductions.push_back(NLRproductions[i]);
    }

    if (NLRproductions.empty())
    {
        Production* production =  new Production();
        production->pushSymbols(nonterminalDash);
        nonterminalProductions.push_back(production);
    }
    nonterminal.setProductions(nonterminalProductions);
    outputNonTerminals.push_back(nonterminal);
    outputNonTerminals.push_back(*nonterminalDash);

    return outputNonTerminals;
}

