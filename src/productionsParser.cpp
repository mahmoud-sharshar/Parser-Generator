#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "productionsParser.h"

using std::ifstream;
using std::map;
using std::pair;
using std::string;
using std::vector;


ProductionParser::ProductionParser(std::string file)
{
    this->filePath = file;
    findRules();
    parseProductionRuls();
}
void ProductionParser::findRules()
{
    ifstream file(filePath);
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line[line.find_first_not_of(' ')] == '#')
                productionsRules.push_back(line);
            else if (productionsRules.size() > 0)
            {
                productionsRules[productionsRules.size() - 1] += " " + line;
            }
            else
            {
                std::cout << "Error in the first line, Production rule line must begin with #" << std::endl;
            }
        }
        file.close();
    }
    else
    {
        std::cout << "Enter valid file path" << std::endl;
    }
}

bool ProductionParser::validateProductionStructure(string production)
{
    if ((production.find_first_of('#') != production.find_last_of('#')) || (production.find_first_of('=') != production.find_last_of('=')))
    {
        return false;
    }
    return true;
}

ProductionPart::ProductionPart(string name, bool t)
{
    this->name = name;
    this->terminal = t;
}

pair<string, vector<vector<ProductionPart>>> ProductionParser::findProductionParts(string production)
{
    string lhs = "";
    string rhs = "";

    unsigned i = production.find_first_not_of('#');
    // find LHS of the Production
    while (production[i] != '=')
    {
        if (production[i] != ' ')
        {
            lhs += production[i];
        }
        i++;
    }
    i++;

    // find RHS parts of ths production
    vector<vector<ProductionPart>> RHSparts;
    vector<ProductionPart> part;
    while (i < production.size())
    {
        if (production[i] == '|')
        {
            if (rhs.size() > 0)
            {
                ProductionPart nonTerminal(rhs, false);
                part.push_back(nonTerminal);
                rhs = "";
            }
            if (part.size() > 0)
            {
                RHSparts.push_back(part);
                part.clear();
            }
        }
        else if (production[i] == '\'')
        {
            if (rhs.size() > 0)
            {
                ProductionPart nonTerminal(rhs, false);
                part.push_back(nonTerminal);
                rhs = "";
            }
            while (production[++i] != '\'')
            {
                rhs += production[i];
            }
            if (rhs.size() > 0)
            {
                ProductionPart terminal(rhs, true);
                part.push_back(terminal);
                rhs = "";
            }
        }
        else if (production[i] == ' ' && rhs.size() > 0)
        {
            ProductionPart nonTerminal(rhs, false);
            part.push_back(nonTerminal);
            rhs = "";
        }
        else if (production[i] == '\\' && production[++i] == 'L')
        {
             if (rhs.size() > 0)
            {
                ProductionPart nonTerminal(rhs, false);
                part.push_back(nonTerminal);
                rhs = "";
            }
            ProductionPart nonTerminal("#", false);
            part.push_back(nonTerminal);
        }
        else if (production[i] != ' ')
        {
            rhs += production[i];
        }
        ++i;
    }
    if (rhs.size() > 0)
    {
        ProductionPart nonTerminal(rhs, false);
        part.push_back(nonTerminal);
        rhs = "";
    }
    if (part.size() > 0)
    {
        RHSparts.push_back(part);
    }
    return {lhs, RHSparts};
}

void ProductionParser::parseProductionRuls()
{
    bool first = false;
    if (productionsRules.size() > 0)
    {
        for (auto production : productionsRules)
        {
            productions.insert(findProductionParts(production));
            if(!first){
                first = true;
                startProduction = productions.begin()->first;
            }
        }
    }
}
