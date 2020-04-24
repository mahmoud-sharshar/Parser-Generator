#include "NFA_TO_DFA.h"

#include<bits/stdc++.h>

using namespace std;
// State class
State::State(int state_id , bool accepted )
{
    this->state_id  = state_id;
    this->accepted = accepted;
    this->token_class = " ";
}

State* State::get_next_DFA_state(char symbol)
{
    return this->next_DFA_states[symbol];
}

vector<State> State::get_epison_closure()
{
    return this->epison_closure;
}

void State::set_epison_closure(vector<State> &epison_closure)
{
    this->epison_closure =  epison_closure;
}


// DFA class
vector<State> DFA::calculate_epison_closure(vector<State> &NFA_states , State * dfa_state)
{
    vector<State> states ;
    unordered_map<int,bool> NFA_states_id ;
    // insert all states into stack
    // O(n)
    for(auto state:NFA_states){
        states.push_back(state);
        NFA_states_id[state.state_id]=true;
        (*dfa_state).NFA_states_id[state.state_id]=true;
    }
    // O(n^2)
    int len = states.size();
    for(int i=0;i<len;i++){
        vector<State*> next_states = states[i].get_next_NFA_states(' ');
        for(auto state:next_states){
                if(NFA_states_id.find((*state).state_id) == NFA_states_id.end()){
                    NFA_states.push_back(*state);
                    (*dfa_state).NFA_states_id[(*state).state_id]=true;
                    NFA_states_id[(*state).state_id]=true;
                    states.push_back(*state);
                    len++;
                }
        }
    }
    return NFA_states;
}
vector<State> DFA::move(State* dfa_state , char symbol)
{
    vector<State> next_states ;
    vector<State> state_closure = (*dfa_state).get_epison_closure();
    unordered_map<int,bool> states_id ;
    for(auto state:state_closure){
        vector<State*>  states = state.get_next_NFA_states(symbol);
        for(auto s:states){
            if(states_id.find((*s).state_id) == states_id.end()){
                 next_states.push_back(*s);
                 states_id[(*s).state_id]=true;
            }
        }
    }
    return next_states ;
}

bool DFA::check_equality(const vector<State> &left , State* right)
{
    if(left.size()!=(*right).NFA_states_id.size()) return false;
    for(auto state:left){
        if((*right).NFA_states_id.find(state.state_id)==(*right).NFA_states_id.end()) return false;
    }
    return true;
}

void DFA::check_acceptance(State* dfa_state)
{
    string token_class ;
    int priority =INT_MAX;
    vector<State> epison_closure = (*dfa_state).get_epison_closure();
    bool check =  false;
    for(auto state:epison_closure){
        if(state.isAccepted()){
            if((this->token_classes).find(state.get_token_class()) != (this->token_classes).end())
                {
                     int new_prioity = this->token_classes[state.get_token_class()];
                        if(priority>new_prioity){
                            priority = new_prioity;
                            token_class = state.get_token_class();
                        }
                }
            check=true;
         //   cout << state.state_id << endl;
        }
    }
    if(check){
        (*dfa_state).setAccepted(true);
        (*dfa_state).set_token_class(token_class);
    }
}
void DFA::convert_to_DFA()
 {
    // calculate inital state for DFA

    vector<State> inital ;
    State start = *((*(this->NFA_Machine)).getStartState());
    inital.push_back(start);
    State* state = new State(0,false);
    (*state).NFA_states_id[start.state_id]=true;
    vector<State> inital_closure = calculate_epison_closure(inital,state);
    (*state).set_epison_closure(inital_closure);
    this->inital_state = state;
    this->DFA_states.push_back(inital_state);

     check_acceptance(state);


    stack<State*> reminder_states ;
    reminder_states.push(this->inital_state);

    while(!reminder_states.empty())
    {
        State* new_state = reminder_states.top();
        reminder_states.pop();
        for (auto symbol:alphabets){
            State* new_DFA_state = new State(DFA_states.size(),false);
            vector<State> transition = move(new_state,symbol);
            vector<State> next_states = calculate_epison_closure(transition , new_DFA_state );

            if(next_states.size()==0){
                 (*new_state).next_DFA_states[symbol]=this->dead_state;
                 continue;
            }
           // cout << "--> " << next_states.size() << endl;
            bool found = false;
            for(auto state:DFA_states){
                if(check_equality(next_states,state)){
                    new_DFA_state = state;
                    found=true;
                   // cout << "ISA" << endl;
                    break;
                }
            }
            if(!found){
               // new_DFA_state = new State(DFA_states.size(),false);
                (*new_DFA_state).set_epison_closure(next_states);
                reminder_states.push(new_DFA_state);
                DFA_states.push_back(new_DFA_state);
                check_acceptance(new_DFA_state);
            }
            (*new_state).next_DFA_states[symbol]=new_DFA_state;
           // cout << (*new_DFA_state).state_id << endl;
          //  cout << (*new_state).get_epison_closure().size() << endl;
        }
    }
 }

