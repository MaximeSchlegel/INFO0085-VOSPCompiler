#ifndef VSOPCOMPILER_SYMBOL_H
#define VSOPCOMPILER_SYMBOL_H

#include <string>
#include <iostream>


class Symbol {
private:
    //position of the symbole in the files
    std::string fileName;
    int line;
    int column;
    //type and value of the symbole
    //use -1 for error and error message in sValue
    int type;
    int iValue;
    std::string sValue;

public:
    Symbol(int type, std::string fileName, int line, int column);
    Symbol(int type, int value, std::string fileName, int line, int column);
    Symbol(int type, std::string value, std::string fileName, int line, int column);
    std::iostream

};


#endif //VSOPCOMPILER_SYMBOL_H
