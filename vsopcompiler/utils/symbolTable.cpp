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
    this->root = new SymbolTableScope(NULL);
    this->head = this->root;
}

bool SymbolTable::add(std::string id, std::string scope, std::string type, int lineNo) {
    SymbolTableScope* currentScope = this->head;
    currentScope->add(id, scope, type, lineNo);
}

SymbolTableEntry* SymbolTable::lookup(std::string id) {
    SymbolTableScope* currentScope = this->head;

    while(currentScope != NULL) {
        SymbolTableEntry* entry = currentScope->lookup(id);

        if(entry != NULL) {
            return entry;
        }

        currentScope = currentScope->parent;
    }

    return NULL;
}

void SymbolTable::pushScope() {
    SymbolTableScope* currentScope = this->head;

    SymbolTableScope* newScope = new SymbolTableScope(currentScope);

    this->head = newScope;
}

void SymbolTable::popScope() {
    SymbolTableScope* currentScope = this->head;

    SymbolTableScope* parent = currentScope->parent;

    this->head = parent;
}