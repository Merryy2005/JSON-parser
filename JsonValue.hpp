#ifndef JSONVALUE
#define JSONVALUE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

class JsonValueBase 
{
    public:
        JsonValueBase() = default;
        virtual ~JsonValueBase() = default;
        virtual void print(int level = 0) const = 0;
};

class JsonNull : public JsonValueBase 
{
    public:
        void print(int level = 0) const override { std::cout << "null"; }
};

class JsonBoolean : public JsonValueBase 
{
    private:
        bool value;
    public:
        explicit JsonBoolean(bool val) : value(val) {}
        void print(int level = 0) const override { std::cout << (value ? "true" : "false"); }
};

class JsonNumber : public JsonValueBase 
{
    private:
        double value;
    public:
        explicit JsonNumber(double val) : value(val) {}
        void print(int level = 0) const override { std::cout << value; }
};

class JsonString : public JsonValueBase 
{
    private:
        std::string value;
    public:
        explicit JsonString(const std::string& val) : value(val) {}
        void print(int level = 0) const override { std::cout << "\"" << value << "\""; }
};

class JsonArray : public JsonValueBase 
{
    private:
        std::vector<std::shared_ptr<JsonValueBase>> elements;
    public:
        void add(const std::shared_ptr<JsonValueBase>& element) { elements.push_back(element); }
        void print(int level = 0) const override 
        {
            std::cout << "[\n";
            for (size_t i = 0; i < elements.size(); ++i) 
            {
                std::cout << std::string(level + 2, ' ');
                elements[i]->print(level + 2);
                if (i < elements.size() - 1) std::cout << ",";
                std::cout << "\n";
            }
            std::cout << std::string(level, ' ');
            std::cout << "]";
        }
};

class JsonObject : public JsonValueBase 
{
    private:
        std::vector<std::pair<std::string, std::shared_ptr<JsonValueBase>>> members;
    public:
        void add(const std::string& key, const std::shared_ptr<JsonValueBase>& value) {members.emplace_back(key, value);}
        void print(int level = 0) const override 
        {
            std::cout << "{\n";
            for(auto it = members.begin(); it != members.end(); ++it) 
            {
                std::cout << std::string(level + 2, ' ') << "\"" << it->first << "\": ";
                it->second->print(level + 2);
                if (it != std::prev(members.end())) std::cout << ",";
                std::cout << "\n";
            }
            std::cout << std::string(level, ' ');
            std::cout << "}";
        }
};

#endif //JSONVALUE