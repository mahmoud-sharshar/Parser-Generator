#ifndef PRODUCTION_PARSER
#define PRODUCTION_PARSER

#include <vector>
#include <string>
#include <map>

struct ProductionPart
{
    std::string name{""};
    bool terminal{false};
    ProductionPart(std::string name, bool t);
};

class ProductionParser
{
private:
    std::string filePath;
    std::string startProduction;
    std::map<std::string, std::vector<std::vector<ProductionPart>>> productions;
    std::vector<std::string> productionsRules;
    void parseProductionRuls();
    void findRules();
public:
    ProductionParser(std::string filePath);
    std::pair<std::string, std::vector<std::vector<ProductionPart>>> findProductionParts(std::string production);
    bool validateProductionStructure(std::string production);

    std::map<std::string, std::vector<std::vector<ProductionPart>>> getProductions(){return productions;}
    std::string getStartProduction(){return startProduction;}
}; 

#endif