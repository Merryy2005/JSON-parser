#include <iostream>
#include "Parser.hpp"

std::string readFile(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) throw std::runtime_error("Could not open file");
    std::string content;
    char cur;
    while (file.get(cur)) content += cur;
    return content;
}

int main() 
{
    try 
    {
        std::string jsonContent = readFile("Alice.json"); 
        Parser parser(jsonContent);
        std::shared_ptr<JsonValueBase> root = parser.parse();
        root->print();
        std::cout << std::endl;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}