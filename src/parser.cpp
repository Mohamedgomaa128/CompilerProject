
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
using namespace std;


class Element {
	// generalization for terminals and nonterminals
	int id;
	Element(int id){
		this->id = id;
	}
};

class Terminal : Element {

	Terminal(int id) : Element(id){

	}
};

class NonTerminal {
	NonTerminal(int id) : Element(id){

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

