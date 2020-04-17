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

vector<string> ProductionParser::findRules(string filePath)
{
    ifstream file(filePath);
    string line;
    vector<string> productions;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line[line.find_first_not_of(' ')] == '#')
                productions.push_back(line);
            else if (productions.size() > 0)
            {
                productions[productions.size() - 1] += " " + line;
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
    return productions;
}

bool ProductionParser::validateProductionStructure(string production)
{
    if ((production.find_first_of('#') != production.find_last_of('#')) || (production.find_first_of('=') != production.find_last_of('=')))
    {
        return false;
    }
    return true;
}

ProductionParser::ProductionPart::ProductionPart(string name, bool t)
{
    this->name = name;
    this->terminal = t;
}

pair<string, vector<vector<ProductionParser::ProductionPart>>> ProductionParser::findProductionParts(string production)
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

map<string, vector<vector<ProductionParser::ProductionPart>>> ProductionParser::parseProductionRuls(string filePath)
{
    vector<string> productionsRules = findRules(filePath);
    map<string, vector<vector<ProductionPart>>> productions;
    if (productionsRules.size() > 0)
    {
        for (auto production : productionsRules)
        {
            productions.insert(findProductionParts(production));
        }
    }
    return productions;
}
