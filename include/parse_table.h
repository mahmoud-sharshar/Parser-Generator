#ifndef PARSE_TABLE_H_INCLUDED
#define PARSE_TABLE_H_INCLUDED

#include<bits/stdc++.h>
#include "productionsParser.h"

using namespace std;

// A hash function used to hash a pair of any kind
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Parsing_table{
    private:
        unordered_map<string, set<string> > first_set ;
        unordered_map<string, set<string> > follow_set ;
        unordered_map< pair<string,string> , vector<ProductionPart> , hash_pair> parse_table;
        // unordered_map< pair<string,string> , bool , hash_pair > sync ;
        string input_file ;
        ProductionParser* parser;
        string start_symbol ;
        set<string> calculate_first(string symbol , bool terminal);
        void calculate_first_set();
        void calculate_follow_set();
        void construct_Parsing_table();
        void eliminate_left_recursion();
        void left_factoring();
    public:
        Parsing_table();
        Parsing_table(string input_file);
        map<string,vector<vector<ProductionPart>>> productions;
        unordered_map< pair<string,string> , vector<ProductionPart> , hash_pair> get_parsing_table();
        unordered_map< pair<string,string> , bool , hash_pair > sync ;
        string get_start_symbol() {return start_symbol;}

};
#endif // PARSE_TABLE_H_INCLUDED
