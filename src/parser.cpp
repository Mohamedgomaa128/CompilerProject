
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
using namespace std;


class Element {
	// generalization for terminals and nonterminals
	string word;
	Element(string word){
		this->word = word;
	}
};

class Terminal : Element {

	Terminal(string word) : Element(word){

	}
};

class NonTerminal {
	NonTerminal(string word) : Element(word){

	}
};

class Production {
	NonTerminal* lhs;
	vector<vector<Element*>> rhs; // means sequence of non terminals and terminals
};



class ContextFreeGrammar { // calculates follow and first
	vector<Production*> productions;
};

class CFGBuilder { // builds cfg from input
	ContextFreeGrammar* cfg;
};







int main() {

}

