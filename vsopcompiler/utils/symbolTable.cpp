#include "symbolTable.h"

SymbolTableEntry::SymbolTableEntry(std::string id, std::string scope, std::string type, int lineNo) {
    this->id = id;
    this->scope = scope;
    this->type = type;
    this->lineNo = lineNo;
}

SymbolTableScope::SymbolTableScope(SymbolTableScope* parent) {
    this->parent = parent;
    this->scope = new std::unordered_map<std::string, SymbolTableEntry*>();
}

void SymbolTableScope::add(std::string id, std::string scope, std::string type, int lineNo) {
    SymbolTableEntry* newEntry = new SymbolTableEntry(id, scope, type, lineNo);
    this->scope->emplace(id, newEntry);
}

SymbolTableEntry* SymbolTableScope::lookup(std::string id) {
    SymbolTableEntry* entry = NULL;
    try {
         entry = this->scope->at(id);
    } catch(std::out_of_range oor) {
        return NULL;
    }

    return entry;
}

SymbolTable::SymbolTable() {
    this->scopes = new std::unordered_map<std::string, SymbolTableScope*>();

    SymbolTableScope* rootScope = new SymbolTableScope(NULL);
    this->scopes->emplace("root", rootScope);

    this->currentScope = rootScope;
}

void SymbolTable::add(std::string id, std::string scope, std::string type, int lineNo) {
    this->currentScope->add(id, scope, type, lineNo);
}

SymbolTableEntry* SymbolTable::lookup(std::string id) {
    SymbolTableScope* tmpScope = this->currentScope;

    while(tmpScope != NULL) {
        SymbolTableEntry* entry = tmpScope->lookup(id);

        if(entry != NULL) {
            return entry;
        }

        tmpScope = tmpScope->parent;
    }

    return NULL;
}

void SymbolTable::enterNewScope(std::string scopeId, std::string parent = "") {
    if(parent != "") {
        SymbolTableScope* parentScope = this->scopes->at(parent);
        SymbolTableScope* newScope = new SymbolTableScope(parentScope);
        this->scopes->emplace(scopeId, newScope);
        this->currentScope = newScope;
    } else {
        SymbolTableScope* newScope = new SymbolTableScope(this->currentScope);
        this->scopes->emplace(scopeId, newScope);
        this->currentScope = newScope;
    }
}

void SymbolTable::exitScope() {
    SymbolTableScope* parentScope = this->currentScope->parent;
    this->currentScope = parentScope;
}