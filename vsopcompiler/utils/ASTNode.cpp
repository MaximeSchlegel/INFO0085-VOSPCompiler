#include "ASTNode.h"


ASTNode::ASTNode(int type, int line, int column) {
    this->iType = type;
    this->isTerminal = true;
    this->line = line;
    this->column = column;
}

ASTNode::ASTNode(int type, int iValue, int line, int column) {
    this->iType = type;
    this->isTerminal = true;
    this->iValue = iValue;
    this->line = line;
    this->column = column;
}

ASTNode::ASTNode(int type, std::string * sValue, int line, int column) {
    this->iType = type;
    this->sValue = sValue;
    this->isTerminal = true;
    this->line = line;
    this->column = column;
}

ASTNode::ASTNode(std::string type, int line, int column) {
    this->sType = type;
    this->isTerminal = false;
    this->line = line;
    this->column = column;
}

void ASTNode::addChild(ASTNode * child) {
    this->children.push_back(child);
}

std::ostream & operator<<(std::ostream & os, const ASTNode & node) {
    if (node.isTerminal) {
        os << node.iType << ", " << node.line << ", " << node.column;
    } else {
        os << node.sType << ", " << node.line << ", " << node.column;
    }
    return os;
}

ASTNode::~ASTNode() {
    for(std::vector<ASTNode *>::iterator it=this->children.begin(); it!=this->children.end(); it++) {
        delete (*it);
    }
}
