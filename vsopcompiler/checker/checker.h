#ifndef VSOPCOMPILER_CHECKER_H
#define VSOPCOMPILER_CHECKER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include "../utils/ASTNode.h"
#include "../utils/symbolTable.h"

class CheckerException: public std::exception {
private:
    int linePos;
    int colPos;
    std::string errorText;

public:
    CheckerException(int linePos, int colPos, std::string errorText) throw()
        : linePos(linePos), colPos(colPos), errorText(errorText) {}

    virtual const char *what() const throw()
    {
        return this->errorText.c_str();
    }

    int getLineNumber() const throw()
    {
        return this->linePos;
    }

    int getColumnNumber() const throw()
    {
        return this->colPos;
    }

    virtual ~CheckerException() throw() {}
};

class Checker {
private:
    ASTNode *root;
    SymbolTable *symbolTable;
    std::map<std::string, std::string> *extend;

    bool preprocess(ASTNode *node);
    bool registerClass(std::string className, std::vector<std::string> *waiting, int line, int col);
    bool registerMethodAndField(ASTNode *node);
    bool checkNode(ASTNode *root);
    bool isChildOf(std::string subclass, std::string testClass);
    std::string getFirstCommonAncestor(std::string c1, std::string c2);

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
