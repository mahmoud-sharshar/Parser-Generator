#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "productionsParser.h"
#include "parse_table.h"

using namespace std;
int main()
{
    string filePath;
    cout << "Enter The path of the file containing production rules: "<< endl;
    getline(cin,filePath);
    ProductionParser parser(filePath);
    parser_table parserTable;
    cout << "start production: " <<  parser.getStartProduction() << endl;
     cout << "number of productions: " << parser.getProductions().size()<<endl;
     for(auto p:parser.getProductions()){
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
    }
    cout<< "productions parser ended" << endl;
    parserTable.productions = parser.getProductions();
       
    parserTable.calculate_first_set();
    parserTable.calculate_follow_set();
    for(auto p:parserTable.first_set){
        cout << "LHS:"<<p.first<<endl;
        for (auto i : p.second)
        {
                cout << i << " ";

        }
         cout << "\n";
        cout << "--------------------\n" <<endl;
    }
    cout << "#####################################\n" ;
    for(auto p:parserTable.follow_set){
        cout << "LHS:"<<p.first<<endl;
        for (auto i : p.second)
        {
                cout << i << " ";

        }
         cout << "\n";
        cout << "--------------------\n" <<endl;
    }
    return 0;
}
