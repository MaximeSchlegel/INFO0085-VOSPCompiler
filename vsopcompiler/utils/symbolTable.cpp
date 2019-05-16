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
    // std::cout << "Scope lookup" << std::endl;
    SymbolTableEntry* entry = NULL;
    try {
        entry = this->scope->at(id);
    } catch(std::out_of_range oor) {
        // std::cout << "Scope lookup nothing" << std::endl;
        return NULL;
    }
    // std::cout << "Scope lookup something" << std::endl;
    return entry;
}

SymbolTable::SymbolTable() {
    this->classes = new std::unordered_map<std::string, SymbolTableScope*>();

    SymbolTableScope* rootScope = new SymbolTableScope(NULL);

    this->currentScope = rootScope;
    this->previousScope = rootScope;
}

void SymbolTable::add(std::string id, std::string type, bool isMethod, std::vector<SymbolTableEntry*>* formals) {
    std::cout << "ADD of " << id << "_________________" << std::endl;
    for (auto it = this->classes->begin(); it != this->classes->end(); it++)
    {
        if (it->second == this->currentScope)
        {
            std::cout << "ADD in scope " << it->first << std::endl;
        }
    }
    this->currentScope->add(id, type, formals, isMethod);
}

SymbolTableEntry* SymbolTable::lookup(std::string id) {
    SymbolTableScope* tmpScope = this->currentScope;
    // std::cout << "Lookup for " << id << std::endl;

    while(tmpScope != NULL) {
        for (auto it = this->classes->begin(); it != this->classes->end(); it++)
        {
            if (it->second == tmpScope)
            {
                // std::cout << "In scope " << it->first << std::endl;
            }
        }
        SymbolTableEntry* entry = tmpScope->lookup(id);

        if(entry != NULL) {
            return entry;
        }

        tmpScope = tmpScope->parent;
        // std::cout << "  try parent" << std::endl;
    }

    // std::cout << "End lookup nothing" << std::endl;
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
        SymbolTableScope* newScope = new SymbolTableScope(NULL);
        this->classes->emplace(className, newScope);
        this->currentScope = newScope;
    }
    this->add("self", className);
    return true;
}

void SymbolTable::exitScope() {
    SymbolTableScope* parentScope = this->currentScope->parent;
    this->currentScope = parentScope;
}

bool SymbolTable::hasClass(std::string name) {
    std::size_t n = this->classes->count(name);

    // std::cout << "For name " << name << " " << n << " occurences found" << std::endl;

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
