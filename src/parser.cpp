
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>
#include <exception>
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
Terminal* EPSILON = new Terminal("eps");
Terminal* DOLLER_SIGN = new Terminal("$");

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
	NonTerminal* startSymb; // productions[0]->lhs;
	unordered_map<NonTerminal *, unordered_set<Terminal *>> firsts;
	unordered_map<NonTerminal *, unordered_set<Terminal *>> follows;
	unordered_set<NonTerminal *, bool> firstDone;	// we will work  dfs-like >> so we need visited array
	unordered_set<NonTerminal *, bool> followDone;
	unordered_map<NonTerminal *, Production*> mapper;







	bool isEpsilon(Element* e){
		return e == EPSILON;
	}

	bool isTerminal(Element* e){
		return dynamic_cast<Terminal*>(e) != nullptr;
	}

	// return first of this nonTerminal and checks for epsilon
	unordered_set<Terminal *> getFirstAndCheckEps(NonTerminal* e, bool &containsEps){
		unordered_set<Terminal *> first = this->firsts[e];
		unordered_set<Terminal *> toRet;

		for (Terminal* t : first){
			if (isEpsilon(t))
				containsEps = true;
			else
				toRet.insert(t);
		}

		return toRet;
	}

	unordered_set<Terminal *> getFirstOfOneVector(vector<Element*>& str){ // for one string of rhs
		// for each string get first

		unordered_set<Terminal *> result;
		for (Element* e : str){
			// case of one element only >> terminal >> Eps/term
			if (isEpsilon(e)){
				result.insert(EPSILON);
				break;
			}

			if (isTerminal(e)){ // first occarenece of terminals
				result.insert((Terminal *)e);
				break;
			}

			// non terminal
			//case str more than one element
			if (!firstDone[(NonTerminal *) e])
				addFirstOfOneProduction(this->mapper[(NonTerminal *) e]);

			// ready
			bool containsEps = false;

			unordered_set<Terminal *> ans = getFirstAndCheckEps((NonTerminal *)e, containsEps);

			// add the returned recursively
			for (Terminal * t : ans)
				result.insert(t);

			// enough till this
			if (!containsEps)
				break;

			if (e == str.back()) // if all contains eps so you should contain eps also
				result.insert(EPSILON);

		}

		return result;

	}

	void addFirstOfOneProduction(Production * p) { //for all rhs
		vector<vector<Element *>> rhs = p->rhs;
		// get rhs >> vector(terminals)
		// get their first of each string(vector) of the rhs
		for (vector<Element *>& elem : rhs){
			// get first for each string
			unordered_set<Terminal *> f = getFirstOfOneVector(elem);

			// add them one by one
			for (Terminal * t : f)
				this->firsts[p->lhs].insert(t);
		}
		// visited(done)
		this->firstDone[p->lhs] = true;
	}

	void buildFirst() {
		//loop on each production and call add()
		for (Production * p : productions)
			if (!firstDone[p->lhs])
				addFirstOfOneProduction(p);

	}








	void lastNonTerminal(NonTerminal* lhsOftheProd, NonTerminal* mine){
		if (lhsOftheProd == mine) // already calculated
			break;
		// A >> BCD >> ALL HAVE EPS
		// then follow(A) should be added to Follow(B)
		if (!this->followDone[lhsOftheProd])
			addFollowOfOneProduction(mapper[lhsOftheProd]);

		for (Terminal* t : this->follows[lhsOftheProd])
			this->follows[mine].insert(t);
	}

	void beforeLastTerminal(NonTerminal* nt, Terminal* t){
		this->follows[nt].insert(t);
	}

	void beforeLastNonTerminal(NonTerminal* lhsOftheProd, NonTerminal* mine, vector<Element *>&v, int index){

		for (; index < v.size(); index++){
			// first at non terminal so condition is not valid
			Element* e = v[index];
			if (isTerminal(e)){
				this->follows[mine].insert((Terminal *)e);
				break;
			}
			// so we are now on a non terminal
			// add the firsts of this non ter to the follow of mine
			bool hasEps = false;
			for (Terminal* t : this->firsts[(NonTerminal *)e]){
				if (isEpsilon(t))
					hasEps = true;
				else
					this->follows[mine].insert(t);
			}

			// the condition to continue is hasing an eps
			if (!hasEps)
				break;

			if (index == v.size() - 1){
				/*
				 A >> ADC
				 then The follow of A calculated from D and C
				 so adding follow of A(lhs) to follow of A(rhs) will make dublication
				 */
				if (lhsOftheProd == mine) // already calculated
					break;
				// A >> BCD >> ALL HAVE EPS
				// then follow(A) should be added to Follow(B)
				if (!this->followDone[lhsOftheProd])
					addFollowOfOneProduction(mapper[lhsOftheProd]);

				for (Terminal* t : this->follows[lhsOftheProd])
					this->follows[mine].insert(t);
			}
		}

	}

	void addFollowOfOneProduction(Production * p) {
		NonTerminal* lhs = p->lhs;

		for (Production * prod : this->productions){
			for (vector<Element *>&v : prod->rhs){
				for (int i = 0; i < v.size(); i++){
					if (v[i] == lhs){
						// case of last nonterminal
						if (i == v.size() - 1)
							lastNonTerminal(prod->lhs, lhs);
						// case of before a terminal
						else if (isTerminal(v[i + 1]))
							beforeLastTerminal(lhs, (Terminal *)v[i + 1]);
						// before a nonterminal
						else
							beforeLastNonTerminal(prod->lhs, lhs, v, i + 1);
					}
				}
			}
		}


		this->followDone[p->lhs] = true;
	}

	void buildFollow() {
		this->follows[startSymb].insert(DOLLER_SIGN);
		for (Production * p : productions)
				if (!followDone[p->lhs])
					addFollowOfOneProduction(p);
	}

};

class CFGBuilder { // builds cfg from input
	ContextFreeGrammar* cfg;
};







int main() {

}

