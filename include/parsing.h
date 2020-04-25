#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED

#include<bits/stdc++.h>
#include "productionsParser.h"
#include "parse_table.h"
#include "parser_stack.h"

using namespace std;

class Parsing_stack{
    private:
         bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs);
         void PrintStack(stack<stack_element> s) ;

    public:
        Parsing_stack(Parsing_table* parser_table);
        void stack_parser(Parsing_table* parser_table);
};
#endif // PARSE_TABLE_H_INCLUDED
