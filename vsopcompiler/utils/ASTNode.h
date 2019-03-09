#ifndef VSOPCOMPILER_ASTNODE_H
#define VSOPCOMPILER_ASTNODE_H

#include <vector>

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
class ASTNode {
private:
    int line;
    int column;
    int type;
    std::vector<ASTNode> children;
};


#endif //VSOPCOMPILER_ASTNODE_H
