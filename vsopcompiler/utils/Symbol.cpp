#include "Symbol.h"
#include <string>
#include <iostream>


Symbol::Symbol(int type, std::string fileName, int line, int column) {
    this->type = type;
    this->fileName = fileName;
    this->line = line;
    this->column = column;
}

Symbol::Symbol(int type, std::string fileName, int value, int line, int column) {
    this->type = type;
    this->iValue = value;
    this->fileName = fileName;
    this->line = line;
    this->column = column;
}

Symbol::Symbol(int type, std::string fileName, std::string value, int line, int column) {
    this->type = type;
    this->sValue = value;
    this->fileName = fileName;
    this->line = line;
    this->column = column;
}
ostream & Symbol::operator<<(ostream &os, const Symbol &symbol) {
    if (symbol.type != ERROR) {
        os << symbol.line << "," << symbol.column << ",";
        switch (symbol.type) {
            case END: os << "end-of-file"; break;
            case BOOL: os << "bool"; break;
            case INT32: os << "int32"; break;
            case STRING: os << "string"; break;
            case UNIT: os << "unit"; break;
            case TYPEID: os << "type-identifier," << symbol.sValue; break;
            case INTLITERAL: os << "integer-literal," << symbol.iValue; break;
            case STRLITERAL: os << "string-literal," << symbol.sValue; break;
            case AND: os << "and"; break;
            case NOT: os << "not"; break;
            case CLASS: os << "class"; break;
            case DO: os << "do"; break;
            case ELSE: os << "else"; break;
            case EXTENDS: os << "extends"; break;
            case FALSE: os << "false"; break;
            case IF: os << "if"; break;
            case IN: os << "in"; break;
            case ISNULL: os << "isnull"; break;
            case LET: os << "let"; break;
            case NEW: os << "new"; break;
            case THEN: os << "then"; break;
            case TRUE: os << "true"; break;
            case WHILE: os << "while"; break;
            case OBJECTID: os << "object-identifier," << symbol.sValue; break;
            case LBRACE: os << "lbrace"; break;
            case RBRACE: os << "rbrace"; break;
            case LPAR: os << "lpar"; break;
            case RPAR: os << "rpar"; break;
            case COLON: os << "colon"; break;
            case SEMICOLON: os << "semicolon"; break;
            case COMMA: os << "comma"; break;
            case PLUS: os << "plus"; break;
            case MINUS: os << "minus"; break;
            case TIMES: os << "times"; break;
            case DIV: os << "div"; break;
            case POW: os << "pow"; break;
            case DOT: os << "dot"; break;
            case EQUAL: os << "equal"; break;
            case LOWER: os << "lower"; break;
            case LOWEREQUAL: os << "lower-equal"; break;
            case ASSIGN: os << "assign"; break;
        }
    } else {
        filename << ":" << lineno << ":" << charno << ": lexical error : " << symbol.sValue;
    }
}
