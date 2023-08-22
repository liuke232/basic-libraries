#pragma once
#include <iostream>
#include "JsonObject.h"
#include "JsonParser.h"

#define YYSTYPE JsonObject*
using namespace jsonparser;
extern FILE* yyin;
extern int yylineno;

int yylex();
int yyparse();
int yyerror(const char* message);