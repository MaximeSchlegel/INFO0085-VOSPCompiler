/*
 * This file contains all the class needed for the definition of a Symbol Table.
 * It is implemented using hashing with separate chaining
 */
#ifndef VSOPCOMPILER_SYMBOLTABLE_H
#define VSOPCOMPILER_SYMBOLTABLE_H

#include <string>
#include <vector>
#include <unordered_map>

/**
 * Class that represents an entry in the symbol table.
 */
class SymbolTableEntry
{
private:
    std::string id;
    std::string type;
    bool isMethod;
    std::vector<SymbolTableEntry *> *formals;

public:
    SymbolTableEntry(std::string id, std::string type, std::vector<SymbolTableEntry *> *formals = nullptr, bool isMethod = false);
    /**
     * Return the name of the entry
     */
    std::string getName() const;
    /**
     * Return the type of the entry
     */
    std::string getType() const;
    /**
     * Return true if the entry is a method, false otherwise
     */
    bool isMethod() const;
    /**
     * Return the list of formals of the entry if it is a method entry
     */
    std::vector<SymbolTableEntry *> *getFormals() const;

    friend class SymbolTableScope;
};

/**
 * Class that represents a scope in the symbol table.
 * A scope holds a list of SymbolTableEntry mapped to their name.
 */
class SymbolTableScope
{
private:
    std::unordered_map<std::string, SymbolTableEntry *> *scope;
    SymbolTableScope *parent;

public:
    SymbolTableScope(SymbolTableScope *parent);
    /**
     * Add an entry to the scope
     */
    void add(std::string id, std::string type, std::vector<SymbolTableEntry *> *formals, bool isMethod = false);
    /**
     * Lookup for a given name and retun the corresponding SymbolTableEntry
     */
    SymbolTableEntry *lookup(std::string id) const;
    /**
     * Return all the names register in the scope
     */
    std::vector<std::string> getNames() const;

    friend class SymbolTable;
};

/**
 * Class that represents a symbol table.
 * It contains a list of SymbolTableScope mapped to their name.
 */
class SymbolTable
{
private:
    std::unordered_map<std::string, SymbolTableScope *> *classes;
    SymbolTableScope *currentScope;
    SymbolTableScope *previousScope;

public:
    SymbolTable();
    /**
     * Add entry to current scope
     */
    void add(std::string id, std::string type, bool isMethod = false, std::vector<SymbolTableEntry *> *formals = NULL);
    /**
     * Look up for a given entry
     */
    SymbolTableEntry *lookup(std::string id) const;
    /**
     * Look up for a given entry in the current scope
     */
    SymbolTableEntry *lookupInCurrentScope(std::string id) const;
    /**
     * Enter a new scope
     */
    bool enterNewScope();
    /**
     * Enter a new scope for a class
     */
    bool enterNewScope(std::string className, std::string parent = "");
    /**
     * Exit scope
     */
    void exitScope();
    /**
     * Check if there exists a scope associated with the given class name
     */
    bool hasClass(std::string name) const;
    /**
     * Return the scope associated with the given class name
     */
    SymbolTableScope *getScope(std::string name) const;
    /**
     * Enter the scope associated with the given class name
     */
    void enterScope(std::string name);
    /**
     * Exit the scope associated with the given class name
     */
    void exitScope(std::string name);
};

#endif // VSOPCOMPILER_SYMBOLTABLE_H
