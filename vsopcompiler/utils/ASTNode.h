//
// Created by Altar on 11/03/2019.
//

#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <vector>
#include <string>
#include <iostream>

class ASTNode {
private:
    int line;
    int column;
    int iType;
    std::string sType;
    int iValue;
    std::string * sValue;
    std::vector<ASTNode *> children;
    bool isTerminal;

public:
    ASTNode(int type, int line, int column);
    ASTNode(int type, int iValue, int line, int column);
    ASTNode(int type, std::string * sValue, int line, int column);
    ASTNode(std::string type, int line, int column);
    void addChild (ASTNode * child);
    friend std::ostream & operator<<(std::ostream & os, const ASTNode & node);
    ~ASTNode();
};


#endif //VSOPCOMPILER_ASTNODE_H
