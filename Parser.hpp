#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include "JsonValue.hpp"
#include "Tokenizer.hpp"

class Parser 
{
    private:
        Tokenizer tokenizer;
        std::vector<Token> tokens;
        size_t currentIndex;
    public:
        explicit Parser(const std::string& json);
        std::shared_ptr<JsonValueBase> parse();
    private:
        void parseValue(std::shared_ptr<JsonValueBase>& value);
        void parseObject(JsonObject& obj);
        void parseArray(JsonArray& arr);
        void parseString(JsonString& str);
        void parseNumber(JsonNumber& num);
        void parseBoolean(JsonBoolean& boolean);
        void parseNull(JsonNull& null);
        Token currentToken() const;
        Token nextToken();
        bool hasMoreTokens() const;
};

#endif // PARSER_HPP
