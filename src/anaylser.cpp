#include "anaylser.h"
#include "REToNFA.cpp"
using namespace std;
/*     token class   */
// constructor
Token::Token(string token_class , string token_value)
{
    this->token_class=token_class;
    this->token_value=token_value;
}

// getter ans setter
string Token::get_token_class()
{
    return this->token_class;
}
string Token::get_token_value()
{
    return this->token_value;
}

/*    Lexical_analyser class  */
// constructor
Lexical_analyser::Lexical_analyser(string filePath , DFA* dfa_machine)
{
    this->input_file = filePath ;
    this->dfa_machine = dfa_machine;
    this->current_line = 1 ;
    this->current_char = 0 ;
}
// implemention of the algorithm to get the next token
Token* Lexical_analyser::get_next_token(ifstream &file , bool error , int current_char)
{
      State* current_state = (*(this->dfa_machine)).inital_state;
      State* dead_state = ((*(this->dfa_machine)).dead_state);
      int initial_char = current_char;
      char symbol[1];
      file.seekg(current_char,ios::beg);
      file.read(symbol,1);
      while(file.peek() != EOF&&(symbol[0]==' '|| symbol[0]=='\n')){
        current_char++;
        file.seekg(current_char,ios::beg);
        file.read(symbol,1);
      }
      State* accepted_state;
      bool done = false;
      int accepted_char ;
      string token_value = "" ;
      string accepted_token= "";
      while(file.peek() != EOF && symbol[0]!=' '&& current_state!=dead_state)
      {
            token_value+=symbol;
            current_state = (*current_state).get_next_DFA_state(symbol[0]);
            if((*current_state).isAccepted()){
                accepted_state = current_state;
                accepted_char = current_char;
                accepted_token = token_value;
                done=true;
            }
            current_char++;
            file.seekg(current_char,ios::beg);
            file.read(symbol,1);
            if(symbol[0] == '\n'){
                (this->current_line)++;
                current_char++;
                file.seekg(current_char,ios::beg);
                file.read(symbol,1);
            }
      }
      if(token_value=="") return nullptr;
      if(done){
         string token_class = (*accepted_state).get_token_class();
         this->current_char = accepted_char+1;
         file.seekg(this->current_char,ios::beg);
         return new Token(token_class,accepted_token);
      }
      if(!error)
      {
        int final_char = current_char;
        Token* token = error_handler(file,initial_char,final_char,token_value);
        return token;
      }
      return nullptr;

}
// error message
Token* Lexical_analyser::error_handler(ifstream &file , int inital_char , int final_char , string token_value)
{
    int correct_char = final_char;
    bool done = false;
    Token* token ;
    for(int i=inital_char+1;i<=final_char;i++)
    {
        token = this->get_next_token(file,true,i);
        if(token!=nullptr){
            correct_char = i;
            done = true;
        }
    }
    if(done){
        cout << "error message in line : " << this->current_line << "-->" << token_value.substr(0,correct_char) << endl;
        return token ;
    }else{
        cout << "error message in line : " << this->current_line << " --> " << token_value << endl;
    }
}
// get all tokens for the input file
void Lexical_analyser::get_tokens()
{
    ifstream file ;
    file.open(this->input_file);
    while(file.peek() != EOF){
        Token* new_token = get_next_token(file,false,this->current_char);

        if(new_token!=nullptr)  {
            this->symbol_table.push_back({(*new_token).get_token_class() ,
                                                              (*new_token).get_token_value() });
            this->tokens.push_back((*new_token).get_token_value());
            cout <<(*new_token).get_token_class() <<   " " << (*new_token).get_token_value() << endl;
        }
        //cout <<  << " " <<  << endl;
    }
}
