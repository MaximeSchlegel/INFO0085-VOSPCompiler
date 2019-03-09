#include "lex.yy.cc"

//TODO: make that -lex without a file name, work correctly (use the sdin from the console)
//TODO: make that the -lex display the symbole recevied
//TODO: add the -parse option with and without filename

int main(int argc, char *argv[]) {
    std::string param(argv[1]);

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

    if(argc == 3 && param.compare("-lex") == 0)
    {
        filename = std::string(argv[2]);
        yyin = fopen(argv[2], "r");
        int token = yylex();
        while (token != END and token != -1) {
            token = yylex();
        }
        fclose(yyin);
        if (token ==-1) {
            return -1;
        }
        return 0;
    }

    return -1;
}
