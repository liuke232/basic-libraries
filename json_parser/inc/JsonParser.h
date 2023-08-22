#pragma once
#include <cstddef>
#include "JsonObject.h"
#include "JsonLiteral.h"

namespace jsonparser {
    /**
     * 解析Json格式的文本
     * \param filename 待解析的文件名
     * \return JsonObject对象
     */
    JsonObject* parse(const char* filename);

    extern std::string text;
    extern std::size_t errors;
    extern int64_t int_value;
    extern double float_value;
    extern JsonObject* root;
    extern FILE* fp_error;
};