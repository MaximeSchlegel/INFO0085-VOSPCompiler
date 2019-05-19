//
// Created by Altar on 11/03/2019.
//

#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

typedef struct {
    int iProp;
    std::string sProp;
}prop;

class ASTNode {
private:
    int iType;    // type of a terminal node
    std::string sType;    //type of a non terminal node
    int iValue;    // value of a int literal
    std::string *sValue;  // value of all the other node meanings changes according to the type of the node
    std::map<std::string, prop> properties;    // hold lines, and return types od the node
    std::vector<ASTNode *> children;    // hold the child of this node

public:
    ASTNode(int type);
    ASTNode(int type, int iValue);
    ASTNode(int type, std::string sType);
    ASTNode(int type, std::string *sValue);
    ASTNode(std::string type);
    std::string getSValue();
    void setPosition(int line, int column);
    int getLine();
    int getColumn();
    void setReturnType(std::string type);
    std::string getReturnType()const;
    std::string printReturnType()const;
    std::string getType()const ;
    void addChild (ASTNode * child, bool front = false);
    std::vector<ASTNode *> getChildren();
    bool doesSubTreeContains(std::string name);
    friend std::ostream & operator<<(std::ostream & os, const ASTNode & node);
    ~ASTNode();
};


#endif //VSOPCOMPILER_ASTNODE_H
