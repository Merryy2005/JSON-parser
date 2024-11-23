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

std::string Tokenizer::parseNumber() 
{
    std::string result;
    bool decimal = false;
    bool exponent = false;
    bool exponentMinus = false;
    while (pos < json.size()) 
    {
        char cur = peek();
        if (std::isdigit(cur)) 
        {
            result += next();
        }
        else if (cur == '.') 
        {
            if (decimal || exponent) 
            {
                throw std::runtime_error("Invalid number format (multiple decimal points or invalid character in exponent)");
            }
            decimal = true;
            result += next();
        }
        else if (cur == 'e' || cur == 'E') 
        {
            if (exponent) 
            {
                throw std::runtime_error("Invalid number format (multiple exponents)");
            }
            exponent = true;
            result += next();
            if (peek() == '-') 
            {
                result += next();
                exponentMinus = true;
            }
        }
        else if (cur == '-' && (result.empty() || result.back() == 'e' || result.back() == 'E')) 
        {
            result += next();
        }
        else if (cur == '+' && (result.empty() || result.back() == 'e' || result.back() == 'E')) 
        {
            result += next();
        }
        else {
            break;  // End the number parsing when encountering an invalid character
        }
    }
    if (result.back() == 'e' || result.back() == 'E') 
    {
        throw std::runtime_error("Invalid number format (incomplete exponent)");
    }
    if (result.back() == '-') 
    {
        throw std::runtime_error("Invalid number format (unfinished negative sign)");
    }
    if(result.size() && result[0] == '0' && !decimal && result != "0") //0123 - invalid
    {
        throw std::runtime_error("Invalid number format (leading zeros not allowed)");
    }
    if(result.size() > 2 && result[0] == '-' && result[1] == '0' && !decimal) //-0123 - invalid
    {
        throw std::runtime_error("Invalid number format (leading zeros not allowed)");
    }
    if(result.size() > 1 && result[0] == '0' && result[1] == '0') //00.123
    {
        throw std::runtime_error("Invalid number format (leading zeros not allowed)");
    }
    if(result.size() > 2 && result[0] == '-' && result[1] == '0' && result[2] == '0') // -00.123
    {
        throw std::runtime_error("Invalid number format (leading zeros not allowed)");
    }
    if (result.size() > 1 && result[0] == '.' && std::isdigit(result[1]))  // .123 -> 0.123
    {
        result = "0" + result; 
    }
    if(result.size() > 2 && result[0] == '-' && result[1] == '.' && std::isdigit(result[2])) // -.123 -> -0.123
    {
        result = "-0" + result.substr(1); 
    }
    return result;
}
