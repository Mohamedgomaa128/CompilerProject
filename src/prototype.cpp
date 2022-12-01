
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
const char EPSILON = '#';

int main() {

	return 0;
}




class Node {
//private :


//public :
	int id;
	//set<Transation> children;
};

class Transation {
	Node *from, *to;
	char transVal;

	Transation(Node* f, Node *t, char val){
		this->from = f;
		this->to = t;
		this->transVal = val;
	}
};

class NFA {
	/*
	  NFA >> concat , union, kleane closure, positive closure
	 */

	Node *start, *end;
	set<Node *> states;
	map<Node *, set<Transation *>> transOfNodes;

	NFA(char val){
		addNode(this->start = new Node());
		addNode(this->end = new Node());
		addTrans(start, new Transation(start, end, val));
	}

	void addNode(Node *node){
		this->states.insert(node);
	}

	void addTrans(Node * node, Transation * t){
		this->transOfNodes[node].insert(t);
	}
	NFA * clone(){
		return NULL;
	}

	NFA* concat(NFA *first, NFA *second) {
		NFA * fcopy = first->clone();
		NFA * scopy = second->clone();
		Node * firstEnd = fcopy->end;
		Node * secondStart = scopy->start;
		// revise merging of the two maps
		fcopy->transOfNodes.merge(scopy->transOfNodes);
		fcopy->addTrans(firstEnd, new Transation(firstEnd, secondStart, EPSILON));
		return fcopy;
	}


	NFA* kleeneClosure(NFA *automata) {

		return NULL;
	}

	NFA* positiveClosure(NFA *automata) {

		return NULL;
	}

	NFA* unionAutomatas(NFA *first, NFA *second) {

		return NULL;
	}
};

/*
class RegToNFA {
	// thomson construction


};


class NFAToCNFA{
	// just a simple function

};

class CNFAToDFA{
	// subset construction

};

class Minimization{
	// choose a way for minimization

};




*/
