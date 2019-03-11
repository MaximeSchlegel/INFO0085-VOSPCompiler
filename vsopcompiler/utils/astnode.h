//
// Created by Altar on 11/03/2019.
//

#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H
#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <string>
#include <vector>
#include <iostream>

/* ASTNode Class
 * Args:
 *   - int lineno
 *   - int charno
 *   - int type
 *   - int iValue
 *   - str strValue
 *   - array<ASTNode> child
 * Methods:
 *   - create
 *   - add child
 *   - display
*/
//TODO: create the class and it's methods

class ASTNode {
private:
    int line;
    int column;
    int type;
    int iValue;
    std::string strValue;
    std::vector<ASTNode> children;

public:
    ASTNode(int type, int line, int colunm);
    ASTNode(int type, int iValue, int line, int colunm);
    ASTNode(int type, std::string strValue , int line, int colunm);
    void addChild(ASTNode child);
    ostream & operator<<(ostream &os, const Book &book);
};


#endif //VSOPCOMPILER_ASTNODE_H



#endif //VSOPCOMPILER_ASTNODE_H
