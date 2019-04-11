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

bool SymbolTable::enterNewScope(std::string scopeId, std::string parent = "") {
    if(parent != "") {
        SymbolTableScope* parentScope;
        try {
          parentScope = this->scopes->at(parent);
        } catch(std::out_of_range oor) {
            return false;
        }
        SymbolTableScope* newScope = new SymbolTableScope(parentScope);
        this->scopes->emplace(scopeId, newScope);
        this->currentScope = newScope;
    } else {
        SymbolTableScope* newScope = new SymbolTableScope(this->currentScope);
        this->scopes->emplace(scopeId, newScope);
        this->currentScope = newScope;
    }

    return true;
}

void SymbolTable::exitScope() {
    SymbolTableScope* parentScope = this->currentScope->parent;
    this->currentScope = parentScope;
}

bool SymbolTable::hasScope(std::string name) {
    std::size_t n = this->scopes->count(name);

    return n == 1 ? true : false;
} 