#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "parse_table.h"
#include <stack> 
#include "parser_stack.h"
#include "parsing.h"
using namespace std;


int main()
{
    string filePath;
    cout << "Enter The path of the file containing production rules: "<< endl;
    getline(cin,filePath);
    Parsing_table parser_table(filePath);
    for(auto s:parser_table.get_parsing_table()){
        pair<string,string> p = s.first;
        cout << p.first << " " << p.second << endl;
        for(auto t:s.second){
            cout << t.name << " " ;
        }
        cout << "\n---------------------------\n";
    }
    cout <<"\n------------------------------------------------------------\n";
    cout << "stacking...."<<endl;
    cout<<"reading start with"<<parser_table.start_symbol<<endl;
	cout << "Enter The path of the file containing the tokens: "<< endl;
    getline(cin,filePath);
    Parsing_stack stacking(&parser_table,filePath);
    cout<<"";
    // stacking.stack_parser(&parser_table);
     return 0;
}
