#include "Tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& text):json(text){}

void Tokenizer::skipWhitespace()
{
    while(pos < json.size() && json[pos] == ' ') ++pos;
}

char Tokenizer::next()
{
    if(pos < json.size()) return json[pos++];
    throw std::runtime_error("Unexpected end of input");
}

char Tokenizer::peek() const 
{
    if (pos < json.size()) return json[pos];
    throw std::runtime_error("Unexpected end of input");
}

std::string Tokenizer::parseString() //parse a string literal in the JSON format
{
    std::string result;
    if(next() != '"') throw std::runtime_error("Expected '\"'");
    while(true)
    {
        char cur = next();
        if(cur == '"') break; //end of string
        if(cur == '\\') //escape char (maybe it is \n , \t , \\ , \")
        {
            char nxt = next();
            switch (nxt) 
            {
                case '\'': result += '\''; break;
                case '\"': result += '\"'; break;
                case '\\': result += '\\'; break;
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'v': result += '\v'; break;
                case 'a': result += '\a'; break;
                case '0': result += '\0'; break;
                default:
                    throw std::runtime_error("Invalid escape character");
            }
        }
        else result += cur;
    }
    return result;
}