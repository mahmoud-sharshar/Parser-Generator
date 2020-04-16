#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
#include "productionsParser.h"

int main()
{
    string filePath;
    cout << "Enter The path of the file containing production rules: "<< endl;
    getline(cin,filePath);
    map<string, vector<vector<ProductionParser::ProductionPart>>> productions = ProductionParser::parseProductionRuls(filePath);
    int num = 0;
    for(auto p:productions){
        cout << "LHS:"<<p.first<<endl;
        num++;
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
    cout << "number of productions: " << num<<endl;
    return 0;
}