#include "REToNFA.h"
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_set>
using namespace std;
//---------------------------------------------------------
// State member functions
//--------------------------------------------------------
std::map<char, std::vector<State *>> State::getTransitions()
{
	return this->transitions;
}

bool State::isAccepted()
{
	return this->accepted;
}

string State::get_token_class()
{
	return this->token_class;
}

std::vector<State *> State::get_next_NFA_states(char symbol)
{
	std::map<char, std::vector<State *>> transitions = getTransitions();
	std::map<char, std::vector<State *>>::iterator it = transitions.find(symbol);
	if (it != transitions.end())
	{
		return transitions.find(symbol)->second;
	}
	else
	{
		return std::vector<State *>{};
	}
}

void State::setAccepted(bool accept)
{
	this->accepted = accept;
}
void State::set_token_class(string token)
{

	this->token_class = token;
}

void State::addTransition(char symbol, std::vector<State *> &nextStates)
{
	this->transitions[symbol] = nextStates;
}

//------------------------------------------------------------
// NFA member functions
//------------------------------------------------------------
void NFA::setStartState(State *start)
{
	this->startState = start;
}

// void NFA::setAcceptStates(std::vector<State *> states)
// {
// 	this->acceptStates = states;
// }
void NFA::setAcceptState(State *state)
{
	this->acceptState = state;
	// (*(this->acceptState)).setAccepted(true);
}

void NFA::addCharToAlphabet(char alph)
{
	this->alphabet.insert(alph);
}

void NFA::setAlphabet(std::unordered_set<char> alph)
{
	this->alphabet = alph;
}
//--------------------------------------------------------
// Parsing input file including lexical rules
//--------------------------------------------------------

std::map<string, NFA *> tokensNFA;
std::map<string, int> tokensPriority;
int lastTokenPriority = 2;
// std::map<string, NFA *> identifiers;

// LHS=RHS and LHS:RHS
std::map<string, string> REDefinitions;

string convertINtoPost(string expr)
{
	stack<char> operators;
	operators.push('#');
	bool concatPos = false;
	string post = "";

	for (int i = 0; i < expr.size(); i++)
	{
		if (expr[i] == '-')
		{
			while (expr[++i] == ' ')
				;
			post += expr[i];
			post += '-';
			// post += ' ';
		}
		else if (expr[i] == '|')
		{
			while (operators.top() == '$')
			{
				post += '$';
				operators.pop();
			}
			operators.push('|');
			post += ' ';
			concatPos = false;
		}
		else if (expr[i] == '(')
		{
			if (i != 0 && expr[i - 1] != ' ' && concatPos && expr[i - 1] != '|' && expr[i - 1] != '+' && expr[i - 1] != '*')
			{
				operators.push('$');
				post += ' ';
			}
			concatPos = false;
			operators.push('(');
		}
		else if (expr[i] == ')')
		{
			while (operators.top() != '(')
			{
				post += operators.top();
				operators.pop();
			}
			operators.pop();
			while (expr[++i] == ' ' && i < expr.size())
				;
			if (i < expr.size() && expr[i] != ' ' && expr[i] != '|' && expr[i] != '-' && expr[i] != '+' && expr[i] != '*')
			{
				operators.push('$');
				post += ' ';
				concatPos = true;
			}
			else
			{
				concatPos = false;
			}
			i--;
		}
		else if (expr[i] == '+')
		{
			post += "+";
			while (expr[++i] == ' ' && i < expr.size())
				;
			if (i < expr.size() && expr[i] != ' ' && expr[i] != '|' && expr[i] != '-' && expr[i] != '+' && expr[i] != '*')
			{
				operators.push('$');
				post += ' ';
				concatPos = true;
			}
			else
			{
				concatPos = false;
			}
			i--;
		}
		else if (expr[i] == '*')
		{
			post += '*';
			while (expr[++i] == ' ' && i < expr.size())
				;
			if (i < expr.size() && expr[i] != ' ' && expr[i] != '|' && expr[i] != '-' && expr[i] != '+' && expr[i] != '*')
			{
				operators.push('$');
				post += ' ';
				concatPos = true;
			}
			else
			{
				concatPos = false;
			}
			i--;
		}
		else if (expr[i] == ' ')
		{

			while (expr[++i] == ' ' && i < expr.size())
				;
			if (i < expr.size() && expr[i] != ' ' && concatPos && expr[i] != '|' && expr[i] != '-' && expr[i] != '+' && expr[i] != '*')
			{
				operators.push('$');
				post += ' ';
			}
			else /*if(expr[i] != '+' && expr[i] != '*')*/
			{
				concatPos = false;
			}
			i--;
		}
		else if (expr[i] == '\\')
		{
			post += '\\';
			post += expr[++i];
			concatPos = true;
		}
		else
		{
			post += expr[i];
			concatPos = true;
		}
	}
	while (operators.top() != '#')
	{
		post += operators.top();
		operators.pop();
	}
	return post;
}

string parsing_RE(string expr)
{
	auto add_space = [](char last) {
		if (last != '`' && last != '*' && last != '+' && last != ' ' && last != '-')
			return true;
		return false;
	};
	std::stack<char> operators;
	operators.push('#');
	string parsed_expr = "";
	char last_parsed_char = '`';
	bool concatPos = false;
	for (int i = expr.find_first_not_of(' '); i < expr.find_last_not_of(' ') + 1; i++)
	{
		if (expr[i] == '|')
		{
			while (operators.top() == '$')
			{
				parsed_expr += '$';
				operators.pop();
			}
			operators.push('|');
			concatPos = false;
			// if(add_space(last_parsed_char)){
			// 	parsed_expr += ' ';
			last_parsed_char = ' ';
			// }
		}
		else if (expr[i] == '*' || expr[i] == '+')
		{
			parsed_expr += expr[i];
			last_parsed_char = expr[i];
		}
		else if (expr[i] == '-')
		{
			while (expr[++i] == ' ')
				;
			parsed_expr += expr[i];
			parsed_expr += '-';
			last_parsed_char = '-';
		}
		else if (expr[i] == '(')
		{

			if (last_parsed_char == ' ')
				operators.push('$');
			else if (add_space(last_parsed_char))
			{
				last_parsed_char = ' ';
				operators.push('$');
			}
			else if (last_parsed_char == '*' || last_parsed_char == '+' || last_parsed_char == '-')
			{
				operators.push('$');
			}
			operators.push('(');
			concatPos = false;
		}
		else if (expr[i] == ')')
		{
			while (operators.top() != '(')
			{
				parsed_expr += operators.top();
				last_parsed_char += operators.top();
				operators.pop();
			}
			operators.pop();
			if (add_space(last_parsed_char))
				last_parsed_char = ' ';
			concatPos = true;
			last_parsed_char = '`';
		}
		else if (expr[i] == ' ')
		{
			last_parsed_char = ' ';
		}
		else
		{
			if (last_parsed_char == ' ')
			{
				parsed_expr += ' ';
			}
			if (concatPos && (last_parsed_char == ' ' || last_parsed_char == '+' || last_parsed_char == '*'))
				operators.push('$');
			parsed_expr += expr[i];
			last_parsed_char = expr[i];
			concatPos = true;
			if (expr[i] == '\\')
			{
				parsed_expr += expr[++i];
			}
		}
	}
	while (operators.top() != '#')
	{
		parsed_expr += operators.top();
		operators.pop();
	}
	return parsed_expr;
}
std::vector<string> findKeywords(string line)
{
	std::vector<string> keywords;
	string prefix = "";
	for (int i = line.find_first_of('{') + 1; i < line.find_first_of('}'); i++)
	{
		if (line[i] == ' ' && prefix.size() > 0)
		{
			keywords.push_back(prefix);
			prefix = "";
		}
		else if (line[i] != ' ')
		{
			prefix += line[i];
		}
	}
	if (prefix.size() > 0)
		keywords.push_back(prefix);
	// cout << "Keywords" << endl;
	// for (int i = 0; i < keywords.size(); i++)
	// {
	// 	cout << keywords[i] << ", ";
	// }
	// cout << endl;
	return keywords;
}

std::vector<char> findPunctuation(string line)
{
	vector<char> punctuation;
	//cout << line << endl;
	for (int i = line.find_first_of('[') + 1; i < line.find_last_of(']'); i++)
	{
		// if(line[i]==';')cout << "that is"<<endl;
		if (line[i] == '\\')
		{
			punctuation.push_back(line[++i]);
			// cout << line[i]<<endl;
		}
		else if (line[i] != ' ')
		{
			punctuation.push_back(line[i]);
			// cout << line[i]<<endl;
		}
	}
	// cout << "punctuation" << endl;
	// for (int i = 0; i < punctuation.size(); i++)
	// {
	// 	cout << punctuation[i] << "  ";
	// }
	// cout << endl;
	return punctuation;
}

std::pair<string, string> findDefinitions(string line, bool *token)
{
	pair<string, string> id;
	string identifier = "";
	string re = "";
	bool idcomplete = false;
	*token = true;

	for (int i = line.find_first_not_of(' '); i <= line.find_last_not_of(' '); i++)
	{
		if (!idcomplete && (line[i] == '=' || line[i] == ':'))
		{
			if (line[i] == '=')
				*token = false;
			id.first = identifier;
			while (line[++i] == ' ')
				;
			i--;
			idcomplete = true;
		}
		else if (idcomplete)
		{
			re += line[i];
		}
		else if (line[i] != ' ')
		{
			identifier += line[i];
		}
	}
	id.second = re;
	// cout << id.first << "----->" << id.second << endl;
	return id;
}

void parseLine(string line)
{
	string id = "";
	int first = line.find_first_not_of(' ');
	std::vector<string> keywords;
	std::vector<char> punctuations;
	if (line[first] == '{')
	{
		keywords = findKeywords(line);
		for (int i = 0; i < keywords.size(); i++)
		{
			tokensNFA[keywords[i]] = wordNFA(keywords[i]);
			tokensNFA[keywords[i]]->getAcceptState()->set_token_class("keyword");
			tokensPriority["keyword"] = 1;
		}
	}
	else if (line[first] == '[')
	{
		punctuations = findPunctuation(line);
		for (int i = 0; i < punctuations.size(); i++)
		{
			string s = "" + punctuations[i];
			tokensNFA[s] = charNFA(punctuations[i]);
			tokensNFA[s]->getAcceptState()->set_token_class("punctuation");
			tokensPriority["punctuation"] = 0;
		}
	}
	else
	{
		bool token = false;
		pair<string, string> id = findDefinitions(line, &token);
		string postRE = parsing_RE(id.second);
		// cout << "post RE -->" << postRE << endl;
		if (token)
		{
			REDefinitions[id.first] = id.second;
			tokensNFA[id.first] = constructNFAFromREPostfix(postRE);
			tokensPriority[id.first] = lastTokenPriority++;
		}
		else
		{
			REDefinitions[id.first] = id.second;
		}
	}
}

/*
    input: file path containing lexical rules of a language
    output:  nfa object representing lexical rules
    remarks: main function for convertion from RE to NFA
*/
NFA *generateNFA(string filePath)
{
	fstream newfile;
	newfile.open(filePath, ios::in);
	if (newfile.is_open())
	{
		string line;
		while (getline(newfile, line))
		{
			// cout << line << endl;
			parseLine(line);
		}
		newfile.close();
	}
	return concatAllTokens();
}

NFA *concatAllTokens()
{
	State *start = new State();
	State *accept = new State();
	accept->setAccepted(true);
	std::vector<State *> startNextStates;
	std::vector<State *> acceptNextState{accept};
	std::unordered_set<char> alphabet;
	std::map<string, NFA *>::iterator it = tokensNFA.begin();
	while (it != tokensNFA.end())
	{
		if (it->second->getAcceptState()->get_token_class() != "keyword" &&
			it->second->getAcceptState()->get_token_class() != "punctuation")
		{
			it->second->getAcceptState()->set_token_class(it->first);
		}
		// assignTokensName(it->second,it->first);
		it->second->getAcceptState()->setAccepted(true);
		// cout << "traverse : NFA for " << it->first << endl;
		// traverseNFA(it->second);

		it->second->getAcceptState()->addTransition(' ', acceptNextState);
		startNextStates.push_back(it->second->getStartState());
		for (auto elem : it->second->getAlphabet())
		{
			alphabet.insert(elem);
		}
		it++;
	}
	start->addTransition(' ', startNextStates);
	NFA *newNFA = new NFA(start);
	newNFA->setAcceptState(accept);
	newNFA->setAlphabet(alphabet);
	newNFA->setTokensPriority(tokensPriority);
	return newNFA;
}
//------------------------------------------------------
// functions to perform convertion from RE to NFAs
//-----------------------------------------------------
NFA *constructNFAFromREPostfix(string postfixRE)
{
	std::vector<NFA *> nfas;
	string prefix = "";
	for (int i = 0; i < postfixRE.size(); i++)
	{
		if (postfixRE[i] == '|')
		{
			if (prefix.size() > 0)
				nfas.push_back(searchForToken(prefix));
			if (nfas.size() > 1)
			{
				nfas[nfas.size() - 2] = unionNFAS(nfas[nfas.size() - 2],
												  nfas[nfas.size() - 1]);
				nfas.pop_back();
			}
			prefix = "";
		}
		else if (postfixRE[i] == '$')
		{
			if (prefix.size() > 0)
				nfas.push_back(searchForToken(prefix));
			if (nfas.size() > 1)
			{
				concat(nfas[nfas.size() - 2], nfas[nfas.size() - 1]);
				nfas.pop_back();
			}
			prefix = "";
		}
		else if (postfixRE[i] == '-')
		{
			if (prefix.size() == 2)
			{
				nfas.push_back(charRangeNFA(prefix[0], prefix[1]));
				prefix = "";
			}
		}
		else if (postfixRE[i] == '+')
		{
			if (prefix.size() > 0)
				nfas.push_back(searchForToken(prefix));
			if (nfas.size() > 0)
			{
				positiveClosure(nfas[nfas.size() - 1]);
				prefix = "";
			}
		}
		else if (postfixRE[i] == '*')
		{
			if (prefix.size() > 0)
				nfas.push_back(searchForToken(prefix));
			if (nfas.size() > 0)
			{
				kleeneClosure(nfas[nfas.size() - 1]);
				prefix = "";
			}
		}
		else if (postfixRE[i] == '\\')
		{
			++i;
			if (postfixRE[i] == 'L' || postfixRE[i] == 'l')
				prefix += ' ';
			else
				prefix += postfixRE[i];
		}
		else if (postfixRE[i] == ' ')
		{
			if (prefix.size() > 0)
			{
				nfas.push_back(searchForToken(prefix));
			}
			prefix = "";
		}
		else
		{
			prefix += postfixRE[i];
		}
	}
	// std::cout << nfas.size();
	if (prefix.size() > 0)
	{
		nfas.push_back(searchForToken(prefix));
	}
	return nfas[0];
}
NFA *searchForToken(string token_class)
{
	if (REDefinitions.size() > 0)
	{
		std::map<string, string>::iterator it = REDefinitions.find(token_class);
		if (it != REDefinitions.end())
		{
			string postRE = parsing_RE(it->second);
			return constructNFAFromREPostfix(postRE);
		}
	}
	return wordNFA(token_class);
}
// construct nfa for c1-c2
NFA *charRangeNFA(char c1, char c2)
{
	std::vector<NFA *> nfaRange(c2 - c1 + 1);
	// cout << c2 << endl;
	// cout << c1 << endl;
	// cout << nfaRange.size();
	for (int i = 0; i < nfaRange.size(); i++)
	{
		nfaRange[i] = charNFA(char(c1 + i));
	}
	State *start = new State();
	State *accept = new State();
	// accept->setAccepted(true);
	std::vector<State *> acceptNextState(1);
	acceptNextState[0] = accept;
	std::vector<State *> startNextStates(nfaRange.size());
	for (int i = 0; i < nfaRange.size(); i++)
	{
		nfaRange[i]->getAcceptState()->addTransition(' ', acceptNextState);
		startNextStates[i] = nfaRange[i]->getStartState();
	}
	start->addTransition(' ', startNextStates);
	NFA *newNFA = new NFA(start);
	newNFA->setAcceptState(accept);
	for (int i = 0; i < nfaRange.size(); i++)
	{
		newNFA->addCharToAlphabet(char(c1 + i));
	}
	return newNFA;
}

// construct nfa for word
NFA *wordNFA(string word)
{
	NFA *newNFA = charNFA(word[0]);
	newNFA->addCharToAlphabet(word[0]);
	for (int i = 1; i < word.size(); i++)
	{
		NFA *nfa = charNFA(word[i]);
		std::vector<State *> nextStates(1);
		nextStates[0] = nfa->getStartState();
		newNFA->getAcceptState()->addTransition(' ', nextStates);
		newNFA->getAcceptState()->setAccepted(false);
		newNFA->setAcceptState(nfa->getAcceptState());
		newNFA->addCharToAlphabet(word[i]);
	}
	// newNFA->getAcceptState()->set_token_class(word);
	return newNFA;
}
// construct nfa for one symbol
NFA *charNFA(char symbol)
{
	State *start = new State();
	State *accept = new State();
	// std::cout << symbol <<std::endl;
	std::vector<State *> startNextStates(1);
	startNextStates[0] = accept;
	(*start).addTransition(symbol, startNextStates);
	// cout << (*start).getTransitions().size()<<endl;
	NFA *newNFA = new NFA(start);
	(*newNFA).setAcceptState(accept);
	newNFA->addCharToAlphabet(symbol);
	return newNFA;
}
// construct nfa for + operator
void positiveClosure(NFA *nfa)
{
	State *start = new State();
	State *accept = new State();
	std::vector<State *> startNextStates(1);
	startNextStates[0] = (*nfa).getStartState();
	(*start).addTransition(' ', startNextStates);

	std::vector<State *> acceptNextStates(2);
	acceptNextStates[0] = accept;
	acceptNextStates[1] = (*nfa).getStartState();
	(*(*nfa).getAcceptState()).addTransition(' ', acceptNextStates);

	(*nfa).setStartState(start);
	(*nfa).setAcceptState(accept);
}

// construct nfa for * operator
void kleeneClosure(NFA *nfa)
{
	State *start = new State();
	State *accept = new State();
	std::vector<State *> nextStates(2);
	nextStates[0] = accept;
	nextStates[1] = (*nfa).getStartState();
	(*start).addTransition(' ', nextStates);
	(*(*nfa).getAcceptState()).addTransition(' ', nextStates);

	(*nfa).setStartState(start);
	(*nfa).setAcceptState(accept);
}

// concatenation of two nfas
void concat(NFA *nfa1, NFA *nfa2)
{
	std::vector<State *> AcceptNextStates(1);
	AcceptNextStates[0] = (*nfa2).getStartState();
	(*(*nfa1).getAcceptState()).addTransition(' ', AcceptNextStates);
	(*(*nfa1).getAcceptState()).setAccepted(false);
	(*nfa1).setAcceptState((*nfa2).getAcceptState());
	for (const auto elem : nfa2->getAlphabet())
	{
		nfa1->addCharToAlphabet(elem);
	}
}

NFA *unionNFAS(NFA *nfa1, NFA *nfa2)
{
	State *start = new State();
	State *accept = new State();
	std::vector<State *> StartNextStates(2);

	StartNextStates[0] = (*nfa1).getStartState();
	StartNextStates[1] = (*nfa2).getStartState();

	(*start).addTransition(' ', StartNextStates);

	std::vector<State *> AcceptNextStates(1);
	AcceptNextStates[0] = accept;
	(*(*nfa1).getAcceptState()).addTransition(' ', AcceptNextStates);
	(*(*nfa2).getAcceptState()).addTransition(' ', AcceptNextStates);
	(*(*nfa1).getAcceptState()).setAccepted(false);
	(*(*nfa2).getAcceptState()).setAccepted(false);
	NFA *newNFA = new NFA(start);
	(*newNFA).setAcceptState(accept);
	newNFA->setAlphabet(nfa1->getAlphabet());
	for (const auto elem : nfa2->getAlphabet())
	{
		newNFA->addCharToAlphabet(elem);
	}
	return newNFA;
}

