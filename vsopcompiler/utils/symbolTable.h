// Symbol table implemented using hashing with separate chaining
#ifndef VSOPCOMPILER_SYMBOLTABLE_H
#define VSOPCOMPILER_SYMBOLTABLE_H

#include <string>
#include <unordered_map>

const int MAXSIZE = 100;

class SymbolTableEntry {
private:
    std::string id;
    std::string scope;
    std::string type;
    int lineNo;

public:
    SymbolTableEntry(std::string id, std::string scope, std::string type, int lineNo);

    friend class SymbolTableScope;
};

class SymbolTableScope {
private:
    std::unordered_map<std::string, SymbolTableEntry*>* scope;
    SymbolTableScope* parent;

public:
    SymbolTableScope(SymbolTableScope* parent);
    void add(std::string id, std::string scope, std::string type, int lineNo);
    SymbolTableEntry* lookup(std::string id);

    friend class SymbolTable;
};

class SymbolTable {
private:
    SymbolTableScope* root;
    SymbolTableScope* head;

public:
    SymbolTable();
    bool add(std::string id, std::string scope, std::string type, int lineNo);
    SymbolTableEntry* lookup(std::string id);
    void pushScope();
    void popScope();
};

#endif // VSOPCOMPILER_SYMBOLTABLE_H