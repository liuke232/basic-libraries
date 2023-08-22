#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include "JsonObject.h"

class JsonLiteral : public JsonObject {
public:
    enum LiteralType {
        String, Integer, Float, Bool, Null
    };
    union {
        std::int64_t int_value;
        double float_value;
        bool bool_value;
    };
    LiteralType literal_type;

    JsonLiteral(std::size_t lr, std::string msg, LiteralType lty);
    ~JsonLiteral() { }
    
    bool expectJsonLiteral(JsonLiteral** literal = nullptr) override;
    bool expectString(std::string* str = nullptr) override;
    bool expectInteger(std::int64_t* val = nullptr) override;
    bool expectFloat(double* val = nullptr) override;
    bool expectBoolean(bool* val = nullptr) override;
    bool expectNull() override;
};
