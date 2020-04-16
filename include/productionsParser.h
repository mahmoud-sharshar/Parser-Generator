#ifndef PRODUCTION_PARSER
#define PRODUCTION_PARSER

#include <vector>
#include <string>
#include <map>

namespace ProductionParser
{
struct ProductionPart
{
    std::string name{""};
    bool terminal{false};
    ProductionPart(std::string name, bool t);
};
std::vector<std::string> findRules(std::string filePath);
bool validateProductionStructure(std::string production);
std::pair<std::string, std::vector<std::vector<ProductionPart>>> findProductionParts(std::string production);
std::map<std::string, std::vector<std::vector<ProductionPart>>> parseProductionRuls(std::string filePath);

}; // namespace ProductionParser

#endif