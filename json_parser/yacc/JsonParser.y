%{
    #include "JsonParser.h"
    #include "JsonLiteral.h"
    #include "yyheader.h"
%}

%token LITERAL_STRING LITERAL_TRUE LITERAL_FALSE LITERAL_NULL LITERAL_INTEGER LITERAL_FLOAT
%token COLON COMMA
%token LBRACKET RBRACKET LBRACE RBRACE

%start JsonFile

%%

// JsonFile，根节点
JsonFile
: JsonListOrDict { root = $1; }

// Json数组或字典
JsonListOrDict
: JsonList { $$ = $1; }
| JsonDict { $$ = $1; }

// Json对象
JsonObject
: JsonListOrDict { $$ = $1; }
| Literal { $$ = $1; }

// 列表
JsonList 
: LBRACKET JsonObjectSequence RBRACKET {
    $$ = $2;
}

// 对象序列
JsonObjectSequence 
: JsonObject {
    JsonList* tmp = new JsonList(yylineno, "JsonList");
    tmp->push($1);
    $$ = tmp;
}
| JsonObjectSequence COMMA JsonObject {
    JsonList* tmp = dynamic_cast<JsonList*>($1);
    tmp->push($3);
    $$ = tmp;
}

// 字面量
Literal
: Number { $$ = $1; }
| String { $$ = $1; }
| Bool { $$ = $1; }
| Null { $$ = $1; }

// 数字
Number
: Integer { $$ = $1; }
| Float { $$ = $1; }

// 字符串
String 
: LITERAL_STRING {
    $$ = new JsonLiteral(yylineno, text.substr(1, text.size() - 2), JsonLiteral::String); 
}

// 布尔值
Bool
: LITERAL_TRUE {
    JsonLiteral* tmp = new JsonLiteral(yylineno, "true", JsonLiteral::Bool); 
    tmp->bool_value = true;
    $$ = tmp;
}
| LITERAL_FALSE {
    JsonLiteral* tmp = new JsonLiteral(yylineno, "false", JsonLiteral::Bool); 
    tmp->bool_value = false;
    $$ = tmp;
}

// 空值
Null
: LITERAL_NULL {
    $$ = new JsonLiteral(yylineno, "null", JsonLiteral::Null);
}

// 整数
Integer
: LITERAL_INTEGER {
    JsonLiteral* tmp = new JsonLiteral(yylineno, text, JsonLiteral::Integer); 
    tmp->int_value = int_value;
    $$ = tmp;
}

// 浮点数
Float
: LITERAL_FLOAT {
    JsonLiteral* tmp = new JsonLiteral(yylineno, text, JsonLiteral::Float); 
    tmp->float_value = float_value;
    $$ = tmp;
}

// 字典
JsonDict
: LBRACE KeyValueSequence RBRACE { $$ = $2; }

// 键值对序列
KeyValueSequence
: String COLON JsonObject {
    JsonDict* tmp = new JsonDict(yylineno, "JsonDict");
    tmp->put(dynamic_cast<JsonLiteral*>($1)->message, $3);
    delete $1;
    $$ = tmp;
}
| KeyValueSequence COMMA String COLON JsonObject {
    JsonDict* tmp = dynamic_cast<JsonDict*>($1);
    tmp->put(dynamic_cast<JsonLiteral*>($3)->message, $5);
    delete $3;
    $$ = tmp;
}


%%

int yyerror(const char* message)
{
	fprintf(fp_error, "error: %s, at line %d\n", message, yylineno);
	return -1;
}