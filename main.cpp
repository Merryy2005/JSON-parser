#include <iostream>
#include "JsonValue.hpp"
#include "Tokenizer.hpp"

std::string readFile(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open file");
    }
    std::string content;
    char cur;
    while (file.get(cur)) 
    {
        content += cur;
    }
    return content;
}

int main() 
{
    try 
    {
        std::string jsonContent = readFile("Alice.json"); 
        Tokenizer tokenizer(jsonContent);
        std::vector<Token> tokens = tokenizer.tokenize();
        for (const auto& token : tokens) 
        {
            std::cout << "Token Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}