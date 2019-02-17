%{
    #include <stdlib.h>
    #include "uthash.h"
    #include "vsopc.tab.h"

    struct hashtable {
        char key[10];
        yytokentype value;
        UT_hash_handle hh;
    };

    struct hashtable *keywords *tmp = NULL;

     tmp = (struct hashtable *)malloc(sizeof *tmp);
     strcpy(tmp->key, "while");
     tmp->id = WHILE;
     HASH_ADD_STR( keywords, "while", tmp);

%}

lowercase-letter    [a-z]
uppercase-letter    [A-Z]
letter              lowercase-letter | uppercase-letter

bin-digit           0 | 1
digit               bin-digit | [2-9]
hex-digit           digit | [a-fA-F]

tab                 "\x9"
lf                  "\x10"
ff                  "\x12"
cr                  "\x13"
whitespace          " " | tab | lf | ff | cr

%%




%%
int yywrap() {
    return 1;
}

int main(){
    yylex();
    return 0;
}