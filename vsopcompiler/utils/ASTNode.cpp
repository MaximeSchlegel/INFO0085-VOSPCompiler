#include "ASTNode.h"


ASTNode::ASTNode(int type) {
    this->iType = type;
}

ASTNode::ASTNode(int type, int iValue) {
    this->iType = type;
    this->iValue = iValue;
}

ASTNode::ASTNode(int type, std::string * sValue) {
    this->iType = type;
    this->sValue = sValue;
}

ASTNode::ASTNode(std::string type) {
    this->sType = type;
}

void ASTNode::setPosition(int line, int column) {
    prop l; l.iProp = line;
    prop c; c.iProp = column;
    this->properties.emplace("line", l);
    this->properties.emplace("column", c);
}

void ASTNode::setType(std::string type) {
    prop t; t.sProp = type;
    this->properties.emplace("type", t);
}

void ASTNode::addChild(ASTNode * child) {
    this->children.push_back(child);
}

std::ostream & operator<<(std::ostream & os, const ASTNode & node) {
    if (node.iType) {
        os << node.iType;
    } else {
        os << node.sType;
    }
    return os;
}

ASTNode::~ASTNode() {
    for(std::vector<ASTNode *>::iterator it=this->children.begin(); it!=this->children.end(); it++) {
        delete (*it);
    }
}
