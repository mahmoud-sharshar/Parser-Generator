#include "parsing.h"

using namespace std ;

// constructor
Parsing_stack::Parsing_stack(Parsing_table* parser_table,std::string fileName){
   this->stack_parser(parser_table,fileName);
}
 bool Parsing_stack::getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs)
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
void Parsing_stack::PrintStack(stack<stack_element> s) 
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
void Parsing_stack::stack_parser(Parsing_table* parser_table,std::string fileName){ 
   stack<stack_element> mystack;
   std::vector<std::string> vecOfStr;
	bool result = getFileContent(fileName, vecOfStr);
 
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
    stack->name=parser_table->start_symbol;
    mystack.push(*stack);
	PrintStack(mystack);
    cout<<endl;
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
            PrintStack(mystack);
            cout<<endl;
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
        else{
            cout<<"error "<<v.name<<"is missing"<<endl;
            if(counter<vecOfStr.size()-1)
               {counter++;
                 mystack.push(v);}
            PrintStack(mystack);
            cout<<endl;
            continue;
        }
        }
        else if(parser_table->get_parsing_table().count(p)==0){
            cout<<"Syntax error"<<parser_table->get_parsing_table().count(p)<<endl;
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

