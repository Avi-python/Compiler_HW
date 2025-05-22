%define parse.error detailed
%locations

%{
#include <stdio.h>
#include <stdlib.h>


int yylex(void);
void yyerror(const char *s);
extern char* yytext; // Make yytext available for error reporting
%}

%union {
    int ival;
    char *sval;
}

// Keywords
%token VOID MAIN INT IF WHILE ERROR

// Literals and Identifiers
%token <sval> IDENTIFIER
%token <ival> NUMBER

// Operators
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE

// Relational Operators
%token LT LE EQ NE GT GE

// Punctuation
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMICOLON

// Define precedence and associativity if using ambiguous grammar,
// but the EBNF to BNF translation here aims for unambiguous rules.

%start Program

%%

Program:
    VOID MAIN LPAREN RPAREN CompoundStatement { printf("Parsing successful: Program recognized.\n"); }
    ;

CompoundStatement:
    LBRACE OptionalDeclaration StatementList RBRACE
    ;

OptionalDeclaration:
    /* empty */
    | Declaration
    ;

Declaration:
    INT IdentList SEMICOLON 
    | error IdentList SEMICOLON { 
        yyerror("Invalid declaration"); 
        yyerrok; 
    }
    ;

IdentList:
    IDENTIFIER
    | IdentList COMMA IDENTIFIER
    ;

StatementList:
    /* empty */
    | StatementList Statement
    ;

// <Statement> ::= [<AssignmentStatement>|<CompoundStatement>|<IfStatement>|<WhileStatement>]
// This means Statement can be one of the choices, or it can be an empty construct (epsilon).
Statement:
    AssignmentStatement
    | CompoundStatement
    | IfStatement
    | WhileStatement
    | /* empty */   // Represents the optional nature of the entire statement block
    ;

AssignmentStatement:
    IDENTIFIER ASSIGN Expression SEMICOLON
    | IDENTIFIER ASSIGN Expression error { 
        yyerrok; 
    }
    | IDENTIFIER ASSIGN error SEMICOLON { 
        yyerror("Invalid expression in assignment");
        yyerrok; 
    }
    ;

IfStatement:
    IF LPAREN Condition RPAREN Statement
    | IF LPAREN Condition error Statement {
        yyerror("Invalid if statement");
        yyerrok;
    }
    ;

WhileStatement:
    WHILE LPAREN Condition RPAREN Statement
    ;

Condition:
    Expression RelOp Expression
    ;

RelOp:
    LT | LE | EQ | NE | GT | GE
    ;

// <Expression> ::= [+|-]<Term>{\+|-\<Term>}
// Translated as: InitialTerm AddTermList
// InitialTerm: Term | PLUS Term | MINUS Term
// AddTermList: /* empty */ | AddTermList AddOp Term
Expression:
    InitialTerm AddTermList
    ;

InitialTerm:
    Term
    | PLUS Term
    | MINUS Term
    ;

AddTermList:
    /* empty */
    | AddTermList AddOp Term
    ;

AddOp:
    PLUS 
    | MINUS
    ;

// <Term> ::= <Factor>{\*|/\ <Factor>}
// Translated as: InitialFactor MulFactorList
// InitialFactor: Factor
// MulFactorList: /* empty */ | MulFactorList MulOp Factor
Term:
    Factor MulFactorList // Factor is InitialFactor as no unary op for Factor in EBNF
    ;

MulFactorList:
    /* empty */
    | MulFactorList MulOp Factor
    ;

MulOp:
    MULTIPLY
    | DIVIDE
    ;

Factor:
    IDENTIFIER
    | NUMBER
    | LPAREN Expression RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %d:%d (Near token: '%s'): %s\n", 
            yylloc.first_line, yylloc.first_column, yytext, s);
}

int main(int argc, char **argv) 
{
    return yyparse();
}
