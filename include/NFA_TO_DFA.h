#include<iostream>
#include<bits/stdc++.h>
#include"REToNFA.h"
using namespace std ;

class DFA
{
    private :
        // object from NFA
        NFA* NFA_Machine ;


    public :
        // constructor
        DFA(){}
        DFA(NFA* NFA_Machine){
            this->NFA_Machine = NFA_Machine;
            this->alphabets = (*NFA_Machine).getAlphabet();
            this->token_classes =(*NFA_Machine).getTokensPriority();
            this->dead_state = new State(-1,false);
        }

            // first step  :)
        //calculate all equvilant states using epison transition
        vector<State> calculate_epison_closure(vector<State> &NFA_states , State* state );

        // calculate the transition for dfa state
        vector<State> move(State* dfa_state , char symbol);

        // determine the acceptance status for dfa state
        void check_acceptance(State* dfa_state);


        // check if the two states are equal or not
        bool check_equality(const vector<State> &left , State* right);

        // convert given NFA to DFA
        void convert_to_DFA();

        void minimization();

        // test and return
        vector<State*> DFA_states ;
        unordered_set<char> alphabets;
        map<string , int > token_classes;
        State* inital_state ;
        State* dead_state  ;

};

