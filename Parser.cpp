#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& json) : tokenizer(json), tokens(tokenizer.tokenize()), currentIndex(0) {}

std::shared_ptr<JsonValueBase> Parser::parse() 
{
    if (!hasMoreTokens()) { throw std::runtime_error("No tokens to parse"); }
    return parseValue(); 
}

std::shared_ptr<JsonValueBase> Parser::parseValue() 
{
    const Token& token = currentToken();
    switch (token.type) 
    {
        case TokenType::CurlyOpen: return parseObject(); 
        case TokenType::SquareOpen: return parseArray();
        case TokenType::String: return parseString();
        case TokenType::Number: return parseNumber();
        case TokenType::Boolean: return parseBoolean(); 
        case TokenType::Null: return parseNull();
        default: throw std::runtime_error("Unexpected token type");
    }
}

std::shared_ptr<JsonObject> Parser::parseObject() 
{
    nextToken(); // '{'
    auto obj = std::make_shared<JsonObject>();
    while (currentToken().type != TokenType::CurlyClose) 
    {
        if (currentToken().type != TokenType::String) { throw std::runtime_error("Expected a string key in object"); }
        std::string key = currentToken().value;
        nextToken(); // Skip key
        if (currentToken().type != TokenType::Colon) { throw std::runtime_error("Expected ':' after key in object"); }
        nextToken(); // ':'
        std::shared_ptr<JsonValueBase> value = parseValue(); // Directly parse value
        obj->add(key, value); // Add the parsed value to the object
        if (currentToken().type == TokenType::Comma) { nextToken(); } // ',' (skip)
    }
    nextToken(); // '}'
    return obj;
}

std::shared_ptr<JsonArray> Parser::parseArray() 
{
    nextToken(); // '['
    auto arr = std::make_shared<JsonArray>();
    while (currentToken().type != TokenType::SquareClose) 
    {
        std::shared_ptr<JsonValueBase> value = parseValue(); 
        arr->add(value); 
        if (currentToken().type == TokenType::Comma) { nextToken(); } // ',' (skip)
    }
    nextToken(); // ']'
    return arr;
}

std::shared_ptr<JsonString> Parser::parseString() 
{
    std::shared_ptr<JsonString> str = std::make_shared<JsonString>(currentToken().value);
    nextToken();
    return str;
}

std::shared_ptr<JsonNumber> Parser::parseNumber() 
{
    std::shared_ptr<JsonNumber> num = std::make_shared<JsonNumber>(std::stod(currentToken().value));
    nextToken();
    return num;
}

std::shared_ptr<JsonBoolean> Parser::parseBoolean() 
{
    std::shared_ptr<JsonBoolean> boolean = std::make_shared<JsonBoolean>(currentToken().value == "true");
    nextToken();
    return boolean;
}

std::shared_ptr<JsonNull> Parser::parseNull() 
{
    nextToken();
    return std::make_shared<JsonNull>();
}

Token Parser::currentToken() const 
{
    if (!hasMoreTokens()) { throw std::runtime_error("No more tokens available"); }
    return tokens[currentIndex];
}

Token Parser::nextToken() 
{
    if (!hasMoreTokens()) { throw std::runtime_error("No more tokens to advance"); }
    return tokens[currentIndex++];
}

bool Parser::hasMoreTokens() const { return currentIndex < tokens.size(); }