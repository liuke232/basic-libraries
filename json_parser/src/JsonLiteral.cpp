#include "JsonLiteral.h"
#include "JsonObject.h"

JsonLiteral::JsonLiteral(std::size_t lr, std::string msg, LiteralType lty) 
    : JsonObject(lr, LITERAL, std::move(msg)), literal_type(lty)
{
}

bool JsonLiteral::expectJsonLiteral(JsonLiteral** literal) {
    if (literal)
        *literal = dynamic_cast<JsonLiteral*>(this);
    return true;
}

bool JsonLiteral::expectString(std::string* str) {
    if (literal_type != String) 
        return false;
    if (str)
        *str = message;
    return true;
}

bool JsonLiteral::expectInteger(std::int64_t* val) {
    if (literal_type != Integer) 
        return false;
    if (val)
        *val = int_value;
    return true;
}

bool JsonLiteral::expectFloat(double* val) {
    if (literal_type != Float) 
        return false;
    if (val)
        *val = float_value;
    return true;
}

bool JsonLiteral::expectBoolean(bool* val) {
    if (literal_type != Bool) 
        return false;
    if (val)
        *val = bool_value;
    return true;
}

bool JsonLiteral::expectNull() {
    return literal_type == Null;
}
