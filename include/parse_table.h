#ifndef PARSE_TABLE_H_INCLUDED
#define PARSE_TABLE_H_INCLUDED

// #include<bits/stdc++.h>
#include <string>
#include<unordered_map>
#include <set>
#include <map>
#include <vector>
#include "productionsParser.h"

using std::string;
using std::unordered_map;
using std::set;
using std::map;
using std::vector;

class parser_table{
public:
map<string, vector<vector<ProductionPart>>> productions ;
unordered_map<string, set<string> > first_set ;
unordered_map<string, set<string> > follow_set ;
set<string> calculate_first(string symbol , bool terminal);
void calculate_first_set();
void calculate_follow_set();
};
#endif // PARSE_TABLE_H_INCLUDED
