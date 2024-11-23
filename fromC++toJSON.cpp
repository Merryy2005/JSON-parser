#include <iostream>
#include "JsonValue.hpp" 

int main() 
{
    auto json = std::make_shared<JsonObject>();
    json->add("name", std::make_shared<JsonString>("Alice"));
    json->add("age", std::make_shared<JsonNumber>(30));
    json->add("isStudent", std::make_shared<JsonBoolean>(false));
    auto address = std::make_shared<JsonObject>();
    address->add("city", std::make_shared<JsonString>("Wonderland"));
    address->add("postalCode", std::make_shared<JsonString>("12345"));
    json->add("address", address);
    json->print();
    return 0;
}
