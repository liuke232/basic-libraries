#include "JsonParser.h"
#include "yyheader.h"
#include <iostream>

namespace jsonparser {

std::string text;
std::size_t errors = 0;
int64_t int_value = 0;
double float_value = 0.0;
JsonObject* root = nullptr;
FILE* fp_error = nullptr;


JsonObject* parse(const char* filename) {
    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Cannot find json file [" << filename << "]." << std::endl;
        return nullptr;
    }
    yyparse();
    return root;
}



}