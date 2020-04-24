#include "NFA_TO_DFA.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

void DFA::minimization()
{
    unordered_set<char> alphabet = this->alphabets;
    int group_num = token_classes.size() + 1;
    vector< vector<State*> > groups(group_num);
    int minimum = INT_MAX ;
    for(auto token:token_classes){
      //  cout << token.first << " " << token.second << endl;
        minimum = min(token.second,minimum);
    }
    for (int i = 0; i < DFA_states.size(); i++)
    {
        State* s = DFA_states[i];
        if (!(*s).isAccepted())
        {
            groups.at(group_num-1).push_back(s);
            (*s).set_partition(group_num - 1);
        }
        else
        {
            string str = (*s).get_token_class();
            int index = token_classes[str];
           // cout << "index : " << index << endl;
            groups[index-minimum].push_back(s);
            (*s).set_partition(index);
        }
    }
   /* for(int i=0;i<groups.size();i++){
        cout << i << endl;
        for(int j=0;j<groups[i].size();j++){
            cout << (*(groups[i][j])).state_id <<" " ;
        }
        cout << "\n-----------------" << endl;
    }*/
    vector< vector<State*> > partitions;
   for (int i = 0; i < group_num; i++)
    {
        if (groups[i].size() != 0)
        {
            partitions.push_back(groups[i]);
        }
    }
    int part_num = partitions.size();
    bool flag = true;
    while (flag || partitions.size() != part_num)
    {
        flag = false;
        part_num = partitions.size();
        for (int i = 0; i < partitions.size(); i++)
        {
            State* s = partitions[i][0];
            vector<State*> new_partition;
            vector<State*> modified_partition;
            modified_partition.push_back(s);
            for (int j = 1; j < partitions[i].size(); j++)
            {
                State* temp = partitions[i][j];
                int z=0;
                for (auto c:alphabet)
                {
                    if ((*(*s).get_next_DFA_state(c)).get_partition() != (*(*temp).get_next_DFA_state(c)).get_partition())
                    {
                        (*temp).set_partition(partitions.size());
                        new_partition.push_back(temp);
                        break;
                    }
                    else if (z == alphabet.size() - 1)
                    {
                        (*temp).set_partition(i);
                        modified_partition.push_back(temp);
                    }
                     z++;
                }
            }
            if (new_partition.size() != 0)
            {
                partitions.push_back(new_partition);
            }
            partitions[i] = modified_partition;
        }
    }
   for(int i=0;i<partitions.size();i++){
        for(int j=0;j<partitions[i].size();j++){
            (*(partitions[i][j])).set_partition(i) ;
        }
    }
    vector< State*> new_DFA_states ;
    for(int i=0;i<partitions.size();i++){
        new_DFA_states.push_back(partitions[i][0]);
        (*(partitions[i][0])).state_id=i;
        for(auto symbol:alphabet){
            State* state = (*(partitions[i][0])).get_next_DFA_state(symbol);
            int curr = (*state).get_partition();
            (*(partitions[i][0])).next_DFA_states[symbol]=partitions[curr][0];
        }
    }
    this->DFA_states=new_DFA_states;
   /* for(auto state:DFA_states){
       cout << (*state).state_id << endl;
       for(auto next_state:(*state).next_DFA_states){
            cout << next_state.first << "  :  " << (*(next_state.second)).state_id << endl;
        }
        cout << "------------------------------\n" ;
    }*/

}
