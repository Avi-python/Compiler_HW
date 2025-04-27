%option c++ noyywrap
%option yyclass="FooLexer"

%{
    #include "FooLexer.hh"
    #include "fooParser.tab.hh"
    
    #undef  YY_DECL
    #define YY_DECL int FooLexer::yylex(std::string *const yylval)
%}

%%

[[:space:]] ;
"+" { return yy::parser::token::ADD; }
"-" { return yy::parser::token::SUB; }
"*" { return yy::parser::token::MUL; }
"/" { return yy::parser::token::DIV; }
"=" { return yy::parser::token::ASSIGN; }
"(" { return yy::parser::token::L_PAREN; }
")" { return yy::parser::token::R_PAREN; }
[[:alpha:]]+ { *yylval = std::string(yytext, yytext + yyleng); return yy::parser::token::IDENTIFIER; }
[[:digit:]]+ { *yylval = std::string(yytext, yytext + yyleng); return yy::parser::token::INTEGER; }
. { return yytext[0]; }

%%