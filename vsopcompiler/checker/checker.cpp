#include "checker.h"


extern std::string filename;


Checker::Checker(ASTNode *root) {
    this->root = root;
    this->symbolTable = new SymbolTable();

    this->extend = new std::map<std::string, std::string>();

    this->extend->emplace("int32", "");
    this->extend->emplace("bool", "");
    this->extend->emplace("string", "");
    this->extend->emplace("unit", "");
    this->extend->emplace("Object", "");
    this->extend->emplace("IO", "Object");

    this->symbolTable->enterNewScope("Object", "");
}


bool Checker::check() {
    if(!this->preprocess(this->root)){
        return false;
    }
    std::cout << "Preprocess sucessful" << std::endl;

    if(!this->checkNode(this->root)){
        return false;
    }
    std::cout << "scopeCheck sucessful" << std::endl;

    return true;
}


bool Checker::preprocess(ASTNode *node) {
    if (node->getType() == "program") {
        std::vector<ASTNode *> children = node->getChildren();

        //register all the declared classes and their parent
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->getType() == "class") {
                std::string className = (children[i]->getChildren())[0]->getSValue();
                std::string classParent = (children[i]->getChildren())[1]->getSValue();

                std::cout << className << " ; " << classParent << std::endl;

                //check if we have already encounter this class
                if(this->extend->find(className) != this->extend->end()) {
                    std::cerr << "Error line " << node->getLine() << ": The class" << className << "has been define several times" << std::endl;
                    return false;
                }

                //add (class, parent to extend
                this->extend->emplace(className, classParent);

            } else {
                // only class are expected under program node
                std::cerr << "Error line " << node->getLine() << ": Unexpected expr" << node->getSValue() << std::endl;
                return false;
            }
        }

        //continue the scope checking in each class
        for(int i = 0; i < children.size() ; i++) {
            if (!this->preprocess(children[i])) {
                return false;
            }
        }

        //check if there is a Main::main
        if (this->extend->find("Main") == this->extend->end()) {
             std::cerr << "Can't find the Main class" << std::endl;
             return false;
        }
        SymbolTableScope *main = this->symbolTable->getScope("Main");

        std::cout << main << std::endl;

        if (!main->lookup("methodmain")) {
             std::cerr << "Can't find Main::main method" << std::endl;
             return false;
        }

    } else if (node->getType() == "class") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        std::cout << "New class: " << name << std::endl;

        // check if the class is already define
        if (!this->symbolTable->hasClass(name)) {
            // check if the class is register
            if (!this->registerClass(name, new std::vector<std::string>())) {
                return false;
            }
        }

        //register the methods and fields of the method
        if(!this->registerMethodAndField(node)) {
            return false;
        }
    }
    return true;
}



bool Checker::registerClass(std::string className, std::vector<std::string> *waiting) {
    std::map<std::string, std::string>::iterator parentIt = this->extend->find(className);

    //check if the parent class is define in the program
    if(parentIt == this->extend->end()) {
        std::cerr << "La class parente " << parentIt->second << " n'est pas definie" << std::endl;
        return false;
    }

    waiting->push_back(className);

    // check if the parent if define in the symbol table
    if (!this->symbolTable->hasClass(parentIt->second)) {
        // if the parent has not already been define
        // check for cyclic definition
        for (int i = 0; i < waiting->size(); i++) {
            if ((*waiting)[i] == parentIt->second) {
                std::cerr << "Definition cyclique" << std::endl;
                return false;
            }
        }

        // try to create the scope of the parent class
        if (!this->registerClass(parentIt->second, waiting)) {
            return false;
        }
    }

    //create the scope for the current class under those of the parent one
    this->symbolTable->enterNewScope(className, parentIt->second);
    this->symbolTable->exitScope();
    waiting->pop_back();
    return true;
}


bool Checker::registerMethodAndField(ASTNode *node) {
    std::vector<ASTNode *> classChildren = node->getChildren();
    std::string className = classChildren[0]->getSValue();

    this->symbolTable->enterScope(className);

    for (int i = 2; i < classChildren.size(); i++) {
        std::vector<ASTNode *> children = classChildren[i]->getChildren();
        std::string name = children[0]->getSValue();

        std::cout << "  " << name << std::endl;

        if (classChildren[i]->getType() == "field") {
            //test if the field is assign;
            if (this->symbolTable->lookupInCurrentScope("variable"+name)) {
                std::cerr << "Error line " << node->getLine() << ": The field is already define" << std::endl;
                return false;
            }

            //test if the type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                return false;
            }

            //create the symbol in the table
            this->symbolTable->add("variable"+name, children[1]->getSValue());

        } else if (classChildren[i]->getType() == "method") {
            //check if the method is already define in the class
            if (this->symbolTable->lookup("method" + name)) {
                std::cerr << "Error line " << node->getLine() << ": The method is already define" << std::endl;
                return false;
            }

            //check if the return type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                return false;
            }

            std::vector<SymbolTableEntry*> *formals = new std::vector<SymbolTableEntry*>();

            //check if the formals are define correctly
            if (children.size() == 4){
                std::cout << "      There is formal to register : " << std::endl;
                std::vector<std::string> *usedName = new std::vector<std::string>();
                std::vector<ASTNode *> methodFormals = children[3]->getChildren();

                for (int j = 1; j < methodFormals.size(); j++) {
                    std::vector<ASTNode *> formalNode = methodFormals[j]->getChildren();
                    std::string formalName = formalNode[0]->getSValue();
                    std::string formalType = formalNode[1]->getSValue();

                    std::cout << "          " << formalName << " : " << formalType << std::endl;

                    //check if the formal name is already used
                    for (int k = 0; k < usedName->size(); k++) {
                        if ((*usedName)[k] == formalName) {
                            std::cerr << "Error line " << node->getLine() << " : Formal's name already used" << std::endl;
                            return false;
                        }
                    }

                    //check if the type is valid
                    if (this->extend->find(formalType) == this->extend->end()) {
                        std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                        return false;
                    }

                    //create the formal and add it to the current formals
                    usedName->push_back(formalName);
                    SymbolTableEntry *formal = new SymbolTableEntry("variable" + formalName, formalType);
                    formals->push_back(formal);

                    std::cout << "          " << "Formal added" << std::endl;
                }
            }

            //TODO: check if it is a valid overwrite

            //create the method in scope
            this->symbolTable->add("method"+name, children[1]->getSValue(), true, formals);//TODO: need to be adapt for method

        } else {
            std::cerr << "Error 1 line " << node->getLine() << ": Unexpetected expr " << node->getSValue() << std::endl;
            return false;
        }
    }

    this->symbolTable->exitScope();
    return true;
}


bool Checker::checkNode(ASTNode *node) {
    std::cout << node->getType() << std::endl;

    if (node->getType() == "program") {
        std::vector < ASTNode * > children = node->getChildren();
        
        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }
        // Check if program contains a class Main
        if(!this->symbolTable->hasClass("Main")) {
            std::cerr << "Error line " << node->getLine() << ": A program must contain a Main class" << std::endl;
            return false;
        }
        // Check if Main contains main()
        this->symbolTable->enterScope("Main");
        if( this->symbolTable->lookup("methodmain") == NULL) {
            std::cerr << "Error line " << node->getLine() << ": The Main class must have a method called main" << std::endl;
            return false;
        }
        this->symbolTable->exitScope("Main");

    } else if (node->getType() == "class") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        this->symbolTable->enterScope(name);

        for (int i = 2; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }
        this->symbolTable->exitScope(name);

    } else if (node->getType() == "field") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        if (children.size() == 3) {
            // check the expression
            if (!this->checkNode(children[2])) {
                return false;
            }
            
            //check that the return type match the expected one
            std::string rType = children[1]->getSValue();
            std::string eType = this->symbolTable->lookup("variable"+name)->getType();
            
            if (rType != eType) {
                std::cerr << "Error line " << node->getLine() << ": Type do not match" << std::endl;
                return false;
            }
        }

    } else if (node->getType() == "method") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if the name is already used => sera tjs vrai
        SymbolTableEntry* previous = this->symbolTable->lookupInCurrentScope(name);
        if(previous != NULL) {
            std::cerr << "Error line " << node->getLine() << ": Method already defined" << std::endl;
            return false; // Same name used inside the current scope
        }
        // Check if declared in parent scope => sert à rien fait dans le preprocess
        SymbolTableEntry* superMethod = this->symbolTable->lookup(name);
        if(superMethod != NULL) {
            //Check prototype
            if(superMethod->getType() == children[1]->getSValue()) {
                return false;
            }
        }
        this->symbolTable->enterNewScope();
        //add formals
        if(children.size() == 4) {
            if(!this->checkNode(children[3])) {
                return false;
            }
        }
        //check the method_body
        if (!this->checkNode(children[2])) {
            return false;
        }
        this->symbolTable->exitScope();
        //check the return type
        // if (children[0]->getSValue() != children[2]->getReturnType()) {
        //     std::cerr << "Types do not match" << std::endl;
        //     return false;
        // }
        node->setReturnType(children[0]->getSValue());

    } else if (node->getType() == "formals") {
        std::vector < ASTNode * > children = node->getChildren();
        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }

    } else if (node->getType() == "formal") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();
        // Check if already defined as formal name must be distinct
        SymbolTableEntry* formal = this->symbolTable->lookup(name);
        if(formal != NULL) {
            std::cerr << "Error line " << node->getLine() << ": Formals must have distinct names" << std::endl;
            return false;
        }
        // Add formal to current scope (new scope create in method)
        this->symbolTable->add(name, children[1]->getSValue());

    } else if (node->getType() == "block") {
        std::vector < ASTNode * > children = node->getChildren();
        this->symbolTable->enterNewScope();

        // Continue checking
        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }
        this->symbolTable->exitScope();
        node->setReturnType(children[children.size() - 1]->getReturnType());

    } else if (node->getType() == "if") {
        std::vector < ASTNode * > children = node->getChildren();
        // Continue checking
        for (int i = 0; i < children.size(); i++) {
            this->symbolTable->enterNewScope();
            if (!this->checkNode(children[i])) {
                return false;
            }
            this->symbolTable->exitScope();
        }
        //check that the first exp is of type bool
        if (children[0]->getReturnType() != "bool") {
            std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
            return false;
        }
        //check that the 2 other expression have the same type
        if (children.size() == 3) {
            if (children[1]->getReturnType() != children[2]->getReturnType()) {
                std::cerr << "Error line " << node->getLine() << ": Types do not match expected" << std::endl;
                return false;
            }
        }
        node->setReturnType(children[1]->getReturnType());

    } else if (node->getType() == "while") {
        std::vector < ASTNode * > children = node->getChildren();
        //continue checking
        for (int i = 0; i < children.size(); i++) {
            this->symbolTable->enterNewScope();
            if (!this->checkNode(children[i])) {
                return false;
            }
            this->symbolTable->exitScope();
        }
        //check that the first exp is of type bool
        if (children[0]->getReturnType() != "bool") {
            std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
            return false;
        }
        node->setReturnType(children[1]->getReturnType());

    } else if (node->getType() == "let") {
        std::vector<ASTNode *> children = node->getChildren();
        
        if (children.size() == 4){
            // Evaluate assign
            if (!this->checkNode(children[2])) {
                return false;
            }

            // Evaluate expr after IN
            this->symbolTable->enterNewScope();

            this->symbolTable->add(children[0]->getSValue(), children[2]->getReturnType());

            if (!this->checkNode(children[3])) {
                return false;
            }
            this->symbolTable->exitScope();

            // Check matching types
            std::string rType = children[1]->getSValue();
            std::string eType = children[3]->getReturnType();

            if (rType != eType){
                std::cerr << "Error line " << node->getLine() << ": Type does not match" << std::endl;
                return false;
            }
        } else {
            // Evaluate expr after IN
            if (!this->checkNode(children[2])) {
                return false;
            }

            // Check matching types
            std::string rType = children[1]->getSValue();
            std::string eType = children[2]->getReturnType();

            if (rType != eType){
                std::cerr << "Error line " << node->getLine() << ": Type does not match" << std::endl;
                return false;
            }
        }
        
        if(children.size() == 4) {
            node->setReturnType(children[3]->getReturnType());
        } else {
            node->setReturnType(children[2]->getReturnType());
        }

    } else if (node->getType() == "assign") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        this->symbolTable->enterNewScope();
        if (!this->checkNode(children[1])) {
            return false;
        }
        this->symbolTable->exitScope();
        if (this->symbolTable->lookup("variable"+(children[0]->getSValue()))->getType() != children[1]->getReturnType()) {
            std::cerr << "Error line " << node->getLine() << ": Same type expected" << std::endl;
            return false;
        }
        node->setReturnType(children[1]->getReturnType());

    } else if (node->getType() == "not") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "bool") {
            std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
        }
        if (children[1]->getReturnType() != "bool") {
            std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Same type expected" << std::endl;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "lower") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "lowerequal") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "plus") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (!this->checkNode(children[1])) {
            return false;
        }

        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
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
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        if (children[1]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "neg") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            std::cerr << "Error line " << node->getLine() << ": Type int32 expected" << std::endl;
        }
        node->setReturnType("int32");

    } else if (node->getType() == "isnull") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        node->setReturnType("bool");

    } else if (node->getType() == "new") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        node->setReturnType(children[0]->getReturnType());
    } else if (node->getType() == "call") {
        std::vector < ASTNode * > children = node->getChildren();

        if (!this->checkNode(children[0])) {
            return false;
        }

        std::string name = children[0]->getSValue();

        SymbolTableScope *classScope = this->symbolTable->getScope(name);

        std::cout << "  4" << std::endl;
        std::cout << classScope << std::endl;

        SymbolTableEntry* method = classScope->lookup("method"+name);
        //check if the method if declared

        std::cout << "  5" << std::endl;

        if (method == nullptr) {
            std::cerr << "Error line " << node->getLine() << ": Method does not exist" << std::endl;
            return false;
        }
        //if their is args
        if (children.size() == 3) {
            std::vector<ASTNode *> args = children[2]->getChildren();
            std::vector<SymbolTableEntry*>* formals = method->getFormals();
            std::vector<std::string> usedName = std::vector<std::string>();
            if (formals->size() == 0) {
                std::cerr << "Error line " << node->getLine() << ": Invalid number of args" << std::endl;
                return false;
            }
            //check the number of args
            if (args.size() != formals->size()) {
                std::cerr << "Error line " << node->getLine() << ": Invalid number of args" << std::endl;
                return false;
            }
            for (int i = 1; i < args.size(); i++) {
                //check the type of the formals
                this->symbolTable->enterNewScope();
                if (!this->checkNode(args[i])) {
                    return false;
                }
                this->symbolTable->exitScope();
                if ((*formals)[i]->getType() != args[i]->getReturnType()) {
                    std::cerr << "Error line " << node->getLine() << ": Type does not match" << std::endl;
                    return false;
                }
            }
        }
        //if the call have n args but the method need them
        if (method->getFormals() == NULL) {
            std::cerr << "Error line " << node->getLine() << ": Invalid number of args" << std::endl;
            return false;
        }
        node->setReturnType(method->getType());

    } else if (node->getType() == "args") {

        std::cout << "args" << std::endl;

    } else if (node->getType() == "bool") {
        node->setReturnType("bool");

    } else if (node->getType() == "int32") {
        node->setReturnType("int32");

    } else if (node->getType() == "unit") {
        node->setReturnType("unit");

    } else if (node->getType() == "typeid") {
        node->setReturnType(node->getSValue());
        //TODO: scope check

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
        SymbolTableEntry* entry = this->symbolTable->lookup(objectId);
        if (entry == NULL) {
            std::cerr << "Error line " << node->getLine() << ": Object Id \""<< objectId <<"\" does not exist" << std::endl;
            return false;
        }
        node->setReturnType(entry->getType());

    }else {

        std::cout << node->getType();
    }
    return true;
}