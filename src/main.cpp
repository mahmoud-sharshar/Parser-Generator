#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
#include "parser_table.cpp"
int main()
{
    string filePath;
    cout << "Enter The path of the file containing production rules: "<< endl;
    getline(cin,filePath);
    parser_table::productions = ProductionParser::parseProductionRuls(filePath);
    int num = 0;
    cout << "number of productions: " << num<<endl;
    parser_table::construct_parsing_table();
    for(auto p:parser_table::first_set){
        cout << "LHS:"<<p.first<<endl;
        for (auto i : p.second)
        {
                cout << i << " ";

        }
         cout << "\n";
        cout << "--------------------\n" <<endl;
    }
    cout << "#####################################\n" ;
    for(auto p:parser_table::follow_set){
        cout << "LHS:"<<p.first<<endl;
        for (auto i : p.second)
        {
                cout << i << " ";

        }
         cout << "\n";
        cout << "--------------------\n" <<endl;
    }
    for(auto s:parser_table::parsing_table){
        pair<string,string> p = s.first;
        cout << p.first << " " << p.second << endl;
        for(auto t:s.second){
            cout << t.name << " " ;
        }
        cout << "\n---------------------------\n";
    }
    return 0;
}
