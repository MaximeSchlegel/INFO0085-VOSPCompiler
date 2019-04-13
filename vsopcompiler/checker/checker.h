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
    std::map<std::string, std::string> *extend;

    bool scopeCheck(ASTNode *node);
    bool registerClass(std::string className, std::vector<std::string> *waiting);
    bool registerMethodAndField(ASTNode *node);
    bool checkNode(ASTNode *root);

public:
    Checker(ASTNode *root);
    bool check();

    //TODO: definition des class
    //          - une class est nmmée Object
    //          - redifinition des class
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
