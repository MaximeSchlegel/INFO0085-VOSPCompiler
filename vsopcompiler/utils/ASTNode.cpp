#include "ASTNode.h"

ASTNode::ASTNode(int type) {
    this->iType = type;
    this->sType = "";
    this->children = std::vector<ASTNode *>();
    this->properties = std::map<std::string, prop>();
}

ASTNode::ASTNode(int type, int iValue) {
    this->iType = type;
    this->sType = "";
    this->iValue = iValue;
    this->children = std::vector<ASTNode *>();
    this->properties = std::map<std::string, prop>();
}

ASTNode::ASTNode(int type, std::string *sValue) {
    this->iType = type;
    this->sValue = sValue;
    this->children = std::vector<ASTNode *>();
    this->properties = std::map<std::string, prop>();
}

ASTNode::ASTNode(std::string type) {
    this->iType = -1;
    this->sType = type;
    this->children = std::vector<ASTNode *>();
    this->properties = std::map<std::string, prop>();
}

void ASTNode::setPosition(int line, int column) {
    prop l; l.iProp = line;
    prop c; c.iProp = column;
    this->properties.emplace("line", l);
    this->properties.emplace("column", c);
}

int ASTNode::getLine() {
    return this->properties.find("line")->second.iProp;
}

int ASTNode::getColumn() {
    return this->properties.find("column")->second.iProp;
}

std::string ASTNode::getSValue() {
    return *this->sValue;
}

void ASTNode::setReturnType(std::string type) {
    prop t; t.sProp = type;
    this->properties.emplace("type", t);
}

std::string ASTNode::getReturnType() {
    std::map<std::string, prop>::iterator it = this->properties.find("type");
    if (it == this->properties.end()){
        return "";
    } else {
        return it->second.sProp;
    }
}

std::string ASTNode::getType() const {
    if (this->iType != -1) {
        switch (this->iType) {
            case 260: return "bool";
            case 261: return "int32";
            case 262: return "string";
            case 263: return "unit";
            case 264: return "typeid";
            case 265: return "true";
            case 266: return "false";
            case 267: return "intliteral";
            case 268: return "stringliteral";
        }
    } else  if (this->sType != "") {
        return this->sType;
    }
    return "error";
}

void ASTNode::addChild(ASTNode * child, bool front) {
    if (front){
        std::vector<ASTNode *>::iterator it = this->children.begin();
        this->children.insert(it, child);
    } else {
        this->children.push_back(child);
    }
}

std::vector<ASTNode *> ASTNode::getChildren() {
    return this->children;
}

std::ostream & operator<<(std::ostream & os, const ASTNode & node) {
    // Same order as in vsopc.y

    if (node.iType != -1) {
        switch (node.iType) {
            case 260: os << "bool"; break;
            case 261: os << "int32"; break;
            case 262: os << "string"; break;
            case 263: os << "unit"; break;
            case 264: os << *node.sValue; break;
            case 265: os << "true"; break;
            case 266: os << "false"; break;
            case 267: os << node.iValue; break;
            case 268: os << *node.sValue; break;
            case 282: os << *node.sValue; break;
        }

    } else if (node.sType.compare("program") == 0) {
        os << "[" << std::endl;
        for (int i = 0; i < node.children.size(); i++) {
            os << *node.children[i];
            if (i != node.children.size() - 1) {
                os << "," << std::endl;
            }
        }
        os << std::endl << " ]" << std::endl;

    } else if (node.sType.compare("class") == 0) {
        os << "Class(" << *(node.children[0]) << ", " << *node.children[1] << "," << std::endl;
        os << "      [";
        bool first = true;
        for (auto const& child: node.children) {
            if (child->sType == "field") {
                if (!first) {
                    os << ", " << *child;
                } else {
                    first = false;
                    os << *child;
                }
            }
        }
        os << "]," << std::endl << "      [";
        first = true;
        for (auto const& child: node.children) {
            if (child->sType == "method") {
                if(!first) {
                    os << ",\n        " << *child;

                } else {
                    first = false;
                    os << *child;
                }
            }
        }
        os << "])";

    } else if (node.sType.compare("field") == 0) {
        if (node.children.size() == 3) {
            os << "Field(" << *node.children[0] << ", " << *node.children[1] << ", " << *node.children[2] << ")";
        } else {
            os << "Field(" << *node.children[0] << ", " << *node.children[1] << ")";
        }

    } else if (node.sType.compare("method") == 0) {
        os << "Method(" << *node.children[0] << ", [";
        if (node.children.size() == 4) {
            os << *node.children[3];
        }
        os << "], " << *node.children[1] << "," << std::endl;
        os << "               " << *node.children[2] <<")";

    } else if (node.sType.compare("formals") == 0) {
        for (int i = 0 ; i < node.children.size() ; i++) {
            os << *node.children[i];
            if (i != node.children.size() - 1) {
                os << ", ";
            }
        }

    } else if (node.sType.compare("formal") == 0) {
        os << *node.children[0] << " : " << *node.children[1];

    } else if (node.sType.compare("block") == 0) {
        if (node.children.size() == 1) {
            os << *node.children[0];
        } else {
            os << "[";
            for (int i = 0; i < node.children.size(); i++) {
                os << *node.children[i];
                if (i != node.children.size() - 1){
                    os << ", ";
                }
            }
            os << "]";
        }

    } else if (node.sType.compare("if") == 0) {
        if (node.children.size() == 3) {
            os << "If(" << *node.children[0] << ", " << *node.children[1] << ", " << *node.children[2] << ")";
        } else {
            os << "If(" << *node.children[0] << ", " << *node.children[1] << ")";
        }

    } else if (node.sType.compare("while") == 0) {
        os << "While(" << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("let") == 0) {
        if (node.children.size() == 4) {
            os << "Let(" << *node.children[0] << ", " << *node.children[1] << ", " << *node.children[2] << ", " << *node.children[3] << ")";
        } else {
            os << "Let(" << *node.children[0] << ", " << *node.children[1] << ", " << *node.children[2] << ")";
        }
    } else if (node.sType.compare("assign") == 0) {
        os << "Assign(" << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("not") == 0) {
        os << "UnOp(not, " << *node.children[0] << ")";

    } else if (node.sType.compare("and") == 0) {
        os << "BinOp(and, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("equal") == 0) {
        os << "BinOp(=, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("lower") == 0) {
        os << "BinOp(<, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("lowerequal") == 0) {
        os << "BinOp(<=, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("plus") == 0) {
        os << "BinOp(+, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("minus") == 0) {
        os << "BinOp(-, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("times") == 0) {
        os << "BinOp(*, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("div") == 0) {
        os << "BinOp(/, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("pow") == 0) {
        os << "BinOp(^, " << *node.children[0] << ", " << *node.children[1] << ")";

    } else if (node.sType.compare("neg") == 0) {
        os << "UnOp(-, " << *node.children[0] << ")";

    } else if (node.sType.compare("isnull") == 0) {
        os << "UnOp(isnull, " << *node.children[0] << ")";

    } else if (node.sType.compare("call") == 0) {
        os << "Call(" << *node.children[0] << ", " << *node.children[1] << ", [";
        if (node.children.size() == 3) {
            os << *node.children[2];
        }
        os << "])";

    } else if (node.sType.compare("new") == 0) {
        os << "New(" << *node.children[0] << ")";

    } else if (node.sType.compare("args") == 0) {
        for (int i = 0; i < node.children.size(); i++) {
            if (i != 0){
                os << ", ";
            }
            os << *node.children[i];
        }

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