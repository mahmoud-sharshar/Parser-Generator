#include "parse_table.h"

using namespace std ;

// constructor
Parsing_table::Parsing_table(string input_file){
    this->parser = parser ;
    this->input_file = input_file;
    this->parser = new ProductionParser(input_file);
    this->productions = parser->getProductions();
    this->start_symbol = parser->getStartProduction();
    // this->eliminate_left_recursion();
}

set<string> Parsing_table::calculate_first(string symbol , bool terminal)
{
    set<string> new_first_set ;
    // case one : terminal symbol
    if(terminal)
    {
        new_first_set.insert(symbol);
        first_set[symbol]  = new_first_set;
        return new_first_set;
    }

    // case two : non terminal symbol
    vector< vector<ProductionPart> > production ;
    production = this->productions[symbol] ;
    int epison_size = 0 ;
    for(auto rule:production){
        bool is_epsion = true ;
        for(auto part:rule){
            if(!is_epsion) break;
            set<string> part_first_set ;
            // calculated before
            if(first_set.find(part.name)!=first_set.end()){
                part_first_set = first_set[part.name] ;
            }
            else{
                part_first_set = calculate_first(part.name,part.terminal);
            }
            is_epsion = false;
            for(auto s:part_first_set){
                    if(s=="#"){
                         epison_size++;
                         is_epsion = true;
                         if(new_first_set.size()==1) new_first_set.insert(s);
                    }
                    else new_first_set.insert(s);
            }
        }
        int len = rule.size();
        if(epison_size==len) new_first_set.insert("#");
    }
    first_set[symbol]=new_first_set;
    return new_first_set ;
}

void Parsing_table::calculate_first_set(){
    for (auto non_terminal:this->productions){
        if(first_set.find(non_terminal.first)==first_set.end()){
                calculate_first(non_terminal.first,false);
        }
    }
}

void Parsing_table::calculate_follow_set(){
    //case 1 : start symbol
    follow_set[start_symbol].insert("$");
    // case 2 : first set of adjacent symbol is in the follow set of the left non terminal
    for (auto production:this->productions){
       // cout << "\n" << production.first << " :: \n" ;
        for(auto rule:production.second){
            int len = rule.size();
            for(int i=0;i<len-1;i++){
                if(!rule[i].terminal){
                   // cout << rule[i].name << endl;
                  bool is_epison = true;
                  for(int j=i+1;j<len;j++){
                        if(!is_epison) break;
                        if(rule[j].terminal){
                            if(first_set.find(rule[j].name)==first_set.end()){
                                set<string> new_first_set ;
                                new_first_set.insert(rule[j].name);
                                first_set[rule[j].name]=new_first_set;
                            }
                        }
                        set<string> new_follow_set= first_set[rule[j].name];
                        new_follow_set.erase("#");
                        if(follow_set.find(rule[i].name)==follow_set.end()){
                            follow_set[rule[i].name] =new_follow_set;
                        }
                        else{
                            for(auto s:new_follow_set){
                                follow_set[rule[i].name].insert(s);
                            }
                        }
                        if(!first_set[rule[j].name].count("#")) is_epison = false;
                  }
                }
            }
        }
    }

    // case 3 : everything in follow set of LHS is in the follow set of the last symbol in RHS
    for(auto production: Parsing_table::productions){
        set<string> old_follow_set = follow_set[production.first];
        for(auto rule:production.second){
            bool is_epison = true ;
            for(int i=rule.size()-1;i>=0;i--){
                if(!is_epison||rule[i].terminal) break;
                else if(follow_set.find(rule[i].name)==follow_set.end()){
                        follow_set[rule[i].name] =old_follow_set;
                    }
                else{
                     for(auto s:old_follow_set){
                         follow_set[rule[i].name].insert(s);
                      }
                }
                if(!first_set[rule[i].name].count("#")) is_epison=false;
            }
        }
    }

}

void Parsing_table::eliminate_left_recursion(){

    map<string , vector< vector<ProductionPart> > > new_productions;
    unordered_map<string,bool> previous_symbols ;
    for(auto production:productions){
        cout << production.first << endl;
        vector < vector<ProductionPart> > new_rules ;
        for(auto rule:production.second){
            if( previous_symbols.find(rule[0].name) != previous_symbols.end() ){
                for(auto previous_rule: new_productions[rule[0].name]){
                    vector<ProductionPart> new_rule = previous_rule;
                    int len = rule.size();
                    for(int i=1;i<len;i++){
                        new_rule.push_back(rule[i]);
                    }
                    new_rules.push_back(new_rule);
                }
            }else{
                new_rules.push_back(rule);
            }
        }
        previous_symbols[production.first]=true;
        // eliminate immediate left recursion
       vector< vector<ProductionPart> >immediate;
       vector< vector<ProductionPart> > non_immediate;
        ProductionPart new_part(production.first + "D", false);
        for(auto rule : new_rules){
            if(rule[0].name == production.first){
                immediate.push_back(rule);
            }else{
                non_immediate.push_back(rule);
            }
        }
        // no immediate left recursion
        if( immediate.size() == 0){
            new_productions[production.first] = non_immediate;
        }else{
            for(auto &non:non_immediate){
                non.push_back(new_part);
            }
            for(auto &immed: immediate){
                immed.erase(immed.begin());
                immed.push_back(new_part);
            }
            new_productions[production.first] = non_immediate;
            vector<ProductionPart> epison_rule ;
            ProductionPart epison_part("#", true);
            epison_rule.push_back(epison_part);
            immediate.push_back(epison_rule);
            new_productions[production.first + "D"] = immediate;
        }
    }
    productions = new_productions;
}

void Parsing_table::construct_Parsing_table(){

    this->calculate_first_set();
    this->calculate_follow_set();

    for(auto production:Parsing_table::productions){
        string non_terminal = production.first;
        for(auto rule:production.second){
            bool is_epison = true;
            bool epison = false;
            // case one using first set of the
            for(auto part:rule){
                if(!is_epison) break;
                string symbol = part.name;
                set<string> first = first_set[symbol];
                for(auto terminal:first){
                    if(terminal!="#")this->parse_table[{non_terminal,terminal}] = rule;
                }
                is_epison = false;
                if(first.count("#")) {
                    is_epison= true;
                    epison = true;
                }
            }
          // case two if first set contain # then using follow set
          if(epison){
                set<string> follow = follow_set[non_terminal];
                for(auto terminal:follow){
                    this->parse_table[{non_terminal,terminal}] = rule;
                }
          }
        }
    }

    // calculate sync
    for(auto production:productions){
        for(auto terminal:follow_set[production.first]){
            sync[{production.first,terminal}] = true ;
        }
    }
}
 unordered_map< pair<string,string> , vector<ProductionPart> , hash_pair> Parsing_table::get_parsing_table(){
     this->construct_Parsing_table();
     return this->parse_table;
 }
