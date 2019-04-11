#include "checker.h"


extern std::string filename;


Checker::Checker(ASTNode *root) {
    this->root = root;
    this->symbolTable = new SymbolTable();
    this->symbolTable->pushScope();

    this->classScope = new std::map<std::string, SymbolTableScope*>();
    this->symbolTable->pushScope();
    this->classScope->emplace("int32", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
    this->symbolTable->pushScope();
    this->classScope->emplace("bool", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
    this->symbolTable->pushScope();
    this->classScope->emplace("string", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
    this->symbolTable->pushScope();
    this->classScope->emplace("unit", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
    this->symbolTable->pushScope();
    this->classScope->emplace("Object", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
    this->symbolTable->pushScope();
    this->classScope->emplace("IO", this->symbolTable->getCurrentScope());
    this->symbolTable->popScope();
}


bool Checker::check() {
    return this->scopeCheck(this->root);
}


bool Checker::scopeCheck(ASTNode *node) {
    std::cout << "Begin" << std::endl;
    if (node->getType() == "program") {
        this->symbolTable->pushScope();
        std::vector<ASTNode *> nodeChildren = node->getChildren();

        //register all the declared classes
        this->registerClasses(node);

        //continue the scope checking in each class
        for(int i = 0; i < nodeChildren.size() ; i++) {
            if (!this->scopeCheck(nodeChildren[i])) {
                return false;
            };
        }

        //check if there is a Main::main
        std::map<std::string, SymbolTableScope *>::iterator it;
        if ((it = this->classScope->find("Main")) != this->classScope->end() && !it->second->lookup("main")) {
            return true;
        } else {
            std::cerr << "Can't find Main::main" << std::endl;
            return false;
        }

    } else if (node->getType() == "class") {
        std::vector<ASTNode *> nodeChildren = node->getChildren();

        //check if the parent class is define somewhere
        std::map<std::string, SymbolTableScope *>::iterator parentIt;
        if ((parentIt = this->classScope->find(nodeChildren[1]->getSValue())) == this->classScope->end()) {
            std::cerr << "La classe parente n'est pas definie" << std::endl;
            return false;
        }

        this->symbolTable->setCurrentScope(parentIt->second);
        for (int i = 2; i < nodeChildren.size(); i++) {
            continue;
        }
        std::cout << "RAS" << std::endl;
        return true;


    }else {
        std::cout << node->getType() << std::endl;
        return true;
    }
}


bool Checker::checkClass(ASTNode *node) {
    std::vector<ASTNode *> nodeChildren = node->getChildren();

    std::cout << "hi  " << nodeChildren[1]->getSValue() << std::endl;
    if (this->classScope->find(nodeChildren[1]->getSValue()) == this->classScope->end()) {
        std::cerr << "La classe parente n'est pas definie" << std::endl;
        return false;
    }

    std::cout << "RAS" << std::endl;
    return true;

}

void Checker::registerClasses(ASTNode *node) {
    std::vector<ASTNode *> nodeChildren = node->getChildren();

    for (int i = 0; i < nodeChildren.size(); i++) {
        if (nodeChildren[i]->getType() == "class") {
            std::string className = ((nodeChildren[i])->getChildren())[0]->getSValue();
            std::cout << nodeChildren[i]->getType() << "  " << className << std::endl;
            this->symbolTable->pushScope();
            this->classScope->emplace(className, this->symbolTable->getCurrentScope());
            this->symbolTable->popScope();
        }
    }
}