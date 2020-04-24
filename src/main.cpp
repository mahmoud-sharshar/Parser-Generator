#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
#include "parser_table.cpp"
#include "REToNFA.cpp"
#include "anaylser.cpp"
int main()
{
    string filePath;
    /*cout << "Enter The path of the file containing production rules: "<< endl;
    getline(cin,filePath);
    Parsing_table parser_table(filePath);
    for(auto p:parser_table.productions){
        cout << "LHS:"<<p.first<<endl;
        for (auto i : p.second)
        {
            for (auto j : i)
            {
                cout << j.name << " ";
            }
            cout << "\n";
        }
        cout << "--------------------\n" <<endl;
    }*/
	NFA* nfa = generateNFA(filePath);
	
    return 0;
}
