#include "symbolTable.h"
#include <iostream>

SymbolTableEntry::SymbolTableEntry(std::string id, std::string type, std::vector<SymbolTableEntry*>* formals, bool isMethod) {
    this->id = id;
    this->type = type;
    this->method = isMethod;
    this->formals = formals;
}

std::string SymbolTableEntry::getName() {
    return this->id;
}

std::string SymbolTableEntry::getType() {
    return this->type;
}

bool SymbolTableEntry::isMethod() {
    return this->method;
}

std::vector<SymbolTableEntry*>* SymbolTableEntry::getFormals() {
    return this->formals;
}

SymbolTableScope::SymbolTableScope(SymbolTableScope* parent) {
    this->parent = parent;
    this->scope = new std::unordered_map<std::string, SymbolTableEntry*>();
}

void SymbolTableScope::add(std::string id, std::string type, std::vector<SymbolTableEntry*>* formals, bool isMethod) {
    SymbolTableEntry* newEntry = new SymbolTableEntry(id, type, formals, isMethod);
    (*this->scope)[id] = newEntry;
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
    this->classes = new std::unordered_map<std::string, SymbolTableScope*>();

    SymbolTableScope* rootScope = new SymbolTableScope(NULL);

    this->currentScope = rootScope;
    this->previousScope = rootScope;
}

void SymbolTable::add(std::string id, std::string type, bool isMethod, std::vector<SymbolTableEntry*>* formals) {
    this->currentScope->add(id, type, formals, isMethod);
}

SymbolTableEntry* SymbolTable::lookup(std::string id) {
    SymbolTableScope* tmpScope = this->currentScope;
    // std::cout << this->currentScope->scope->size() << std::endl;

    while(tmpScope != NULL) {
        SymbolTableEntry* entry = tmpScope->lookup(id);

        if(entry != NULL) {
            return entry;
        }

        tmpScope = tmpScope->parent;
    }

    return NULL;
}

SymbolTableEntry* SymbolTable::lookupInCurrentScope(std::string id) {
    SymbolTableEntry* entry = this->currentScope->lookup(id);

    if(entry != NULL) {
        return entry;
    }

    return NULL;
}

bool SymbolTable::enterNewScope() {
    SymbolTableScope* newScope = new SymbolTableScope(this->currentScope);

    this->currentScope = newScope;
}

bool SymbolTable::enterNewScope(std::string className, std::string parent) {
    if(parent != "") {
        SymbolTableScope* parentScope;
        try {
          parentScope = this->classes->at(parent);
        } catch(std::out_of_range oor) {
            return false;
        }
        SymbolTableScope* newScope = new SymbolTableScope(parentScope);
        this->classes->emplace(className, newScope);
        this->currentScope = newScope;
    } else {
        SymbolTableScope* newScope = new SymbolTableScope(this->currentScope);
        this->classes->emplace(className, newScope);
        this->currentScope = newScope;
    }

    return true;
}

void SymbolTable::exitScope() {
    SymbolTableScope* parentScope = this->currentScope->parent;
    this->currentScope = parentScope;
}

bool SymbolTable::hasClass(std::string name) {
    std::size_t n = this->classes->count(name);

    return n == 1 ? true : false;
}

SymbolTableScope* SymbolTable::getScope(std::string name) {
    if(this->hasClass(name)) {
        return this->classes->at(name);
    }
    return NULL;
}

void SymbolTable::enterScope(std::string name) {
    if(this->hasClass(name)) {
        this->previousScope = this->currentScope;
        this->currentScope = this->classes->at(name);
    }
}

void SymbolTable::exitScope(std::string name) {
    if(this->hasClass(name)) {
        this->currentScope = this->previousScope;
    }
}