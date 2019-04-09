#include "symbolTable.h"

SymbolTableEntry::SymbolTableEntry() {
    this->next = NULL;
}

SymbolTableEntry::SymbolTableEntry(std::string key, std::string value, std::string type, int lineNo) {
    this->id = key;
    this->scope = value;
    this->type = type;
    this->lineNo = lineNo;
    this->next = NULL;
}

SymbolTable::SymbolTable() {
    for(int i = 0; i < MAXSIZE; i++) {
        this->table[i] == NULL;
    }
}

// Hash function
// Somme des charactères modulo la taille de la table
int SymbolTable::hash(std::string id) {
    int sum = 0;

    for(int i = 0; i < id.length(); i++) {
        sum += id[i];
    }

    return sum%MAXSIZE;
}

bool SymbolTable::add(std::string id, std::string scope, std::string type, int lineNo) {
    int index = this->hash(id);
    SymbolTableEntry* newEntry = new SymbolTableEntry(id, scope, type, lineNo);

    if(this->table[index] == NULL) {
        this->table[index] = newEntry;
        
        return true;
    } else {
        SymbolTableEntry* entry = this->table[index];
        while(entry->next != NULL) {
            entry = entry->next;
        }
        entry->next = newEntry;

        return true;
    }

    return false;
}

SymbolTableEntry* SymbolTable::lookup(std::string id) {
    int index = this->hash(id);
    SymbolTableEntry* entry = this->table[index];

    if(entry == NULL) {
        return NULL;
    }

    while(entry != NULL) {
        if(entry->id == id) {
            return entry;
        }

        entry = entry->next;
    }

    return NULL;
}

bool SymbolTable::modify(std::string id, std::string scope, std::string type, int lineNo) {
    int index = this->hash(id);
    SymbolTableEntry* entry = this->table[index];

    if(entry == NULL) {
        return false;
    }

    while(entry != NULL) {
        if(entry->id == id) {
            entry->scope = scope;
            entry->type = type;
            entry->lineNo = lineNo;
            return true;
        }

        entry = entry->next;
    }

    return false;
}