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
    ASTNode *root;    // root of the AST to check
    SymbolTable *symbolTable;    //symbol table build form the AST
    std::map<std::string, std::string> *extend;    //used to resolve the extend relation of the class

    void preprocess(ASTNode *node);
    void registerClass(std::string className, std::vector<std::string> *waiting, int line, int col);
    void registerMethodAndField(ASTNode *node);
    void checkNode(ASTNode *root);
    bool isChildOf(std::string subclass, std::string testClass);
    std::string getFirstCommonAncestor(std::string c1, std::string c2);

public:
    Checker(ASTNode *root);
    void check();
};

#endif //VSOPCOMPILER_CHECKER_H
