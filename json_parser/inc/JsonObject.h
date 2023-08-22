#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

class JsonList;
class JsonDict;
class JsonLiteral;

class JsonObject {
public:
    enum Type {
        DICT, LIST, LITERAL
    };
    std::size_t linenr;
    Type type;
    std::string message;

    JsonObject(std::size_t lr, Type ty, std::string msg);
    virtual ~JsonObject() = 0;

    virtual bool expectJsonDict(JsonDict** dict = nullptr);
    virtual bool expectJsonList(JsonList** list = nullptr);
    virtual bool expectJsonLiteral(JsonLiteral** literal = nullptr);
    virtual bool expectString(std::string* str = nullptr);
    virtual bool expectInteger(std::int64_t* val = nullptr);
    virtual bool expectFloat(double* val = nullptr);
    virtual bool expectBoolean(bool* val = nullptr);
    virtual bool expectNull();
};

class JsonList : public JsonObject {
protected:
    std::vector<JsonObject*> list;

public:
    JsonList(std::size_t lr, std::string msg);
    ~JsonList() override;

    void push(JsonObject* obj);
    std::vector<JsonObject*>& items();
    
    bool expectJsonList(JsonList** list = nullptr) override;
};

class JsonDict : public JsonObject {
protected:
    std::unordered_map<std::string, JsonObject*> map;

public:
    JsonDict(std::size_t lr, std::string msg);
    ~JsonDict() override;

    void put(const std::string& key, JsonObject* obj);
    bool contains(const std::string& key);
    JsonObject* get(const std::string& key);
    std::unordered_map<std::string, JsonObject*>& items();

    bool expectJsonDict(JsonDict** dict = nullptr) override;
};