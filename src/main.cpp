#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "parse_table.h"
#include <stack> 
#include "parser_stack.h"
#include "parsing.h"
using namespace std;

 bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs)
 {

	// Open the File
 	std::ifstream in(fileName.c_str());
 
 	// Check if object is valid
 	if(!in)
 	{
 		std::cerr << "Cannot open the File : "<<fileName<<std::endl;
 		return false;
 	}
 
 	std::string str;
 	// Read the next line from File untill it reaches the end.
 	while (std::getline(in, str))
 	{
 		// Line contains string of length > 0 then save it in vector
 		if(str.size() > 0)
 			vecOfStrs.push_back(str);
 	}
 	//Close The File
 	in.close();
 	return true;
 }
void PrintStack(stack<stack_element> s) 
{ 
    // If stack is empty then return 
    if (s.empty())  
        return; 
      
  
    stack_element x = s.top(); 
  
    // Pop the top element of the stack 
    s.pop(); 
  
    // Recursively call the function PrintStack 
    PrintStack(s); 
  
    // Print the stack element starting 
    // from the bottom 
    cout << x.name << " "; 
  
    // Push the same element onto the stack 
    // to preserve the order 
    s.push(x); 
} 

void stack_parser(Parsing_table* parser_table){
   stack<stack_element> mystack;
   std::vector<std::string> vecOfStr;
	bool result = getFileContent("I:\\kimo project\\Parser-Generator\\data.txt", vecOfStr);
 
	// if(result)
	// {
	// 	// Print the vector contents
	// 	for(std::string & line : vecOfStr)
	// 		std::cout<<line<<std::endl;
	// }
    stack_element* stack = new stack_element();
    stack->name="$";
    stack->terminal=true;
    mystack.push(*stack);
    stack  = new stack_element();
    stack->name="E";
    mystack.push(*stack);
    int counter=0;
     while(!mystack.empty()){
        stack_element v = mystack.top();
        mystack.pop();
        // cout<<v<<"    "<<vecOfStr[counter]<<endl;
        pair<string,string> p;
        p.first = v.name;
        if(v.name != "#"){
        p.second = vecOfStr[counter];
        //cout<<"input is "<<vecOfStr[counter]<<endl;
        if(parser_table->sync[p] && parser_table->get_parsing_table().count(p)==0){
            continue;
        }
        if(p.first==(p.second)){
            counter++;
            cout<<"matched"<<endl;
        }
        else if(v.terminal){
            if(v.name == "$"){
            cout<<"error "<<vecOfStr[counter]<<"is extra token"<<endl;
            }
        else
            cout<<"error "<<vecOfStr[counter]<<"is extra token"<<endl;
            counter++;
            mystack.push(v);
            continue;
        }
        else if(parser_table->get_parsing_table().count(p)==0){
            cout<<"Syntax error"<<endl;
            counter++;
            mystack.push(v);
        }
        else {
            std::vector<ProductionPart> myvector;
            for (auto  x :parser_table->get_parsing_table()[p])
            { 
                  myvector.push_back(x);

            } 
            vector<ProductionPart>::iterator i = myvector.end();
            while (i != myvector.begin())
            {
              --i;
              stack_element* z = new stack_element();
              z->name=(*i).name;
              z->terminal = (*i).terminal;
              mystack.push(*z);
    /*do stuff */
            } 
        }
        }
    PrintStack(mystack);
    if(counter < (signed int)vecOfStr.size())
       cout<<"       input is " <<vecOfStr[counter]<<endl;
        }
    PrintStack(mystack);
}

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
    cout <<"\n------------------------------------------------------------\n";
    cout << "stacking...."<<endl;
    cout<<"reading start with"<<parser_table.start_symbol<<endl;
	cout << "Enter The path of the file containing the tokens: "<< endl;
    getline(cin,filePath);
    Parsing_stack stacking(&parser_table,filePath);
    // stacking.stack_parser(&parser_table);
     return 0;
}
