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

namespace parser_table{
map<string, vector<vector<ProductionParser::ProductionPart>>> productions ;
unordered_map<string, set<string> > first_set ;
unordered_map<string, set<string> > follow_set ;
unordered_map< pair<string,string> , vector<ProductionParser::ProductionPart> , hash_pair> parsing_table;
set<string> calculate_first(string symbol , bool terminal);
void calculate_first_set();
void calculate_follow_set();
void construct_parsing_table();
};
#endif // PARSE_TABLE_H_INCLUDED
