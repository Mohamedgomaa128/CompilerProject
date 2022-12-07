
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
using namespace std;

typedef vector<int> vi;
const char EPSILON = '#';





class Node {
//private :

	// constructor to give id incrementally

public :
	int id;
	bool acceptingNode;
	//set<Transation> children;
	Node * clone(){
		// temp function;
		Node * n = new Node();
		n->id = this->id;
		n->acceptingNode = this->acceptingNode;
		return n;
	}
};

class Transition {

	Node *from, *to;
	char transVal;
	Transition(){
		//empty constructor
	}

public:
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
public :
	Node *start, *end;
	set<Node *> nodes;
	map<Node *, set<Transition *>> transOfNodes;

	NFA(){
		//nulls
		//this->start = NULL;
	}
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
		// create a map to avoid repeating cloning and avoid pointers problems
		unordered_map<Node *, Node *> umap;
		for (Node * n : this->nodes)
			umap[n] = n->clone();

		NFA* nfa = new NFA();
		for (Node* n : this->nodes){
			nfa->addNode(umap[n]);
			for (Transition * t : this->transOfNodes[n])
				nfa->addTrans(umap[t->from], new Transition(umap[t->from], umap[t->to], t->transVal));
		}
		nfa->start = umap[this->start];
		nfa->end = umap[this->end];

		return nfa;
	}

	NFA* concat(NFA *first, NFA *second) {
		NFA * fcopy = first->clone();
		NFA * scopy = second->clone();

		// delete last node of first nfa
		fcopy->nodes.erase(fcopy->end);
		// move edges to the first node of second nfa
		for (Node* n : fcopy->nodes)
			for (Transition* t : fcopy->transOfNodes[n])
				if (t->to == fcopy->end)
					t->to = scopy->start;

		for (Node *n : scopy->nodes){
			fcopy->addNode(n); // add the nodes to the first

			// add the transations to the first
			for (Transition * t : scopy->transOfNodes[n])
				fcopy->addTrans(n, t);
		}

		fcopy->end = scopy->start;
		// return the concatinate
		return fcopy;
	}


	NFA* kleeneClosure(NFA *automata) {
		NFA * nfa = positiveClosure(automata);
		nfa->addTrans(nfa->start, new Transition(nfa->start, nfa->end, EPSILON));
		return nfa;
	}

	NFA* positiveClosure(NFA *automata) {
		NFA * nfa = automata->clone();

		// new nodes added
		Node * newStart = new Node();
		Node * newEnd = new Node();
		nfa->addNode(newStart);
		nfa->addNode(newEnd);

		// new epsilon trans
		nfa->addTrans(newStart, new Transition(newStart, nfa->start, EPSILON));
		nfa->addTrans(newStart, new Transition(nfa->end, newEnd, EPSILON));
		nfa->addTrans(nfa->end, new Transition(nfa->end, nfa->start, EPSILON));

		// new terminals
		nfa->start = newStart;
		nfa->end = newEnd;
		return nfa;
	}

	NFA* unionAutomatas(NFA *first, NFA *second) {
		NFA* f = first->clone();
		NFA* s = second->clone();

		// create new nodes
		Node* newStart = new Node();
		Node* newEnd = new Node();
		f->addNode(newStart);
		f->addNode(newEnd);

		// move second one to first one
		for (Node* n : s->nodes){
			f->addNode(n);
			for (Transition* t : s->transOfNodes[n])
				f->addTrans(n, t);
		}
		// 4 epsilon transition
		f->addTrans(newStart, new Transition(newStart, f->start, EPSILON));
		f->addTrans(newStart, new Transition(newStart, s->start, EPSILON));
		f->addTrans(f->end, new Transition(f->end, newEnd, EPSILON));
		f->addTrans(s->end, new Transition(s->end, newEnd, EPSILON));
		// change pointers
		f->start = newStart;
		f->end = newEnd;

		return f;
	}

	set<char> getInputSymbols(){
		set<char> inputSymbols;
		for (Node * n : nodes){
			for (Transition* t : this->transOfNodes[n])
				if (t->transVal != EPSILON)
					inputSymbols.insert(t->transVal);
		}

		return inputSymbols;
	}
};


class DFA {
	Node* stNode;
	map<Node*, set<Transition *>> nTrans;
	map<Node*, set<Transition *>> dTrans;
	map<set<Node *>, Node*> groupedStates;
	set<Node*> dStates;
	set<char > inputSymbols;




	void build(NFA* nfa){
		this->nTrans = nfa->transOfNodes;
		this->inputSymbols = nfa->getInputSymbols();

		set<Node*> n = epsClosure(this->stNode);
		groupedStates[n] = new Node();
		this->stNode = n;

		set<set<Node*>> statesBeforeMapping;
		statesBeforeMapping.insert(n);

		map<set<Node*>, bool> visited;
		visited[n] = false;

		while (!n.empty){
			visited[n] = true;
			modifyAcceptingState(groupedStates[n], n);


			for (char ch : this->inputSymbols){
				set<Node*> newNode = epsClosure(move(n, ch));
				if (!newNode.empty()){

					if (statesBeforeMapping.find(newNode) == statesBeforeMapping.end()){
						groupedStates[newNode] = new Node();
						statesBeforeMapping.insert(n);
						visited[n] = false;
						modifyAcceptingState(groupedStates[newNode], newNode);
					}
					dTrans[groupedStates[n]].insert(new Transition(groupedStates[n], groupedStates[newNode], ch));
				}
			}
			n = getUnvisited(visited);
		}
	}


	set<Node*> getUnvisited(map<set<Node*>, bool>& visited){
		set<Node*> empty;

		for (auto entry : visited)
			if (entry.second == false)
				return entry.first;

		return empty;
	}

	set<Node *> epsClosure(Node* n){
		set<Node *> ret = {n};
		stack<Node*> stk;
		stk.push(n);

		while (!stk.empty()){
			Node* node = stk.top();
			stk.pop();
			// go to next level of nodes >> if new add and dfs from it
			for (Transition * t : this->nTrans[node])
				if (t->transVal == EPSILON && t->to != nullptr
						&& ret.find(t->to) == ret.end()){
					ret.insert(t->to);
					stk.push(t->to);
				}
		}

		return ret;
	}

	set<Node *> epsClosure(set<Node *> n){
		set<Node *> ret(n.begin(), n.end());

		for (Node * node : n){
			set<Node *> eps = epsClosure(node);
			ret.insert(eps.begin(), eps.end());
		}

		return ret;
	}
	set<Node*> move(Node* n, char ch){
		set<Node *> nextNodes;
		set<Transition*> transi = this->nTrans[n];
		for (Transition* t : transi)
			if (t->transVal == ch)
				nextNodes.insert(t->to);

		return nextNodes;
	}

	set<Node*> move(set<Node*> n, char ch){
		set<Node *> nextNodes;
		for (Node* node : n){
			set<Node *> ret = move(node, ch);
			if (!ret.empty())
				nextNodes.insert(ret.begin(), ret.end());
		}

		return nextNodes;
	}

	void modifyAcceptingState(Node* newNode, set<Node*>& old){
		for (Node * node : old)
			if (node->acceptingNode){
				newNode->acceptingNode = true;

				//
				return;
			}
		newNode->acceptingNode = false;
	}

};


