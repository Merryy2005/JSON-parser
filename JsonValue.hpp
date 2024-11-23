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
        virtual std::string getType() const = 0;
        virtual void print() const = 0;
};

class JsonNull : public JsonValueBase 
{
    public:
        std::string getType() const override { return "Null"; }
        void print() const override { std::cout << "null"; }
};

class JsonBoolean : public JsonValueBase 
{
    private:
        bool value;
    public:
        explicit JsonBoolean(bool val) : value(val) {}
        bool getValue() const { return value; }
        std::string getType() const override { return "Boolean"; }
        void print() const override { std::cout << (value ? "true" : "false"); }
};

class JsonNumber : public JsonValueBase 
{
    private:
        double value;
    public:
        explicit JsonNumber(double val) : value(val) {}
        double getValue() const { return value; }
        std::string getType() const override { return "Number"; }
        void print() const override { std::cout << value; }
};

class JsonString : public JsonValueBase 
{
    private:
        std::string value;
    public:
        explicit JsonString(const std::string& val) : value(val) {}
        const std::string& getValue() const { return value; }
        std::string getType() const override { return "String"; }
        void print() const override { std::cout << "\"" << value << "\""; }
};

class JsonArray : public JsonValueBase 
{
    private:
        std::vector<std::shared_ptr<JsonValueBase>> elements;
    public:
        void add(const std::shared_ptr<JsonValueBase>& element) { elements.push_back(element); }
        const std::vector<std::shared_ptr<JsonValueBase>>& getElements() const { return elements; }
        std::string getType() const override { return "Array"; }
        void print() const override 
        {
            std::cout << "[";
            for (size_t i = 0; i < elements.size(); ++i) 
            {
                elements[i]->print();
                if (i < elements.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        }
};

class JsonObject : public JsonValueBase 
{
    private:
        std::map<std::string, std::shared_ptr<JsonValueBase>> members;
    public:
        void add(const std::string& key, const std::shared_ptr<JsonValueBase>& value) { members[key] = value; }
        const std::map<std::string, std::shared_ptr<JsonValueBase>>& getMembers() const { return members; }
        std::string getType() const override { return "Object"; }
        void print() const override 
        {
            std::cout << "{";
            for(auto it = members.begin(); it != members.end(); ++it) 
            {
                std::cout << "\"" << it->first << "\": ";
                it->second->print();
                if (it != std::prev(members.end())) std::cout << ", ";
            }
            std::cout << "}";
        }
};

#endif //JSONVALUE