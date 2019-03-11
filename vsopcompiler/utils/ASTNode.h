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
    int type;
    int iValue;
    std::string strValue;
    std::vector<ASTNode *> children;

public:
    ASTNode();
    ASTNode(int type, int line, int column);
    ASTNode(int type, int iValue, int line, int column);
    ASTNode(int type, std::string strValue, int line, int column);
    void addChild (ASTNode * child);
    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);
};


#endif //VSOPCOMPILER_ASTNODE_H
