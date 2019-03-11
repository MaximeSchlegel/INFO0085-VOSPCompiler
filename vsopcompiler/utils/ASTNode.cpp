#include "ASTNode.h"


ASTNode::ASTNode(int type, int line, int column) {
    this->type = type;
    this->line = line;
    this->column = column;
}

ASTNode::ASTNode(int type, int iValue, int line, int column) {
    this->type = type;
    this->iValue = iValue;
    this->line = line;
    this->column = column;
}

ASTNode::ASTNode(int type, std::strValue Value, int line, int column) {
    this->type = type;
    this->strValue = strValue;
    this->line = line;
    this->column = column;
}

void ASTNode::addChild(ASTNode * child) {
    this->children.pushback(child);
}

std::ostream & ASTNode::operator<<(std::ostream &os, const ASTNode &node) {
    os << node.type << ", " << node.line << ", " << node.column;
}