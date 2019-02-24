#include "utils.h"

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
            case END: std::cout << "end-of-file"; break;
            case BOOL: std::cout << "bool"; break;
            case INT32: std::cout << "int32"; break;
            case STRING: std::cout << "string"; break;
            case UNIT: std::cout << "unit"; break;
            case TYPEID: std::cout << "type-identifier," << *yylval.strValue; break;
            case INTLITERAL: std::cout << "integer-literal," << yylval.intValue; break;
            case STRLITERAL: std::cout << "string-literal," << *yylval.strValue; break;
            case AND: std::cout << "and"; break;
            case NOT: std::cout << "not"; break;
            case CLASS: std::cout << "class"; break;
            case DO: std::cout << "do"; break;
            case ELSE: std::cout << "else"; break;
            case EXTENDS: std::cout << "extends"; break;
            case FALSE: std::cout << "false"; break;
            case IF: std::cout << "if"; break;
            case IN: std::cout << "in"; break;
            case ISNULL: std::cout << "isnull"; break;
            case LET: std::cout << "let"; break;
            case NEW: std::cout << "new"; break;
            case THEN: std::cout << "then"; break;
            case TRUE: std::cout << "true"; break;
            case WHILE: std::cout << "while"; break;
            case OBJECTID: std::cout << "object-identifier," << *yylval.strValue; break;
            case LBRACE: std::cout << "lbrace"; break;
            case RBRACE: std::cout << "rbrace"; break;
            case LPAR: std::cout << "lpar"; break;
            case RPAR: std::cout << "rpar"; break;
            case COLON: std::cout << "colon"; break;
            case SEMICOLON: std::cout << "semicolon"; break;
            case COMMA: std::cout << "comma"; break;
            case PLUS: std::cout << "plus"; break;
            case MINUS: std::cout << "minus"; break;
            case TIMES: std::cout << "times"; break;
            case DIV: std::cout << "div"; break;
            case POW: std::cout << "pow"; break;
            case DOT: std::cout << "dot"; break;
            case EQUAL: std::cout << "equal"; break;
            case LOWER: std::cout << "lower"; break;
            case LOWEREQUAL: std::cout << "lower-equal"; break;
            case ASSIGN: std::cout << "assign"; break;
            case UNKNOWN: std::cout << "Mystery Char," << yylval.strValue; break;
        }
        std::cout << std::endl;
    }
    return token;
}

int returnAndDisplayError(int error, int lineno, int charno, bool display) {
    if (display) {
        std::cerr << lineno << ":" << charno << ": lexical error : ";
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
        }
        std::cerr << std::endl;
    }
    return -1;
}

void displayTokens(std::vector<std::pair<int, std::pair<int, int>>> v) {
    for (std::vector<std::pair<int, std::pair<int, int>>>::iterator it=v.begin(); it!=v.end(); it++) {
        int token = (*it).first, lineno = (*it).second.first, charno = (*it).second.second;
        if (token == -1) {
            returnAndDisplayError(token, lineno, charno);
        } else {
            std::string s = "Hello World";
            yylval.strValue = &s; yylval.intValue = 10;
            returnAndDisplayToken(token);
        }
    }
}