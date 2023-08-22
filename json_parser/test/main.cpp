#include "JsonObject.h"
#include "JsonParser.h"
#include <iostream>

int main() {
    auto x = jsonparser::parse("test/111.json");
    if (x)
        std::cout << "Root is " << x->type << std::endl;
    else
        std::cout << "x is null" << std::endl;
    std::string s;
    return 0;
}