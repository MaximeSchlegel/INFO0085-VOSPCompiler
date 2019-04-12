#include "checker.h"


extern std::string filename;


Checker::Checker(ASTNode *root) {
    this->root = root;
    this->symbolTable = new SymbolTable();
    this->symbolTable->enterNewScope("int32");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("bool");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("string");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("unit");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("Object");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("IO");
    this->symbolTable->exitScope();

    this->extend = new std::map<std::string, std::string>();
}


bool Checker::check() {
    if(!this->scopeCheck(this->root)){
        return false;
    }
    if(!this->typeCheck(this->root)){
        return false;
    }
    return true;
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
        if (!this->symbolTable->hasScope(name)) {
            //check if the class is register
            if ( !this->registerClass(name, new std::vector<std::string>())) {
                return false;
            }
        }
        //continue the checking on the methods and fields
        this->symbolTable->enterNewScope(name);
        for(int i = 0; i < children.size() ; i++) {
            if (!this->scopeCheck(children[i])) {
                return false;
            }
        }
        this->symbolTable->exitScope();
        return true;

    } else if (node->getType() == "field") {
        true;

    } else if (node->getType() == "method") {


    } else if (node->getType() == "block") {

    } else if (node->getType() == "let") {

    } else if (node->getType() == "assign") {

    } else if (node->getType() == "call") {

    } else if (node->getType() == "new") {

    } else if (node->getType() == "objectid") {

    } else {
        //we just continue the scope checking on all the child (case of if while etc)
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
            //test if the field is assign;
            if (this->symbolTable->lookup(name)) {
                std::cerr << "The field is already define" << std::endl;
                return false;
            }
            //test if the type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Type is not define" << std::endl;
                return false;
            }
            //create the symbol in the table
            this->symbolTable->add(name, className, children[1]->getSValue(), 0);

        } else if (classChildren[i]->getType() == "method") {
            //check if the method is already define
            if (this->symbolTable->lookup(name)) {
                //TODO: check if the formals are the same
                std::cerr << "The method is already define" << std::endl;
                return false;
            }
            //check if the formals are define carrectly
            if (children.size() == 3){
                std::vector<std::string>usedName = std::vector<std::string>();
                std::vector<ASTNode>
                for (int )
            }

            //check if the return type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Type is not define" << std::endl;
                return false;
            }

            //create the method in scope
            this->symbolTable->add(name, className, children[1]->getSValue(), 0); //TODO: need to be adapt for method


        }
    }

    return true;
}


bool checkNode(ASTNode *node) {
    if (node->getType() == "program") {
        std::vector < ASTNode * > children = node->getChildren();
        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        // Check if program contains a class Main
        if(!this->symbolTable->hasClass("Main")) {
            std::cerr << "A program must contain a Main class" << std::endl;
            return false;
        }

        // Check if Main contains main()
        SymbolTableScope* mainScope = this->sumbolTable->getScope("Main");
        if(mainScope->lookup("main") == NULL) {
            std::cerr << "The Main class must have a method called main" << std::endl;
            return false;
        }
        return true;

    } else if (node->getType() == "class") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();
        
        //ENTER A NEW SCOPE
        this->symbolTable->getScope(name);

        for (int i = 2; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        //EXIT THE SCOPE
        this->symbolTable->exitScope();

        return true;

    } else if (node->getType() == "field") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        if (children.size() == 3) {
            // check the expression type
            if (!this->checkNode(children[2])) {
                return false;
            }

            //check that the return type match the expected one
            std::string rType = children->getReturnType();
            if (rType == "" || rType != this->symbolTable->lookup(name)->type) {
                std::cerr << "Type do not match" << std::endl;
                return false;
            }
        }
        return true;

    } else if (node->getType() == "method") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if the name is already used
        SymbolTableEntry* previous = this->symbolTable->lookupInCurrentScope(name);
        if(previous != NULL) {
            std::cerr << "Method already defined" << std::endl;
            return false; // Same name used inside the current scope
        }
        // Check if declared in parent scope
        SymbolTableEntry* superMethod = this->symbolTable->lookup(name);
        if(superMethod != NULL) {
            //Check prototype
            // TODO compare formals
            if(superMethod->type == children[1]->getSValue()) {
                return false;
            }
        }
        
        //ENTER A NEW SCOPE
        this->symbolTable->enterNewScope();

        // add formals
        if(children.size() == 4) {
            this->checkNode(children[3]);
        }

        // check block
        if (!this->checkNode(children[2])) {
            return false;
        }

        //EXIT THE SCOPE
        this->symbolTable->exitScope();

        return true;

    } else if (node->getType() == "formals") {
        std::vector < ASTNode * > children = node->getChildren();
        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        return true;

    } else if (node->getType() == "formal") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if already defined as formal name must be distinct
        SymbolTableEntry* formal = this->symbolTable->lookup(name);
        if(formal != NULL) {
            std::cerr << "Formals must have distinct names" << std::endl;
            return false;
        }

        // Add formal to current scope (new scope create in method)
        this->symbolTable->add(name, children[1]->getSValue());

        return true;

    } else if (node->getType() == "block") {
        //ENTER A NEW SCOPE
        this->symbolTable->enterNewScope();

        // Continue checking
        for (int i = 2; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        //EXIT THE SCOPE
        this->symbolTable->exitScope();

        return true;

    } else if (node->getType() == "if") {
        //ENTER A NEW SCOPE
        this->symbolTable->enterNewScope();

        // Continue checking
        for (int i = 2; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        //EXIT THE SCOPE
        this->symbolTable->exitScope();

        return true;

    } else if (node->getType() == "while") {
        //ENTER A NEW SCOPE
        this->symbolTable->enterNewScope();

        // Continue checking
        for (int i = 2; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

        //EXIT THE SCOPE
        this->symbolTable->exitScope();

        return true;

    } else if (node->getType() == "let") {
    } else if (node->getType() == "assign") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != children[0]->getReturnType()) {
            std::cerr << "Same type expected" << std::endl;
            return false;
        }
        node->setReturnType("bool");
    } else if (node->getType() == "not") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "bool") {
            std::cerr << "Type bool expected" << std::endl;
            return false;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "and") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "bool") {
            std::cerr << "Type bool expected" << std::endl;
        }
        if (children[1]->getReturnType() != "bool") {
            std::cerr << "Type bool expected" << std::endl;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "equal") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != children[1]->getReturnType()) {
            std::cerr << "Same type expected" << std::endl;
        }
        node->setReturnType(children[0]->getReturnType());

    } else if (node->getType() == "lower") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "lowerequal") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "plus") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "minus") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "times") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "div") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "pow") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "neg") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "isnull") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "call") {
    } else if (node->getType() == "new") {
    } else if (node->getType() == "args") {
        std:cout << "args" << std::endl;

    } else if (node->getType() == "bool") {
        node->setReturnType("bool");

    } else if (node->getType() == "int32") {
        node->setReturnType("int32");

    } else if (node->getType() == "unit") {
        node->setReturnType("unit");

    } else if (node->getType() == "typeid") {
        node->setReturnType(node->getSValue());

    } else if (node->getType() == "true") {
        node->setReturnType("bool");

    } else if (node->getType() == "false") {
        node->setReturnType("bool");

    } else if (node->getType() == "intliteral") {
        node->setReturnType("int32");

    } else if (node->getType() == "stringliteral") {
        node->setReturnType("string");

    } else if (node->getType() == "objectid") {
        std::string objectId = node->getSValue();
        //lockup to get the type of the symbol
        //if not define return flase
        //set the type to the returned type
    }else {
        std::cout << node->getType();
    }
    return true;
}