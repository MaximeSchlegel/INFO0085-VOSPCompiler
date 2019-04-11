#ifndef VSOPCOMPILER_CHECKER_H
#define VSOPCOMPILER_CHECKER_H

#include <map>
#include <vector>
#include <string>

#include "../utils/ASTNode.h"
#include "../utils/symbolTable.h"


class Checker {
private:
    ASTNode *root;
    SymbolTable *symbolTable;
    std::map<std::string, SymbolTableScope*> *classScope;

public:
    Checker(ASTNode *root);
    bool check();
    bool scopeCheck(ASTNode *node);
    bool checkClass(ASTNode *node);
    void registerClasses(ASTNode *node);
    //TODO: definition des class
    //          - redifinition des class
    //          - une class est nmmée Object
    //          - cycle dans les héritages

    //TODO: methodes
    //          - args avec le même nom
    //          - overwrite d'une methode d'un parent avec des args ou return value differente

    //TODO: field
    //          - redifinition de type
    //          - un field est nommé self

    //TODO: Type
    //          - un type non défini est utilisé
    //          - les type des return value doit match ceux attendu par la node au dessus

    //TODO: ObjectID
    //          - utilisation d'un Id non defini dans le scope

    //TODO: Other
    //          - self est attribué

    //TODO: Main
    //          - le programme doit avoir une class Main
    //          - la class Main doit avoir une methode main
    //          - Main.main signature: unit -> int32
};

#endif //VSOPCOMPILER_CHECKER_H
