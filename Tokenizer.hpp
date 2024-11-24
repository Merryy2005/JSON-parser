#ifndef TOKENIZER
#define TOKENIZER

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <vector>
#include <unordered_map>

//CurlyOpen {
//CurlyClose }
//SquareOpen [
//SquareClose ]
//Colon :
//Comma ,

// we use tokenizer to add new features and we can then modify how tokenization works without changing parser

enum class TokenType {CurlyOpen, CurlyClose, SquareOpen, SquareClose, Colon, Comma, String, Number, Boolean, Null};

class Token
{
    public:
        TokenType type;
        std::string value; //which token it is
    public:
        explicit Token(TokenType t , const std::string& v) : type(t) , value(v){}
};

class Tokenizer
{        
    private:
        const std::string& json;
        std::size_t pos;
    public:
        explicit Tokenizer(const std::string&);
        std::vector<Token> tokenize();
    private:
        void skipWhitespace();
        char next();
        char peek() const; //top 
        std::string parseString();
        std::string parseNumber();
};

#endif //TOKENIZER