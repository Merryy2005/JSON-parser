#include "Tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& text):json(text) , pos(0){}

void Tokenizer::skipWhitespace() { while (pos < json.size() && std::isspace(json[pos])) ++pos; }

char Tokenizer::next()
{
    if(pos < json.size()) { return json[pos++]; }
    throw std::runtime_error("Unexpected end of input");
}

char Tokenizer::peek() const 
{
    if (pos < json.size()) { return json[pos]; }
    throw std::runtime_error("Unexpected end of input");
}

std::string Tokenizer::parseString() //parse a string literal in the JSON format
{
    std::string result;
    if (next() != '"') throw std::runtime_error("Expected '\"'");
    const std::unordered_map<char, char> escapeChars = {
        {'\"', '\"'}, {'\\', '\\'}, {'n', '\n'}, {'t', '\t'}, {'r', '\r'},
        {'b', '\b'}, {'f', '\f'}, {'v', '\v'}, {'a', '\a'}, {'0', '\0'}
    };
    while (true)
    {
        char cur = next();
        if (cur == '"') break; // end of string
        if (cur == '\\')
        {
            char nxt = next();
            auto it = escapeChars.find(nxt);
            if (it != escapeChars.end()) { result += it->second; }
            else { throw std::runtime_error("Invalid escape character"); }
        }
        else { result += cur; }
    }
    return result;
}

std::string Tokenizer::parseNumber() 
{
    std::string result;
    bool decimal = false, exponent = false, exponentMinus = false;
    while (pos < json.size()) 
    {
        char cur = peek();
        if (std::isdigit(cur)) { result += next(); }
        else if (cur == '.') 
        {
            if (decimal || exponent) { throw std::runtime_error("Invalid number format (multiple decimal points or invalid character in exponent)"); }
            decimal = true;
            result += next();
        }
        else if (cur == 'e' || cur == 'E') 
        {
            if (exponent) { throw std::runtime_error("Invalid number format (multiple exponents)"); }
            exponent = true;
            result += next();
            if (peek() == '-') { result += next(); exponentMinus = true; }
        }
        else if (cur == '-' && (result.empty() || result.back() == 'e' || result.back() == 'E')) { result += next(); }
        else if (cur == '+' && (result.empty() || result.back() == 'e' || result.back() == 'E')) { result += next(); }
        else break;  // End the number parsing when encountering an invalid character
    }
    if (result.back() == 'e' || result.back() == 'E') { throw std::runtime_error("Invalid number format (incomplete exponent)"); }
    if (result.back() == '-') { throw std::runtime_error("Invalid number format (unfinished negative sign)"); }
    if (result.size() > 1 && result[0] == '0' && result[1] != '.' && result != "0") { throw std::runtime_error("Invalid number format (leading zeros not allowed)"); }
    if (result.size() > 1 && result[0] == '.' && std::isdigit(result[1])) { result = "0" + result; }
    if (result.size() > 2 && result[0] == '-' && result[1] == '.' && std::isdigit(result[2])) { result = "-0" + result.substr(1); } // Handle -0.123 case
    return result;
}

std::vector<Token> Tokenizer::tokenize() 
{
    std::vector<Token> tokens;
    while (pos < json.size()) 
    {
        skipWhitespace();
        char cur = peek();
        if (cur == '{') { tokens.push_back(Token(TokenType::CurlyOpen, std::string(1, next()))); } 
        else if (cur == '}') { tokens.push_back(Token(TokenType::CurlyClose, std::string(1, next()))); } 
        else if (cur == '[') { tokens.push_back(Token(TokenType::SquareOpen, std::string(1, next()))); } 
        else if (cur == ']') { tokens.push_back(Token(TokenType::SquareClose, std::string(1, next()))); } 
        else if (cur == ':') { tokens.push_back(Token(TokenType::Colon, std::string(1, next()))); } 
        else if (cur == ',') { tokens.push_back(Token(TokenType::Comma, std::string(1, next()))); } 
        else if (cur == '"') { tokens.push_back(Token(TokenType::String, parseString())); } 
        else if (std::isdigit(cur) || cur == '-' || cur == '.') { tokens.push_back(Token(TokenType::Number, parseNumber())); } 
        else if (json.compare(pos, 4, "true") == 0) { tokens.push_back(Token(TokenType::Boolean, "true")); pos += 4; } 
        else if (json.compare(pos, 5, "false") == 0) { tokens.push_back(Token(TokenType::Boolean, "false")); pos += 5; } 
        else if (json.compare(pos, 4, "null") == 0) { tokens.push_back(Token(TokenType::Null, "null")); pos += 4; } 
        else { throw std::runtime_error("Unexpected character"); }
    }
    return tokens;
}