#include "ASTNode.h"
std::string ttos (int token) {
    switch (token) {
        case 260: return "bool";
        case 261: return "int32";
        case 262: return "string";
        case 263: return "unit";
    }
    return "error";
}

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

std::string ASTNode::getType() {
    if (this->iType) {
        return ttos(this->iType);
    } else if (this->iType == 264) {
        this->sValue;
    }
    return "error";
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
