#ifndef TOKENIZER
#define TOKENIZER

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <vector>

//CurlyOpen {
//CurlyClose }
//SquareOpen [
//SquareClose ]
//Colon :
//Comma ,

enum class TokenType {CurlyOpen, CurlyClose, SquareOpen, SquareClose, Colon, Comma, String, Number, Boolean, Null};

class Tokenizer
{
    private:
        class Token
        {
            private:
                TokenType type;
                std::string value; //which token it is
            public:
                explicit Token(TokenType t , const std::string& v) : type(t) , value(v){}
            friend class Tokenizer;
        };
    private:
        const std::string& json;
        std::size_t pos = 0;
    public:
        explicit Tokenizer(const std::string&);
        std::vector<Token> tokenize();
        std::string readFile(const std::string&);
    private:
        void skipWhitespace();
        char next();
        char peek() const; //top 
        std::string parseString();
        std::string parseNumber();
};

#endif //TOKENIZER