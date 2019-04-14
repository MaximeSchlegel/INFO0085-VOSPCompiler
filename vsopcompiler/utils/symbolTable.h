// Symbol table implemented using hashing with separate chaining
#ifndef VSOPCOMPILER_SYMBOLTABLE_H
#define VSOPCOMPILER_SYMBOLTABLE_H

#include <string>
#include <vector>
#include <unordered_map>

const int MAXSIZE = 100;

class SymbolTableEntry {
private:
    std::string id;
    std::string type;
    bool method;
    std::vector<SymbolTableEntry*>* formals;

public:
    //TODO: il faut un moyen d'acceder au args
    //TODO: il faut revoir les arguments qu'il stocke
    SymbolTableEntry(std::string id, std::string type, std::vector<SymbolTableEntry*>* formals, bool isMethod = false);
    std::string getName();
    std::string getType();
    bool isMethod();
    std::vector<SymbolTableEntry*>* getFormals();

    friend class SymbolTableScope;
};


class SymbolTableScope {
private:
    std::unordered_map<std::string, SymbolTableEntry*>* scope;
    SymbolTableScope* parent;

public:
    SymbolTableScope(SymbolTableScope* parent);
    void add(std::string id, std::string type, std::vector<SymbolTableEntry*>* formals, bool isMethod = false);
    SymbolTableEntry* lookup(std::string id);

    friend class SymbolTable;
};


class SymbolTable {
private:
    std::unordered_map<std::string, SymbolTableScope*>* classes;
    SymbolTableScope* currentScope;
    SymbolTableScope* previousScope;

public:
    SymbolTable();
    /**
     * Add entry to current scope
     */
    void add(std::string id, std::string type, bool isMethod = false, std::vector<SymbolTableEntry*>* formals = NULL);
    /**
     * Look up for a given entry
     */
    SymbolTableEntry* lookup(std::string id);
    /**
     * Look up for a given entry in the current scope
     */
    SymbolTableEntry* lookupInCurrentScope(std::string id);
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
    bool hasClass(std::string name);
    /**
     * Return the scope associated with the given class name
     */
    SymbolTableScope* getScope(std::string name);
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