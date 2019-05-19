#ifndef VSOPCOMPILER_CHECKER_H
#define VSOPCOMPILER_CHECKER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include "../utils/ASTNode.h"
#include "../utils/symbolTable.h"

class CheckerException : public std::exception
{
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

class Checker
{
private:
    ASTNode *root;                              // root of the AST to check
    SymbolTable *symbolTable;                   //symbol table build form the AST
    std::map<std::string, std::string> *extend; //used to resolve the extend relation of the class

    /**
     * Register the declared class, their methods and fields so that the checker can refer to them
     */
    void preprocess(ASTNode *node);
    /**
     * Register the class in the symbol table
     * className is the class to register
     * waiting is a stack of the classes that wait for className to be define
     */
    void registerClass(std::string className, std::vector<std::string> *waiting, int line, int col);
    /**
     * Register the methods and fields of a method
     */
    void registerMethodAndField(ASTNode *node);
    /**
     * Real semantic analysis
     * Check the scope and set the type of each node of the AST
     */
    void checkNode(ASTNode *root);
    /**
     * Check if the subclass is a child of the testclass
     */
    bool isChildOf(std::string subclass, std::string testClass);
    /**
     * Return the name of the first common ancestor of two classes
     */
    std::string getFirstCommonAncestor(std::string c1, std::string c2);

public:
    Checker(ASTNode *root);
    /**
     * Launch the semantic check process
     */
    void check();
};

#endif //VSOPCOMPILER_CHECKER_H
