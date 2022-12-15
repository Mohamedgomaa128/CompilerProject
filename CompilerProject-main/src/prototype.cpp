#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

typedef vector<int> vi;
const string EPSILON = "#";

class Node
{
    // private :
    // constructor to give id incrementally

public:
    int id;
    bool acceptingNode=false;
    string pattern;
    // set<Transation> children;
    Node *clone()
    {
        // temp function;
        Node *n = new Node();
        n->id = this->id;
        n->acceptingNode = this->acceptingNode;
        n->pattern = this->pattern;
        return n;
    }
};

class Transition
{

public:
    Node *from, *to;
    string transVal;
    Transition()
    {
        // empty constructor
    }
    Transition(Node *f, Node *t, string val)
    {
        Transition();
        this->from = f;
        this->to = t;
        this->transVal = val;
    }
};

class NFA
{
public:
    /*
      NFA >> concat , union, kleane closure, positive closure
     */

    Node *start, *end;
    set<Node *> nodes;
    map<Node *, set<Transition *>> transOfNodes;

    NFA()
    {
        // nulls
        // this->start = NULL;
    }
    NFA(string val)
    {
        addNode(this->start = new Node());
        addNode(this->end = new Node());
        end->pattern=val;
        end->acceptingNode=true;
        addTrans(start, new Transition(start, end, val));

    }

    void addNode(Node *node)
    {
        this->nodes.insert(node);
    }

    void addTrans(Node *node, Transition *t)
    {
        this->transOfNodes[node].insert(t);
    }

    NFA *clone()
    {
        // create a map to avoid repeating cloning and avoid pointers problems
        unordered_map<Node *, Node *> umap;
        for (Node *n : this->nodes)
            umap[n] = n->clone();

        NFA *nfa = new NFA();
        for (Node *n : this->nodes)
        {
            nfa->addNode(umap[n]);
            for (Transition *t : this->transOfNodes[n])
                nfa->addTrans(umap[t->from], new Transition(umap[t->from], umap[t->to], t->transVal));
        }
        nfa->start = umap[this->start];
        nfa->end = umap[this->end];

        return nfa;
    }

    NFA *concat(NFA *first, NFA *second)
    {
        NFA *fcopy = first->clone();
        NFA *scopy = second->clone();
        string p1,p2;
        p1=fcopy->end->pattern;
        p2=scopy->end->pattern;

        // delete last node of first nfa
        fcopy->nodes.erase(fcopy->end);
        // move edges to the first node of second nfa
        for (Node *n : fcopy->nodes)
            for (Transition *t : fcopy->transOfNodes[n])
                if (t->to == fcopy->end)
                    t->to = scopy->start;

        for (Node *n : scopy->nodes)
        {
            fcopy->addNode(n); // add the nodes to the first

            // add the transations to the first
            for (Transition *t : scopy->transOfNodes[n])
                fcopy->addTrans(n, t);
        }

        scopy->end->acceptingNode=true;
        scopy->end->pattern=p1+" "+p2;
        fcopy->end = scopy->end;
        // return the concatinate
        return fcopy;
    }

    NFA *kleeneClosure(NFA *automata)
    {
        NFA *nfa = positiveClosure(automata);
        nfa->end->pattern=nfa->end->pattern.substr(0,nfa->end->pattern.size()-1)+"*";
        nfa->addTrans(nfa->start, new Transition(nfa->start, nfa->end, EPSILON));
        return nfa;
    }

    NFA *positiveClosure(NFA *automata)
    {
        NFA *nfa = automata->clone();

        // new nodes added
        Node *newStart = new Node();
        Node *newEnd = new Node();
        newEnd->acceptingNode=true;
        newEnd->pattern=automata->end->pattern+"+";
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

    NFA *unionAutomatas(NFA *first, NFA *second)
    {
        NFA *f = first->clone();
        NFA *s = second->clone();

        string p1,p2;
        p1=f->end->pattern;
        p2=s->end->pattern;

        // create new nodes
        Node *newStart = new Node();
        Node *newEnd = new Node();
        f->addNode(newStart);
        f->addNode(newEnd);

        // move second one to first one
        for (Node *n : s->nodes)
        {
            f->addNode(n);
            for (Transition *t : s->transOfNodes[n])
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
        f->end->acceptingNode=true;
        f->end->pattern=p1+"|"+p2;

        return f;
    }

    NFA *combineAutomatas(set<NFA* > automatas)
    {

        NFA *cnfa = new NFA();
        Node *newStart = new Node();
        Node *newEnd = new Node();
        cnfa->addNode(newStart);
        cnfa->addNode(newEnd);

        for (NFA* automata : automatas)
        {
            cnfa->addTrans(newStart, new Transition(newStart, automata->start, EPSILON));
            cnfa->addTrans(automata->end, new Transition(automata->end, newEnd, EPSILON));
            for (Node *n : automata->nodes)
            {
                cnfa->addNode(n);
                for (Transition *t : automata->transOfNodes[n])
                    cnfa->addTrans(n, t);
            }
        }

        cnfa->start = newStart;
        cnfa->end = newEnd;

        return cnfa;
    }

    set<string> getInputSymbols(){
		set<string> inputSymbols;
		for (Node * n : nodes){
			for (Transition* t : this->transOfNodes[n])
				if (t->transVal != EPSILON)
					inputSymbols.insert(t->transVal);
		}

		return inputSymbols;
	}
};

class DFA {
    public:
	Node* stNode;
	map<Node*, set<Transition *>> nTrans;
	map<Node*, set<Transition *>> dTrans;
	map<set<Node *>, Node*> groupedStates;
	set<Node*> dStates;
	set<string > inputSymbols;
	bool buildOrMin; // build >> nTrans,

	DFA(NFA* nfa){
        this->stNode= new Node ();
		buildOrMin = true;
		this->build(nfa);
		buildOrMin = false;
		this->minimize();
	}


	void build(NFA* nfa){

		this->nTrans = nfa->transOfNodes;
		this->inputSymbols = nfa->getInputSymbols();

		set<Node*> n = epsClosure(nfa->start);
		groupedStates[n] = nfa->start;
		this->stNode = groupedStates[n];

		set<set<Node*>> statesBeforeMapping;
		statesBeforeMapping.insert(n);

		map<set<Node*>, bool> visited;
		visited[n] = false;

		// loop on each unvisited states and get epsClosure and possible moves getting all levels and add them to the dfa
		while (!n.empty()){
			//mark as visited
			visited[n] = true;
			modifyAcceptingState(groupedStates[n],n);


			// next level of nodes by moving and getting epsClosure
			for (string pattern : this->inputSymbols){
				set<Node*> newNode = epsClosure(move(n, pattern));
				if (!newNode.empty()){ // added to the next level of n
					//if not found >> add i
					if (statesBeforeMapping.find(newNode) == statesBeforeMapping.end()){
						groupedStates[newNode] = new Node();
						statesBeforeMapping.insert(newNode);
						visited[newNode] = false;
						modifyAcceptingState(groupedStates[newNode], newNode);
					}
					// add the transition
					dTrans[groupedStates[n]].insert(new Transition(groupedStates[n], groupedStates[newNode],pattern ));
				}
			}
			n = getUnvisited(visited);
		}
	}


	set<Node*> getUnvisited(map<set<Node*>, bool> visited){
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

	set<Node*> move(Node* n, string pattern){
		// move a node by a char

		set<Node *> nextNodes;
		// for build or minimization
		set<Transition*> transi = (buildOrMin ? this->nTrans[n] : this->dTrans[n]);
		for (Transition* t : transi)
			if (t->transVal == pattern)
				nextNodes.insert(t->to);

		return nextNodes;
	}

	set<Node*> move(set<Node*> n, string pattern){
		// move a set of nodes by char
		set<Node *> nextNodes;
		for (Node* node : n){
			set<Node *> ret = move(node, pattern);
			if (!ret.empty())
				nextNodes.insert(ret.begin(), ret.end());
		}

		return nextNodes;
	}

	void modifyAcceptingState(Node* newNode, set<Node*> old){
	    set<Node*> clone = old;
		// if one of old node is accepting convert current to accepting
		for (Node * node : clone)
			if (node==NULL||node->acceptingNode){
				newNode->acceptingNode = true;
                newNode->pattern = node->pattern;
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
			for (string pattern : inputSymbols)
				newGroups = partition(newGroups, pattern);
			if (newGroups == groups)
				notMinimized = false;

			groups = newGroups;
		}
        mapGroupsToStates(groups);
	}

	set<set<Node*>> partition(set<set<Node*>> groups, string pattern){

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

					if (goToSameGroup(groups, node1, node2, pattern)){
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

	bool goToSameGroup(set<set<Node*>> groups, Node* n1, Node* n2, string pattern){
		// to same group ?
		set<Node*> next1 = move(n1, pattern);
		set<Node*> next2 = move(n2, pattern);

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
		for (auto entry : repStates)
			dStates.insert((Node *) entry.second);

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


class Language
{
public:
    Language()
    {
    }
    string name;
    NFA *automata;
};

class Expression
{
public:
    Expression()
    {
    }
    string name;
    NFA *automata;
};
vector<string> outputs;
vector<string> inputs;
vector<Language> languages;
vector<Expression> expressions;
vector<string> keywords;
vector<char> puncs;
vector<char> operators{'|', '*', '+', ' '};

NFA* automata;
DFA* finalAutomata;
int opPriority(char op)
{
    switch (op)
    {
    case '|':
        return 1;
    case ' ':
        return 2;
    case '*':
        return 3;
    case '+':
        return 3;
    default:
        return 0;
    }
}

NFA *getLanguage(string pattern)
{
    for (int i = 0; i < languages.size(); i++)
    {
        if (languages[i].name == pattern)
            return languages[i].automata->clone();
    }
    for (int i = 0; i < expressions.size(); i++)
    {
        if (expressions[i].name == pattern)
            return expressions[i].automata->clone();
    }
    return NULL;
}

NFA *applyOperation(NFA *automata1, NFA *automata2, char op)
{
    NFA *result = new NFA();
    switch (op)
    {
    case '|':
        return result->unionAutomatas(automata1, automata2);
    case ' ':
        return result->concat(automata1, automata2);
    case '*':
        return result->kleeneClosure(automata2);
    case '+':
        return result->positiveClosure(automata2);
    default:
        return NULL;
    }
}

bool canConcat(char c)
{
    if (isalpha(c) || c == '.'|| c== '*' || c=='+')
        return true;
    return false;
}

string handleSpace(string s)
{
    string str = s;
    int j=0;
    for (int i = 0; i < s.size(); i++)
    {
        str = str.substr(0, j);
        if (s[i] == ' ')
        {
            if(i != 0 && i != s.size() - 1 && ((canConcat(s[i - 1]) && canConcat(s[i + 1]))||s[i - 1] == ')' || s[i + 1] == '('  ))
            {
                str += " ";
                j++;
            }
        }
        else
        {
            str+=s[i];
            j++;
        }
    }
    return str;
}

NFA* regToNFA(string s,string pattern)
{
    int l = s.size();
    stack<char> ops;
    stack<NFA *> automatas;

    for (int i = 0; i < l; i++)
    {
        if (s[i] == '(')
        {
            ops.push(s[i]);
        }

        else if (s[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                char op = ops.top();
                ops.pop();
                NFA *automata2 = automatas.top();
                NFA *automata1;
                automatas.pop();
                if (op == ' ' || op == '|')
                {
                    automata1 = automatas.top();
                    automatas.pop();
                }
                NFA *result = applyOperation(automata1, automata2, op);
                automatas.push(result);
            }
            if (!ops.empty())
                ops.pop();
        }
        else if (!(std::find(operators.begin(), operators.end(), s[i]) != operators.end()) || (std::find(operators.begin(), operators.end(), s[i]) != operators.end()) && (s[i - 1] == '\\'))
        {
            string tokenName = "";
            int slashIndex = -1;
            while (i < s.size() && (s[i]!='('&&s[i]!=')')&&(!(std::find(operators.begin(), operators.end(), s[i]) != operators.end()) || (std::find(operators.begin(), operators.end(), s[i]) != operators.end()) && (s[i - 1] == '\\')))
            {
                if (s[i] == '\\')
                {
                    if (s[i + 1] == 'L'){
                        tokenName += "#";
                        i++;
                    }

                    else
                    {
                        slashIndex = tokenName.size();
                    }
                }
                else
                {
                    tokenName += s[i];
                }
                i++;
            }

            if(tokenName[0]==' ')
                tokenName=tokenName.substr(1);
            if(tokenName[tokenName.size()-1]==' ')
                tokenName=tokenName.substr(0,tokenName.size()-1);

            NFA *nfa = new NFA(tokenName);
            if(tokenName==EPSILON)
                tokenName="\\L";
            NFA *result = getLanguage(tokenName);
            if (result == NULL)
            {

                if (slashIndex > 0)
                {
                    NFA *nfa1, *nfa2;
                    nfa1 = new NFA(tokenName.substr(0, slashIndex));
                    nfa2 = new NFA(tokenName.substr(slashIndex));
                    nfa = nfa->concat(nfa1, nfa2);
                }
            }
            else
            {
                nfa = result->clone();
            }
            automatas.push(nfa);
            i--;
        }

        else
        {
            while (!ops.empty() && opPriority(s[i]) < opPriority(ops.top()))
            {
                char op = ops.top();
                ops.pop();
                NFA *automata2 = automatas.top();
                NFA *automata1;
                automatas.pop();
                if (op == ' ' || op == '|')
                {
                    automata1 = automatas.top();
                    automatas.pop();
                }
                NFA *result = applyOperation(automata1, automata2, op);
                automatas.push(result);
            }
            ops.push(s[i]);
        }
    }

    while (!ops.empty())
    {
        char op = ops.top();
        ops.pop();
        NFA *automata2 = automatas.top();
        NFA *automata1;
        automatas.pop();
        if (op == ' ' || op == '|')
        {
            automata1 = automatas.top();
            automatas.pop();
        }
        NFA *result = applyOperation(automata1, automata2, op);
        automatas.push(result);
    }
    NFA* automata = automatas.top();
    for(Node* node : automata->nodes){
        if(node->acceptingNode)
            node->pattern=pattern;
    }
    return automata;
}

void ruleParser(string s)
{
    if (s.find('=') != string::npos && s.find(':') == string::npos)
    {
        Language *language = new Language();

        char *token = strtok(&s[0], "=");
        string name = token;
        name=name.substr(0,name.size()-1);
        language->name = name;
        token = strtok(NULL, "\n");
        string reg = token;
        reg = handleSpace(reg);
        language->automata = regToNFA(reg,name);
        languages.push_back(*language);
    }
    else if (s.find(':') != string::npos)
    {
        Expression *expression = new Expression();
        char *token = strtok(&s[0], ":");
        string name = token;
        if(name[name.size()-1]==' ')
            name=name.substr(0,name.size()-1);
        expression->name = name;
        token = strtok(NULL, "\n");
        string reg = token;
        reg = handleSpace(reg);
        expression->automata = regToNFA(reg,name);
        expressions.push_back(*expression);
    }
    else if (s[0] == '{')
    {
        s = s.substr(1, s.size() - 2);
        char *token = strtok(&s[0], " ");
        while (token != NULL)
        {
            keywords.push_back(token);
            token=strtok(NULL," ");
        }
    }
    else if (s[0] == '[')
    {
        s = s.substr(1, s.size() - 2);
        char *token = strtok(&s[0], " ");
        while (token != NULL)
        {
            if(token[0]=='\\'){
                puncs.push_back(token[1]);
            }
            else {
                puncs.push_back(token[0]);
            }
            token=strtok(NULL," ");
        }
    }
}
bool checkRegex(char c, string transVal)
{
    string regStr = "[" + transVal + "]";
    regex reg(regStr);
    string cStr = "";
    cStr+=c;
    if (regex_match(cStr, reg))
    {
        return true;
    }
    return false;
}

void checkWord(string word)
{
    if(word==""){
        return;
    }
    Node *currentNode = finalAutomata->stNode;
    set<Transition *> nextTransitions = finalAutomata->dTrans.find(currentNode)->second;
    int p = 0;
    Node *lastAccepting;
    int lastAcceptingIndex = -1;
    Transition* trans ;
    bool flag = false;
    for (int i = 0; i < word.size(); i++)
    {
        if(finalAutomata->dTrans.find(currentNode) != (finalAutomata->dTrans.end())){
            for (Transition *transition : nextTransitions)
            {
                string pattern = transition->transVal;
                if (checkRegex(word[i], pattern))
                {
                    currentNode = trans->to;
                    nextTransitions = finalAutomata->dTrans.find(currentNode)->second;
                    lastAccepting = currentNode;
                    lastAcceptingIndex = i;
                    break;
                }
            }
        }
    }

    if (lastAcceptingIndex != -1)
    {
        outputs.push_back(lastAccepting->pattern);
        if (lastAcceptingIndex < word.size() - 1)
            checkWord(word.substr(lastAcceptingIndex + 1));
        return;
    }
    else
    {
        outputs.push_back("ERROR");
    }
}



void loadFile()
{
    fstream file;
    file.open("rules.txt", ios::in);
    string line;
    while (getline(file, line))
    {
        ruleParser(line);
    }
    set<NFA*> automatasSet;
    for(Expression exp : expressions)
    {
        automatasSet.insert(exp.automata->clone());
    }
    automata=automata->combineAutomatas(automatasSet);
    finalAutomata= new DFA(automata);
    DFA* dfa = finalAutomata;
    int id = 1;
    for(Node* node : finalAutomata->dStates){
        node->id=id++;
    }
    for(Node* node : finalAutomata->dStates){

        cout<<"State ID:"+ to_string(node->id) + "\tAccepting?\t" +node->pattern<<endl;
        cout<<"-------------------------------------------"<<endl;
        if(finalAutomata->dTrans.find(node) == (finalAutomata->dTrans.end()))
            continue;
        for(Transition* t : finalAutomata->dTrans.find(node)->second){
            if(t!=NULL)
                cout<< "On input \""+(t->transVal)+"\" it goes to state " + to_string((t->to->id))+"."<< endl;
        }
        cout<<"-------------------------------------------"<<endl;
        cout<<"-------------------------------------------"<<endl;
    }
    file.close();
    file.open("input.txt", ios::in);
    while (getline(file, line))
    {
        char *token = strtok(&line[0], " ");
        while (token != NULL)
        {
            string word = token;
            for (int i = 0; i < word.size(); i++)
            {
                if (std::find(puncs.begin(), puncs.end(), word[i]) != puncs.end())
                {
                    if (i > 0)
                    {
                        inputs.push_back(word.substr(0, i));
                        word = word.substr(i, word.size());
                        i=0;
                    }
                    inputs.push_back(word.substr(0, 1));
                    word = word.substr(1, word.size());
                    i--;
                }
            }
            inputs.push_back(word);
            token = strtok(NULL, " ");
        }
    }

    for (string word : inputs)
    {
        if (std::find(keywords.begin(), keywords.end(), word) != keywords.end())
        {
            outputs.push_back(word);
        }
        else if (std::find(puncs.begin(), puncs.end(), word[0]) != puncs.end())
        {
            outputs.push_back(word);
        }
        else
        {
            checkWord(word);
        }
    }

    fstream outputFile;
    outputFile.open("output.txt", ios::out);

    for (string word : outputs)
    {
        outputFile << word << endl;
    }

}

int main()
{
    loadFile();

    return 0;
}
