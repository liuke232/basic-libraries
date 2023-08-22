#include "JsonObject.h"
#include <unordered_map>

JsonObject::JsonObject(std::size_t lr, Type ty, std::string msg)
    : linenr(lr), type(ty), message(std::move(msg)) { }

JsonObject::~JsonObject() { }

bool JsonObject::expectJsonDict(JsonDict** dict) {
    (void)dict;
    return false;
}

bool JsonObject::expectJsonList(JsonList** list) {
    (void)list;
    return false;
}

bool JsonObject::expectJsonLiteral(JsonLiteral** literal) {
    (void)literal;
    return false;
}

bool JsonObject::expectString(std::string* str) {
    (void)str;
    return false;
}

bool JsonObject::expectInteger(std::int64_t* val) {
    (void)val;
    return false;
}

bool JsonObject::expectFloat(double* val) {
    (void)val;
    return false;
}

bool JsonObject::expectBoolean(bool* val) {
    (void)val;
    return false;
}

bool JsonObject::expectNull() {
    return false;
}

JsonList::JsonList(std::size_t lr, std::string msg)
    : JsonObject(lr, LIST, std::move(msg)) { }

JsonList::~JsonList() {
    for (JsonObject* o : list) {
        delete o;
    }
}

void JsonList::push(JsonObject* obj) {
    list.push_back(obj);
}

std::vector<JsonObject*>& JsonList::items() {
    return list;
}

bool JsonList::expectJsonList(JsonList** list) {
    if (list)
        *list = dynamic_cast<JsonList*>(this);
    return true;
}

JsonDict::JsonDict(std::size_t lr, std::string msg)
    : JsonObject(lr, DICT, std::move(msg)) { }

JsonDict::~JsonDict() {
    for (auto& o : map) {
        delete o.second;
    }
}

void JsonDict::put(const std::string& key, JsonObject* obj) {
    map[key] = obj;
}
    
bool JsonDict::contains(const std::string& key) {
    return map.find(key) != map.end();
}
    
JsonObject* JsonDict::get(const std::string& key) {
    return contains(key) ? map[key] : nullptr;
}

std::unordered_map<std::string, JsonObject*>& JsonDict::items() {
    return map;
}

bool JsonDict::expectJsonDict(JsonDict** dict) {
    if (dict)
        *dict = dynamic_cast<JsonDict*>(this);
    return true;
}
