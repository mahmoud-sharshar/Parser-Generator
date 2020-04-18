#ifndef PARSE_TABLE_H_INCLUDED
#define PARSE_TABLE_H_INCLUDED

#include<bits/stdc++.h>
#include "productionsParser.h"

using namespace std;

namespace parser_table{
map<string, vector<vector<ProductionParser::ProductionPart>>> productions ;
unordered_map<string, set<string> > first_set ;
unordered_map<string, set<string> > follow_set ;
set<string> calculate_first(string symbol , bool terminal);
void calculate_first_set();
void calculate_follow_set();
};
#endif // PARSE_TABLE_H_INCLUDED
