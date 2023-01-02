
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
using namespace std;


class Element {
	// generalization for terminals and nonterminals
	int id;
};

class Terminal {
	Element* e;
	public :
	Terminal(Element* ee) {
		this->e = ee;
	}
};

class NonTerminal {
	Element* e;
	public :
	NonTerminal(Element* ee) {
		this->e = ee;
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

