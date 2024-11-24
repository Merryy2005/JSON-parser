#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& json):tokenizer(json), tokens(tokenizer.tokenize()), currentIndex(0) {}

std::shared_ptr<JsonValueBase> Parser::parse() 
{
    if (!hasMoreTokens()) 
    {
        throw std::runtime_error("No tokens to parse");
    }
    std::shared_ptr<JsonValueBase> value;
    parseValue(value);
    return value;
}

void Parser::parseValue(std::shared_ptr<JsonValueBase>& value) 
{
    const Token& token = currentToken();
    switch (token.type) 
    {
        case TokenType::CurlyOpen: 
        {
            auto obj = std::make_shared<JsonObject>();
            parseObject(*obj);
            value = obj;
            break;
        }
        case TokenType::SquareOpen: 
        {
            auto arr = std::make_shared<JsonArray>();
            parseArray(*arr);
            value = arr;
            break;
        }
        case TokenType::String: 
        {
            auto str = std::make_shared<JsonString>("");
            parseString(*str);
            value = str;
            break;
        }
        case TokenType::Number: 
        {
            auto num = std::make_shared<JsonNumber>(0);
            parseNumber(*num);
            value = num;
            break;
        }
        case TokenType::Boolean: 
        {
            auto boolean = std::make_shared<JsonBoolean>(false);
            parseBoolean(*boolean);
            value = boolean;
            break;
        }
        case TokenType::Null: 
        {
            auto nullValue = std::make_shared<JsonNull>();
            parseNull(*nullValue);
            value = nullValue;
            break;
        }
        default:
            throw std::runtime_error("Unexpected token type");
    }
}

void Parser::parseObject(JsonObject& obj) 
{
    nextToken(); // '{'
    while (currentToken().type != TokenType::CurlyClose) 
    {
        if (currentToken().type != TokenType::String) 
        {
            throw std::runtime_error("Expected a string key in object");
        }
        std::string key = currentToken().value;
        nextToken(); // Skip key
        if (currentToken().type != TokenType::Colon) 
        {
            throw std::runtime_error("Expected ':' after key in object");
        }
        nextToken(); // ':'
        std::shared_ptr<JsonValueBase> value;
        parseValue(value);
        obj.add(key, value);
        if (currentToken().type == TokenType::Comma) 
        {
            nextToken(); // ','
        }
    }
    nextToken(); // '}'
}

void Parser::parseArray(JsonArray& arr) 
{
    nextToken(); // '['
    while (currentToken().type != TokenType::SquareClose) 
    {
        std::shared_ptr<JsonValueBase> value;
        parseValue(value);
        arr.add(value);
        if (currentToken().type == TokenType::Comma) 
        {
            nextToken(); // ','
        }
    }
    nextToken(); // ']'
}

void Parser::parseString(JsonString& str) 
{
    str = JsonString(currentToken().value);
    nextToken();
}

void Parser::parseNumber(JsonNumber& num) 
{
    num = JsonNumber(std::stod(currentToken().value));
    nextToken();
}

void Parser::parseBoolean(JsonBoolean& boolean) 
{
    boolean = JsonBoolean(currentToken().value == "true");
    nextToken();
}

void Parser::parseNull(JsonNull& null) 
{
    nextToken();
}

Token Parser::currentToken() const 
{
    if (!hasMoreTokens()) 
    {
        throw std::runtime_error("No more tokens available");
    }
    return tokens[currentIndex];
}

Token Parser::nextToken() 
{
    if (!hasMoreTokens()) 
    {
        throw std::runtime_error("No more tokens to advance");
    }
    return tokens[currentIndex++];
}

bool Parser::hasMoreTokens() const 
{
    return currentIndex < tokens.size();
}
