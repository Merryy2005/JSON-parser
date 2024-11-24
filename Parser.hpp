#ifndef PARSER_HPP
#define PARSER_HPP

#include "Tokenizer.hpp"
#include "JsonValue.hpp"
#include <memory>

class Parser
{
    private:
        Tokenizer tokenizer;
        std::vector<Token> tokens;
        std::size_t currentIndex;
    public:
        explicit Parser(const std::string& json);
        std::shared_ptr<JsonValueBase> parse();
    private:
        std::shared_ptr<JsonValueBase> parseValue(); 
        std::shared_ptr<JsonObject> parseObject(); 
        std::shared_ptr<JsonArray> parseArray(); 
        std::shared_ptr<JsonString> parseString(); 
        std::shared_ptr<JsonNumber> parseNumber(); 
        std::shared_ptr<JsonBoolean> parseBoolean(); 
        std::shared_ptr<JsonNull> parseNull(); 
    private:
        Token currentToken() const;
        Token nextToken();
        bool hasMoreTokens() const;
};

#endif //PARSER_HPP