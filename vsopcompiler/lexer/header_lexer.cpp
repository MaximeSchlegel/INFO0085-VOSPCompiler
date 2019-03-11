#include "header_lexer.h"

void yycharnoRefresh() {
    yycharno += prevTokenLength;
    prevTokenLength = yyleng;
}

void yycharnoNewline() {
    yycharno = 1;
    prevTokenLength = 0;
}

int binToDigit (char c) {
    if ('0' <= c && c <= '1') { return c - '0'; }
    return -1;
}

int digitToDigit (char c) {
    if ('0' <= c && c <= '9') { return c - '0'; }
    return -1;
}

int hexToDigit (char c) {
    if ('0' <= c && c <= '9') { return c - '0'; }
    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
    return -1;
}

int debinify (std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = binToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 2, (double) s.size() - 1 - i);
    }
    return res;
}

int dedigify (std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = digitToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 10,(double) s.size() - 1 - i);
    }
    return res;
}

int dehexify(std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = hexToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 16, (double) s.size() - 1 - i);
    }
    return res;
}

int returnAndDisplayToken (int token, int lineno, int charno, bool display) {
    if (display) {
        std::cout << lineno << "," << charno << ",";
        switch (token) {
            case 259: std::cout << "end-of-file"; break;    //END token
            case 260: std::cout << "bool"; break;           //BOOL token
            case 261: std::cout << "int32"; break;          //INT32 token
            case 262: std::cout << "string"; break;         //STRING token
            case 263: std::cout << "unit"; break;           //UNIT token
            case 264: std::cout << "type-identifier," << *yylval.strValue; break;   //TYPEID token
            case 267: std::cout << "integer-literal," << yylval.intValue; break;    //INTLITERAL token
            case 268: std::cout << "string-literal," << *yylval.strValue; break;    //STRLITERAL token
            case 269: std::cout << "and"; break;            //AND token
            case 270: std::cout << "not"; break;            //NOT token
            case 271: std::cout << "class"; break;          //CLASS token
            case 272: std::cout << "do"; break;             //DO token
            case 273: std::cout << "else"; break;           //ELSE token
            case 274: std::cout << "extends"; break;        //EXTENDS token
            case 266: std::cout << "false"; break;          //FALSE token
            case 275: std::cout << "if"; break;             //IF token
            case 276: std::cout << "in"; break;             //IN token
            case 277: std::cout << "isnull"; break;         //ISNULL token
            case 278: std::cout << "let"; break;            //LET token
            case 279: std::cout << "new"; break;            //NEW token
            case 280: std::cout << "then"; break;           //THEN token
            case 265: std::cout << "true"; break;           //TRUE token
            case 281: std::cout << "while"; break;          //WHILE token
            case 282: std::cout << "object-identifier," << *yylval.strValue; break; //OBJECTID token
            case 283: std::cout << "lbrace"; break;         //LBRACE token
            case 284: std::cout << "rbrace"; break;         //RBRACE token
            case 285: std::cout << "lpar"; break;           //LPAR token
            case 286: std::cout << "rpar"; break;           //RPAR token
            case 287: std::cout << "colon"; break;          //COLON token
            case 288: std::cout << "semicolon"; break;      //SEMICOLON token
            case 289: std::cout << "comma"; break;          //COMMA token
            case 290: std::cout << "plus"; break;           //PLUS token
            case 291: std::cout << "minus"; break;          //MINUS token
            case 292: std::cout << "times"; break;          //TIMES token
            case 293: std::cout << "div"; break;            //DIV token
            case 294: std::cout << "pow"; break;            //POW token
            case 295: std::cout << "dot"; break;            //DOT token
            case 296: std::cout << "equal"; break;          //EQUAL token
            case 297: std::cout << "lower"; break;          //LOWER token
            case 298: std::cout << "lower-equal"; break;    //LOWER-EQUAL token
            case 299: std::cout << "assign"; break;         //ASSIGN token
        }
        std::cout << std::endl;
    }
    return token;
}

int returnAndDisplayError(int error, int lineno, int charno, bool display) {
    if (display) {
        std::cerr << filename << ":" << lineno << ":" << charno << ": lexical error : ";
        switch (error) {
            case 101: std::cerr << "unexpected end-of-file"; break;
            case 102: std::cerr << "invalid binary number"; break;
            case 103: std::cerr << "invalid decimal number"; break;
            case 104: std::cerr << "invalid hexadecimal number"; break;
            case 105: std::cerr << "missing whitespace between integer and identifier"; break;
            case 106: std::cerr << "invalid escaped sequence"; break;
            case 107: std::cerr << "invalid hexadecimal code"; break;
            case 108: std::cerr << "null char in string"; break;
            case 109: std::cerr << "feed line in string"; break;
            case 110: std::cerr << "invalid character"; break;
        }
        std::cerr << std::endl;
    }
    return -1;
}
