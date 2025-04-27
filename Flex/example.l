%{
#include <stdio.h>
%}

%%

"Hello World" printf("Oh Yes\n");

%%

int main() {
    yylex();
    return 0;
}