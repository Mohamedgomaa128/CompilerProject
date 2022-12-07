
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
using namespace std;

typedef vector<int> vi;
const char EPSILON = '#';





class Node {

public :
	int id;
	bool acceptingNode;

	Node * clone(){
		// temp function;
		Node * n = new Node();
		n->id = this->id;
		n->acceptingNode = this->acceptingNode;
		return n;
	}
};

class Transition {
public:
	Node* from;
	Node* to;
	char transVal;

public:
	Transition(Node* f, Node *t, char val){
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
	bool buildOrMin; // build >> nTrans,

	DFA(NFA* nfa){
		buildOrMin = true;
		this->build(nfa);
		buildOrMin = false;
		this->build(nfa);
	}


	void build(NFA* nfa){

		this->nTrans = nfa->transOfNodes;
		this->inputSymbols = nfa->getInputSymbols();

		set<Node*> n = epsClosure(this->stNode);
		groupedStates[n] = new Node();
		this->stNode = groupedStates[n];

		set<set<Node*>> statesBeforeMapping;
		statesBeforeMapping.insert(n);

		map<set<Node*>, bool> visited;
		visited[n] = false;

		// loop on each unvisited states and get epsClosure and possible moves getting all levels and add them to the dfa
		while (!n.empty()){
			//mark as visited
			visited[n] = true;
			modifyAcceptingState(groupedStates[n], n);

			// next level of nodes by moving and getting epsClosure
			for (char ch : this->inputSymbols){
				set<Node*> newNode = epsClosure(move(n, ch));
				if (!newNode.empty()){ // added to the next level of n
					//if not found >> add i
					if (statesBeforeMapping.find(newNode) == statesBeforeMapping.end()){
						groupedStates[newNode] = new Node();
						statesBeforeMapping.insert(newNode);
						visited[newNode] = false;
						modifyAcceptingState(groupedStates[newNode], newNode);
					}
					// add the transition
					dTrans[groupedStates[n]].insert(new Transition(groupedStates[n], groupedStates[newNode], ch));
				}
			}
			n = getUnvisited(visited);
		}
	}


	set<Node*> getUnvisited(map<set<Node*>, bool>& visited){
		//get first unvisited form the visited map

		set<Node*> empty;

		for (auto entry : visited)
			if (entry.second == false)
				return entry.first;

		return empty;
	}

	set<Node *> epsClosure(Node* n){
		//get epsilon closure of set of a Node
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
		//get epsilon closure of set of Nodes
		set<Node *> ret(n.begin(), n.end());

		for (Node * node : n){
			set<Node *> eps = epsClosure(node);
			ret.insert(eps.begin(), eps.end());
		}

		return ret;
	}

	set<Node*> move(Node* n, char ch){
		// move a node by a char

		set<Node *> nextNodes;
		// for build or minimization
		set<Transition*> transi = (buildOrMin ? this->nTrans[n] : this->dTrans[n]);
		for (Transition* t : transi)
			if (t->transVal == ch)
				nextNodes.insert(t->to);

		return nextNodes;
	}

	set<Node*> move(set<Node*> n, char ch){
		// move a set of nodes by char
		set<Node *> nextNodes;
		for (Node* node : n){
			set<Node *> ret = move(node, ch);
			if (!ret.empty())
				nextNodes.insert(ret.begin(), ret.end());
		}

		return nextNodes;
	}

	void modifyAcceptingState(Node* newNode, set<Node*>& old){
		// if one of old node is accepting convert current to accepting
		for (Node * node : old)
			if (node->acceptingNode){
				newNode->acceptingNode = true;

				//
				return;
			}
		newNode->acceptingNode = false;
	}

	void minimize() {
		set<set<Node*>> groups;
		unordered_map<bool, set<Node*>> mapp;
		// first to two groups

		for (auto entiry : groupedStates){
			Node* n = entiry.second;
			mapp[n->acceptingNode].insert(n);
		}

		groups.insert(mapp[true]);
		groups.insert(mapp[false]);

		// Continually partition them into groups according to input till be minimum
		bool notMinimized = true;
		while (notMinimized){
			set<set<Node*>> newGroups(groups.begin(), groups.end());
			for (char ch : inputSymbols)
				newGroups = partition(newGroups, ch);
			if (newGroups == groups)
				notMinimized = false;

			groups = newGroups;
		}
	}

	set<set<Node*>> partition(set<set<Node*>>& groups, char ch){

		set<set<Node*>> newGroups;

		for (set<Node*> g : groups){
			// choose a set
			unordered_map<Node*, bool> visited;

			for (Node* node : g)
				visited[node] = false;

			//select each unvisited node and loop on other O(n^2) passing
			set<Node*>::iterator it;
			for (it = g.begin(); it != g.end(); it++){
				Node* node1 = *it;
				if (visited[node1])
					continue;

				set<Node*> subGroup;
				subGroup.insert(node1);
				visited[node1] = true;

				set<Node*>::iterator secondIt = next(it, 1);
				for (; secondIt != g.end(); secondIt++){
					Node* node2 = *secondIt;

					if (visited[node2])
						continue;
					//get nodes go to same state and add them to same group

					if (goToSameGroup(groups, node1, node2, ch)){
						subGroup.insert(node2);
						visited[node2] = true;
					}
				}
				// now add new group
				newGroups.insert(subGroup);
			}
		}

		return newGroups;
	}

	bool goToSameGroup(set<set<Node*>>& groups, Node* n1, Node* n2, char ch){
		// to same group ?
		set<Node*> next1 = move(n1, ch);
		set<Node*> next2 = move(n2, ch);

		if (next1.empty() && next2.empty())
			return true;

		// different
		if ((next1.empty() && !next2.empty()) || (!next1.empty() && next2.empty()))
			return false;

		Node* nn1 = *next1.begin();
		Node* nn2 = *next2.begin();

		// loop till found group where they go to
		for (set<Node*> group : groups)
			if (group.find(nn1) != group.end() && group.find(nn2) != group.end())
				return true;

		return false;
	}

	void mapGroupsToStates(set<set<Node*>> groups){
		unordered_map<Node*, Node*> repStates;
		for (set<Node*> group : groups){
			// for each set choose a representative
			Node* repNode = *group.begin();
			repStates[repNode] = repNode;

			for (Node * node : group)
				repStates[node] = repNode;
		}

		// add new nodes
		for (auto entiry : repStates)
			dStates.insert((Node *) entiry.second);

		map<Node*, set<Transition*>> newTrans;

		// loop on each and add trans
		for (set<Node*> group : groups){
			Node* repNode = repStates[*group.begin()];
			// all nodes in same group has same transitions >> to same group
			for (Transition* t : dTrans[repNode])
				newTrans[repNode].insert(new Transition(repNode, repStates[t->to], t->transVal));
		}

		dTrans = newTrans;
	}


};


