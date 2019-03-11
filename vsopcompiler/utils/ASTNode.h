//
// Created by Altar on 11/03/2019.
//

#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <array>
#include <string>
#include <iostrem>

class ASTNode {
private:
    int line;
    int column;
    int type;
    int iValue;
    std::string strValue;
    std::array<* ASTNode> children;

public:
    ASTNode(int type, int line, int column);
    ASTNode(int type, int iValue, int line, int column);
    ASTNode(int type, std::strValue Value, int line, int column);
    void addChild (ASTNode * child);
    ostream &operator<<(ostream &os, const ASTNode & node);

};


#endif //VSOPCOMPILER_ASTNODE_H
