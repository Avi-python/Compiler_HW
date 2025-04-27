%require "3.2"
%language "c++"

%code requires {
    #include <string>
    #include <iostream>
    #include "FooLexer.hh"
}

%define api.value.type {std::string}

%parse-param {FooLexer &lexer}

%header

%code {
    #define yylex lexer.yylex
}

%token INTEGER
%token IDENTIFIER
%token ASSIGN
%token L_PAREN
%token R_PAREN
%token ADD
%token SUB
%token MUL
%token DIV

%%

assign
    : IDENTIFIER ASSIGN expression { std::cout << $1 << " = "; }
    ;

expression
    : expression ADD term { std::cout << " + "; }
    | expression SUB term { std::cout << " - "; }
    | term
    ;

term
    : term MUL factor { std::cout << " * "; }
    | term DIV factor { std::cout << " / "; }
    | factor
    ;

factor
    : number
    | IDENTIFIER { std::cout << $1; }
    | L_PAREN expression R_PAREN
    ;

number
    : INTEGER { std::cout << $1; }
    ;

%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}