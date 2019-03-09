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

void Symbol::display() {


}