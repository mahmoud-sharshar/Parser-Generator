#include <iostream>

#include "anaylser.h"
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <chrono>
using namespace std::chrono;

void traverseNFA(NFA *nfa)
{
	ofstream nfaOutput;
	nfaOutput.open("nfaOutput.txt");
	queue<State *> nfaStates;
	nfaStates.push((*   nfa).getStartState());
	int num = 0;
	(*(*nfa).getStartState()).traversed = true;
	(*(*nfa).getStartState()).state_id = ++num;
	while (!nfaStates.empty())
	{
		State *topState = nfaStates.front();
		//cout << (*topState).isAccepted() << endl;
		// std::map<char, std::vector<State *>>::iterator it = topState->getTransitions().begin();
		// cout << " transitions size " << topState->getTransitions().size() << endl;
		for (std::pair<char, std::vector<State *>> element : topState->getTransitions()) {
			// cout << "entered"<<endl;
			nfaOutput << "state " << topState->state_id << " under (" << element.first << ") goes to " << endl
				 << "        ";
			for (int i = 0; i < (element.second).size(); i++)
			{

				if (!(element.second[i]->traversed))
				{
					nfaStates.push(element.second[i]);
					element.second[i]->traversed = true;
					element.second[i]->state_id = ++num;
				}
				nfaOutput << "State ";
				nfaOutput << element.second[i]->state_id;
				nfaOutput << ", ";
			}
			nfaOutput << endl;


		}
		//cout << nfaStates.size()<<endl;
		nfaStates.pop();
	}
}

int main()
{
	string filePath;
	getline(cin,filePath);
	NFA* nfa = generateNFA(filePath);
	traverseNFA(nfa);
	/*for(auto token: (*nfa).getTokensPriority()){
        cout << token.first << " " << token.second << endl;
	}*/

	DFA* dfa = new DFA(nfa);
	auto start = high_resolution_clock::now();
    (*dfa).convert_to_DFA();
    auto stop = high_resolution_clock::now();
    State* ss = (*dfa).inital_state ;
   // cout << (*(*ss).get_next_DFA_state('}')).state_id << endl ;
//   /* set<State> ss ;
//    ss.insert((*(*nfa).getStartState()));
//    set<State> states = (*dfa).calculate_epison_closure(ss);
//    states = (*dfa).calculate_epison_closure(states);
//    cout << states.size() << endl;*/
//    for(auto state:(*dfa).DFA_states){
//        cout << (*state).state_id << " " << (*state).get_token_class() << endl;
//      /*  for(auto next_state:(*state).next_DFA_states){
//            cout << next_state.first << "  :  " << (*(next_state.second)).state_id << endl;
//        }
//        cout << "------------------------------\n" ;*/
//    }
//    auto duration = duration_cast<microseconds>(stop - start);
//    cout << "\nTime taken by function: "
//         << duration.count()/1000000.0 << " seconds" << endl;
  //  (*dfa).minimization();
    Lexical_analyser* anaylser = new Lexical_analyser("inputf.txt" , dfa);
    (*anaylser).get_tokens();

    freopen("output.txt", "w", stdout);
    for(auto token:anaylser->get_token_values()){
        cout << token << endl;
    }
	return 0;

}

// lexical rules ex1.txt
//  (?<={).+(?=})

// (?<=\[).+(?=\])

// .+=.+

// .+(?=\=)

// (?<=\=).+

// (?<=:).+

// .+(?=:)
