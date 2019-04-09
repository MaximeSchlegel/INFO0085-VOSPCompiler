// Symbol table implemented using hashing with separate chaining
#ifndef VSOPCOMPILER_SYMBOLTABLE_H
#define VSOPCOMPILER_SYMBOLTABLE_H

#include <string>

const int MAXSIZE = 100;

class SymbolTableEntry {
private:
    std::string id;
    std::string scope;
    std::string type;
    int lineNo;
    SymbolTableEntry* next;

public:
    SymbolTableEntry();
    SymbolTableEntry(std::string key, std::string value, std::string type, int lineNo);

    friend class SymbolTable;
};

class SymbolTable {
private:
    SymbolTableEntry* table[MAXSIZE];

public:
    SymbolTable();
    int hash(std::string id);
    bool add(std::string id, std::string scope, std::string type, int lineNo);
    SymbolTableEntry* lookup(std::string id);
    bool modify(std::string id, std::string scope, std::string type, int lineNo);
};

#endif // VSOPCOMPILER_SYMBOLTABLE_H