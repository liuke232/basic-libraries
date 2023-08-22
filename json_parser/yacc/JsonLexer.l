%option nounput noyywrap yylineno

%{
    #include <cstdlib>
    #include <sstream>
    #include "JsonParser.tab.h"
    #include "yyheader.h"
%}

LINECOMMENT \/\/[^\n]*
EOL (\r\n|\r|\n)
SPACE [ \t]+
BINNUMBER 0[Bb][01]+
OCTNUMBER 0[0-7]+
DECNUMBER [+-]?[0-9]+
HEXNUMBER 0[Xx][A-Fa-f0-9]+
FLOATNUMBER [+-]?[0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?
STRING \"[^\"]*\"
IDENTIFIER [A-Za-z_][A-Za-z0-9_]*

%%

{LINECOMMENT} { /* do nothing */ }

{EOL} { /* do nothing */ }

{SPACE} { /* do nothing */ }

{IDENTIFIER} {
    int ret = 0;
    if (strcmp(yytext, "true") == 0)
        ret = LITERAL_TRUE;
    else if (strcmp(yytext, "false") == 0)
        ret = LITERAL_FALSE;
    else if (strcmp(yytext, "null") == 0)
        ret = LITERAL_NULL;
    else {
        std::stringstream ss;
        ss << "Unknown Word [" << yytext << "]"; 
        yyerror(ss.str().c_str());
    }
    text = yytext;
    return ret;
}

"[" { return LBRACKET; }
"]" { return RBRACKET; }
"{" { return LBRACE; }
"}" { return RBRACE; }
":" { return COLON; }
"," { return COMMA; }

{BINNUMBER} {
    text = yytext;
    int_value = std::strtol(yytext, nullptr, 2);
    return LITERAL_INTEGER; 
}
{OCTNUMBER} {
    text = yytext;
    int_value = std::strtol(yytext, nullptr, 8);
    return LITERAL_INTEGER; 
}
{DECNUMBER} {
    text = yytext;
    int_value = std::atol(yytext);
    return LITERAL_INTEGER; 
}
{HEXNUMBER} {
    text = yytext;
    int_value = std::strtol(yytext, nullptr, 16);
    return LITERAL_INTEGER; 
}

{STRING} {
    text = yytext;
    return LITERAL_STRING;
}

{FLOATNUMBER} {
    text = yytext;
    float_value = std::strtod(yytext, nullptr);
    return LITERAL_FLOAT;
}


. {
    std::stringstream ss;
    ss << "invalid character [" << yytext << "]";
    errors++;
    yyerror(ss.str().c_str());
}

%%
