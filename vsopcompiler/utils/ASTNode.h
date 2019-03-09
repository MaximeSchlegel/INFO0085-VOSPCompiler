#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <string>
#include <vector>
#include <iostream>

/* Class des node de AST
 * Args:
 *   - int lineno
 *   - int charno
 *   - string type
 *   - ??? prop
 *   - array<ASTNode> child
 * Methods:
 *   - create
 *   - add child
 *   - add prop
 *   - display
*/
//TODO: create the class and it's methods

class ASTNode {
private:
    int line;
    int column;
    std::string type;
    std::vector<ASTNode> children;
};


#endif //VSOPCOMPILER_ASTNODE_H
