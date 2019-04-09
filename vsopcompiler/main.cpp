#include "vsopc.tab.c"
#include "lex.yy.c"

#include "utils/ASTNode.h"

//extern "C" int yyparse();
//extern "C" int yyerror();
//
//extern YYLTYPE yylloc;
//extern YYSTYPE yylval;
extern ASTNode * astResult;

int DISPLAY;

int main(int argc, char *argv[]) {
    reservedId.emplace("bool", BOOL);
    reservedId.emplace("int32", INT32);
    reservedId.emplace("string", STRING);
    reservedId.emplace("unit", UNIT);
    reservedId.emplace("and", AND);
    reservedId.emplace("not", NOT);
    reservedId.emplace("class", CLASS);
    reservedId.emplace("do", DO);
    reservedId.emplace("else", ELSE);
    reservedId.emplace("extends", EXTENDS);
    reservedId.emplace("false", FALSE);
    reservedId.emplace("if", IF);
    reservedId.emplace("in", IN);
    reservedId.emplace("isnull", ISNULL);
    reservedId.emplace("let", LET);
    reservedId.emplace("new", NEW);
    reservedId.emplace("then", THEN);
    reservedId.emplace("true", TRUE);
    reservedId.emplace("while", WHILE);

    yylloc.first_column = 1;
    yylloc.first_line =1;
    yylloc.last_column = 1;
    yylloc.last_line = 1;

//    yydebug = 1;

    if(argc == 2 and std::string(argv[1]).compare("-lex") == 0)
    {
        DISPLAY = true;
        int token = yylex();
        while (token != END and token != ERROR) {
            token = yylex();
        }
        return 0;
    }

    if (argc == 2 and std::string(argv[1]).compare("-parse") == 0) {
        DISPLAY = false;
        int r = yyparse();
        std::cout << r << std::endl << *astResult;
        return r;
    }

    if(argc == 3 && std::string(argv[1]).compare("-lex") == 0) {
        DISPLAY = true;
        yyin = fopen(argv[2], "r");
        filename = argv[2];

        int token;
        do {
            token = yylex();
        } while (token != END and token != ERROR);
        fclose(yyin);
        if (token == ERROR) {
            return -1;
        }
        return 0;
    }

    if(argc == 3 && std::string(argv[1]).compare("-parse") == 0) {
        DISPLAY = false;
        yyin = fopen(argv[2], "r");
        filename = argv[2];

        if (yyparse() == 1) {
            std::cerr << filename << ":" << yylloc.first_line << ":" << yylloc.first_column << ": syntax error";
            fclose(yyin);
            return -1;
        } else {
            std::cout << *astResult;
            fclose(yyin);
            return 0;
        }
    }
    return -1;
}