
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

	// constructor to give id incrementally

//public :
	int id;
	//set<Transation> children;
};

class Transition {

	Node *from, *to;
	char transVal;
	Transition(){
		//empty constructor
	}

	Transition(Node* f, Node *t, char val){
		Transition();
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
	set<Node *> nodes;
	map<Node *, set<Transition *>> transOfNodes;


	NFA(char val){
		addNode(this->start = new Node());
		addNode(this->end = new Node());
		addTrans(start, new Transition(start, end, val));
	}

	void addNode(Node *node){
		this->nodes.insert(node);
	}

	void addTrans(Node * node, Transition * t){
		this->transOfNodes[node].insert(t);
	}
	NFA * clone(){
		return NULL;
	}

	NFA* concat(NFA *first, NFA *second) {
		NFA * fcopy = first->clone();
		NFA * scopy = second->clone();

		// delete last node of first nfa
		fcopy->nodes.erase(fcopy->end);
		// move edges to the first node of second nfa
		for (Node* n : fcopy->nodes)
			for (Transition* t : this->transOfNodes[n])
				if (t->to == fcopy->end)
					t->to = scopy->start;

		for (Node *n : scopy->nodes){
			fcopy->addNode(n); // add the nodes to the first

			// add the transations to the first
			for (Transition * t : scopy->transOfNodes[n])
				fcopy->addTrans(n, t);
		}

		fcopy->end = scopy->start;


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
