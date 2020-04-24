#ifndef RETONFA_H
#define RETONFA_H

#include<bits/stdc++.h>


class State
{
private:
	std::map<char,std::vector<State*> > transitions;

	bool accepted;
	std::string token_class;
    int partition;
    std::vector<State> epison_closure ;
public:
    State():accepted(false){}
    State(int state_id , bool accepted);

	int state_id;
	std::map<char , State*> next_DFA_states;
	std::unordered_map<int,bool> NFA_states_id ;

	bool traversed = false;
	std::map<char,std::vector<State*> > getTransitions();


    std::vector<State> get_epison_closure();
    void set_epison_closure(std::vector<State> &epison_closure);

	bool isAccepted();
	std::string get_token_class();
	std::vector<State *> get_next_NFA_states(char symbol);
    State* get_next_DFA_state(char symbol);

	void setAccepted(bool accept);
	void set_token_class(std::string token);
	void addTransition(char symbol, std::vector<State *> &nextStates);

        void set_partition(int index)
        {
            this -> partition = index;
        }
        int get_partition()
        {
            return this -> partition;
        }
	// void removeTransition(char symbol);
	// void setNextStates(char symbol);
};

class NFA
{
private:
	State *startState;
	State* acceptState;
	std::unordered_set  <char> alphabet;
	std::map<std::string, int> tokensPriority;
	// char[] alphabet;
	// void setAsciiAlphabet();
public:
	NFA(){};
	NFA(State *start){startState=start;}

	void setStartState(State *start);
	void setAcceptState(State *state);

	void addCharToAlphabet(char alph);
	void setAlphabet(std::unordered_set<char> alph);
    void setTokensPriority(std::map<std::string, int> prioritys){this->tokensPriority = prioritys;}

	State *getStartState() { return startState; }
	State* getAcceptState() { return acceptState; }
	std::unordered_set<char> getAlphabet(){return alphabet;}
	std::map<std::string, int> getTokensPriority(){return this->tokensPriority;}

};

/*
    input:

    output:

    remarks:
*/
void positiveClosure(NFA *nfa);
void kleeneClosure(NFA *nfa);
void concat(NFA* nfa1,NFA* nfa2);
NFA *unionNFAS(NFA* nfa1,NFA* nfa2);
NFA *charNFA(char symbol);
NFA *wordNFA(std::string word);
NFA *charRangeNFA(char c1, char c2);
std::string convertINtoPost(std::string expr);
std::vector<std::string> findKeywords(std::string line);
std::vector<char> findPunctuation(std::string line);
std::pair<std::string, std::string> findDefinitions(std::string line, bool *token);
void parseLine(std::string line);
NFA *generateNFA(std::string filePath);
NFA *concatAllTokens();
NFA *constructNFAFromREPostfix(std::string postfixRE);
NFA* searchForToken(std::string tokenName);

#endif

