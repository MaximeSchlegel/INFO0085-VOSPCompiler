#ifndef VSOPCOMPILER_CHECKER_H
#define VSOPCOMPILER_CHECKER_H

#include <map>
#include <vector>
#include <string>

#include "../utils/ASTNode.h"


class Checker {
private:
    std::map<std::string, std::vector<std::string> *> *primary;
//    std::map<std::string, std::vector<string>> declared_variable;
    // declared_type
    // declared class.method
    // declared object with scope

public:
    Checker();
    int check(ASTNode *root);
    void registerClasses(ASTNode *root);
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
