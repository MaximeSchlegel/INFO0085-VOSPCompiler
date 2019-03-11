#include "lex.yy.c"

YYLTYPE yylloc;
YYSTYPE yylval;

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

    if(argc == 3 && std::string(argv[1]).compare("-lex") == 0)
    {
        filename = std::string(argv[2]);
        yyin = fopen(argv[2], "r");
        int token = yylex();
        while (token != END and token != -1) {
            token = yylex();
            std::cout << yylloc.first_line << ", " << yylloc.first_column << std::endl;
        }
        fclose(yyin);
        if (token ==-1) {
            return -1;
        }
        return 0;
    }
    else if(argc == 2)
    {
        filename = std::string(argv[1]);
        yyin = fopen(argv[2], "r");
        int token = yylex();
        // Lexer and parser main here

        return 0;
    }

    return -1;
}