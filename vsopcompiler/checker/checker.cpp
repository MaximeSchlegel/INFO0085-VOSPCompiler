#include "checker.h"


extern std::string filename;


Checker::Checker(ASTNode *root) {
    this->root = root;
    this->symbolTable = new SymbolTable();
    this->symbolTable->enterNewScope("int32");
    this->symbolTable->enterNewScope("bool");
    this->symbolTable->enterNewScope("string");
    this->symbolTable->enterNewScope("unit");
    this->symbolTable->enterNewScope("Object");
    this->symbolTable->enterNewScope("IO");

    this->extend = new std::map<std::string, std::string>();
}


bool Checker::check() {
    return this->scopeCheck(this->root);
}


bool Checker::scopeCheck(ASTNode *node) {
    if (node->getType() == "program") {
        std::vector<ASTNode *> children = node->getChildren();

        //register all the declared classes and their parent
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->getType() == "class") {
                std::string className = (children[i]->getChildren())[0]->getSValue();
                std::string classParent = (children[i]->getChildren())[1]->getSValue();
                if(this->extend->find(className) != this->extend->end()) {
                    //we have already encounter it
                    std::cerr << "The class" << className << "has been define several times" << std::endl;
                    return false;
                }
                this->extend->emplace(className, classParent);
            }
        }
        //continue the scope checking in each class
        for(int i = 0; i < children.size() ; i++) {
            if (!this->scopeCheck(children[i])) {
                return false;
            };
        }
        //check if there is a Main::main
        std::map<std::string, std::string>::iterator mainIt = this->extend->find("Main");
        if (mainIt == this->extend->end()) {
            std::cerr << "Can't find the Main class" << std::endl;
            return false;
        }
        this->symbolTable->enterNewScope("Main");
        if (!this->symbolTable->lookup("main")) {
            std::cerr << "Can't find Main::main methode" << std::endl;
            return false;
        }
        return true;

    } else if (node->getType() == "class") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();
        // check if the class is already define
        if (this->symbolTable->hasScope(name)) {
            //register its methods and fields
            if (!this->registerMethodAndField(node)) {
                return false;
            }
        } else {
            //register the class then its methods and field
            if ( !this->registerClass(name, new std::vector<std::string>()) || !this->registerMethodAndField(node)) {
                return false;
            }
        }
        //continue the checking on the methods and fields
        for(int i = 0; i < children.size() ; i++) {
            if (!this->scopeCheck(children[i])) {
                return false;
            }
        }
        return true;

    } else if (node->getType() == "field") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();
        //test if the field is assign;
        if (children.size() == 3) {
            //check the expr and set it's return type
            if (!this->scopeCheck(children[3])) {
                return false;
            }
            //check that the return type is conform to the one declare
            if (this->symbolTable->lookup(name)->type != children[3]->getType()) {
                std::cerr << "Type doesn't match" << std::endl;
                return false;
            }

        }
        return true;
    }else {
        std::cout << node->getType() << std::endl;
        return false;
    }
    return true;
}



bool Checker::registerClass(std::string className, std::vector<std::string> *waiting) {
    std::map<std::string, std::string>::iterator parentIt = this->extend->find(className);
    if(parentIt == this->extend->end()) {
        //check if the parent class is define in the program
        std::cerr << "La class parente n'est pas definie" << std::endl;
        return false;
    }

    waiting->push_back(className);
    if (!this->symbolTable->hasScope(parentIt->second)) {
        //check if the parent class has already been
        for (int i = 0; i < waiting->size(); i++) {
            //check for cyclic definition
            if ((*waiting)[i] == parentIt->second) {
                std::cerr << "Definition cyclique" << std::endl;
                return false;
            }
        }
        if (!this->registerClass(parentIt->second, waiting)) {
            //try to create the scope of the parent class
            return false;
        }
    }

    this->symbolTable->enterNewScope(className, parentIt->second);
    waiting->pop_back();
    //create the scope for the current class under those of the parent one
    return true;
}


bool Checker::registerMethodAndField(ASTNode *node) {
    std::vector<ASTNode *> classChildren = node->getChildren();
    std::string className = classChildren[0]->getSValue();
    this->symbolTable->enterNewScope(className);

    for (int i = 2; i < classChildren.size(); i++) {
        std::vector<ASTNode *> children = classChildren[i]->getChildren();
        std::string name = children[0]->getSValue();

        if (classChildren[i]->getType() == "field") {
            if (this->symbolTable->lookup(name)) {
                std::cerr << "The field is already define" << std::endl;
                return false;
            }

            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Type is not define" << std::endl;
                return false;
            }

            this->symbolTable->add(name, className, children[1]->getSValue(), 0);

        } else if (classChildren[i]->getType() == "method") {
            if (this->symbolTable->lookup(name)) {
                std::cerr << "The method is already define" << std::endl;
                return false;
            }

            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Type is not define" << std::endl;
                return false;
            }

            this->symbolTable->add(name, className, children[1]->getSValue(), 0); //TODO: need to be adapt for method
        }
    }

    return true;
}