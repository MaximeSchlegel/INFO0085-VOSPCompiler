//
// Created by Altar on 11/03/2019.
//

#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

typedef struct
{
    int iProp;
    std::string sProp;
} prop;

class ASTNode
{
private:
    int iType;                              // type of a terminal node
    std::string sType;                      // type of a non terminal node
    int iValue;                             // value of a int literal
    std::string *sValue;                    // value of all the other node meanings changes according to the type of the node
    std::map<std::string, prop> properties; // hold lines, and return types od the node
    std::vector<ASTNode *> children;        // hold the child of this node

public:
    /**
     * Create a terminal node from a vsop native type, true or false literal
     */
    ASTNode(int type);
    /**
     * create a terminal node from a vsop int literal
     */
    ASTNode(int type, int iValue);
    /**
     * Create a terminal node from a string literal
     */
    ASTNode(int type, std::string *sValue);
    /**
     * Create a non-terminal node -> program, class, formal, etc
     */
    ASTNode(std::string type);
    /**
     * Set the position of the node -> position of the token which create the node
     */
    void setPosition(int line, int column);
    /**
     * Get the line of the token which create the node
     */
    int getLine();
    /**
     * Get the colum of the token which create the node
     */
    int getColumn();
    /**
     * Return the sValue of the node
     */
    std::string getSValue();
    /**
     * Set the type of a non terminal node in the properties list
     */
    void setReturnType(std::string type);
    /**
     * Return the return type of the node
     */
    std::string getReturnType() const;
    /**
     * Return a string with the printable version of the type of the node
     */
    std::string printReturnType() const;
    /**
     * Return the type of the node
     */
    std::string getType() const;
    /**
     * Add a child to a given node, by default add it to the end of the list
     */
    void addChild(ASTNode *child, bool front = false);
    /**
     * Return the list of children node
     */
    std::vector<ASTNode *> getChildren();
    /**
     * Search for a given node in the children of the node
     */
    bool doesSubTreeContains(std::string name);
    /**
     * Recursively go through the AST to display it
     */
    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);
    ~ASTNode();
};

#endif //VSOPCOMPILER_ASTNODE_H
