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
    Parsing_table parser_table(filePath);
    for(auto s:parser_table.get_parsing_table()){
        pair<string,string> p = s.first;
        cout << p.first << " " << p.second << endl;
        for(auto t:s.second){
            cout << t.name << " " ;
        }
        cout << "\n---------------------------\n";
    }
    return 0;
}
