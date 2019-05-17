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
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("IO", "Object");
    this->symbolTable->exitScope();

    this->symbolTable->enterScope("IO");

    //create IO base functions formals
    SymbolTableEntry *formal;
    std::vector<SymbolTableEntry*> *formals;

    formal = new SymbolTableEntry("variableinput", "string");
    formals = new std::vector<SymbolTableEntry*>();
    formals->push_back(formal);
    this->symbolTable->add("methodprint", "IO", true, formals);

    formal = new SymbolTableEntry("variableinput", "bool");
    formals = new std::vector<SymbolTableEntry*>();
    formals->push_back(formal);
    this->symbolTable->add("methodprintBool", "IO", true, formals);

    formal = new SymbolTableEntry("variableinput", "int32");
    formals = new std::vector<SymbolTableEntry*>();
    formals->push_back(formal);
    this->symbolTable->add("methodprintInt32", "IO", true, formals);

    formals = new std::vector<SymbolTableEntry*>();
    this->symbolTable->add("methodinputLine", "string", true, formals);

    formals = new std::vector<SymbolTableEntry*>();
    this->symbolTable->add("methodinputBool", "bool", true, formals);

    formals = new std::vector<SymbolTableEntry*>();
    this->symbolTable->add("methodinputInt32", "int32", true, formals);

    this->symbolTable->exitScope("IO");
}

bool Checker::isChildOf(std::string subclass, std::string testClass) {
    if(subclass == testClass) {
        return true;
    }

    std::map<std::string, std::string>::iterator entry = this->extend->find(subclass);
    while (entry != this->extend->end()){
        if (entry->second == testClass) {
            return true;
        } else {
            entry = this->extend->find(entry->second);
        }
    }
    return false;
}

bool Checker::check() {
    if(!this->preprocess(this->root)){
        return false;
    }
//    std::cout << "Preprocess sucessful" << std::endl;

    if(!this->checkNode(this->root)){
        return false;
    }
//    std::cout << "scopeCheck sucessful" << std::endl;

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

                //check if we have already encounter this class
                if(this->extend->find(className) != this->extend->end()) {
                    // std::cerr << "Error line " << node->getLine() << ": The class" << className << "has been define several times" << std::endl;
                    throw CheckerException(node->getLine(), node->getColumn(), "The class" + className + "has been define several times");
                }

                //add (class, parent to extend
                this->extend->emplace(className, classParent);

            } else {
                // only class are expected under program node
                // std::cerr << "Error line " << node->getLine() << ": Unexpected expr" << node->getSValue() << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "Unexpected expr" + node->getSValue());
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
            //  std::cerr << "Can't find the Main class" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Can't find the Main class");
        }
        SymbolTableScope *main = this->symbolTable->getScope("Main");

//        std::cout << main << std::endl;

        SymbolTableEntry* mainMethod = main->lookup("methodmain");
        if (!mainMethod) {
            //  std::cerr << "Can't find Main::main method" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Can't find Main::main method");
        }
        if (mainMethod->getFormals()->size() != 0) {
            throw CheckerException(node->getLine(), node->getColumn(), "Main.main() should have no arguments");
        }
        if (mainMethod->getType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Main.main() should return int32");
        }

    } else if (node->getType() == "class") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        // check if the class is already define
        if (!this->symbolTable->hasClass(name)) {
            // check if the class is register
            if (!this->registerClass(name, new std::vector<std::string>(), node->getLine(), node->getColumn())) {
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



bool Checker::registerClass(std::string className, std::vector<std::string> *waiting, int line, int col) {
    std::map<std::string, std::string>::iterator classIt = this->extend->find(className);
    std::map<std::string, std::string>::iterator parentIt = this->extend->find(classIt->second);

    //check if the parent class is define in the program
    if(parentIt == this->extend->end()) {
        // std::cerr << "La class parente " << parentIt->second << " n'est pas definie" << std::endl;
        throw CheckerException(line, col, "La class parente " + classIt->second + " n'est pas definie");
    }

    waiting->push_back(className);

    // check if the parent if define in the symbol table
    if (!this->symbolTable->hasClass(classIt->second)) {
        // if the parent has not already been define
        // check for cyclic definition
        for (int i = 0; i < waiting->size(); i++) {
            if ((*waiting)[i] == classIt->second) {
                // std::cerr << "Definition cyclique" << std::endl;
                throw CheckerException(line, col, "Definition cyclique");
            }
        }

        // try to create the scope of the parent class
        if (!this->registerClass(classIt->second, waiting, line, col)) {
            return false;
        }
    }

    //create the scope for the current class under those of the parent one
    this->symbolTable->enterNewScope(className, classIt->second);
    this->symbolTable->exitScope();
    waiting->pop_back();
    return true;
}


bool Checker::registerMethodAndField(ASTNode *node) {
    std::vector<ASTNode *> classChildren = node->getChildren();
    std::string className = classChildren[0]->getSValue();

    std::vector<std::string> fieldsAndMethodsName;
    // for(int i = 2; i < classChildren.size(); i++) {
    //     ASTNode* name = classChildren[i]->getChildren()[0];
    //     fieldsAndMethodsName.push_back(name->getSValue());
    // }

    this->symbolTable->enterScope(className);

    for (int i = 2; i < classChildren.size(); i++) {
        std::vector<ASTNode *> children = classChildren[i]->getChildren();
        std::string name = children[0]->getSValue();

        fieldsAndMethodsName = this->symbolTable->getScope(className)->getNames();

        if (classChildren[i]->getType() == "field") {
            //test if the field is assign;
            if (this->symbolTable->lookupInCurrentScope("variable"+name)) {
                // std::cerr << "Error line " << node->getLine() << ": The field is already define" << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "The field is already define");
            }

            // CHeck if redefinition of inherited field
            if (this->symbolTable->lookup("variable"+name)) {
                throw CheckerException(node->getLine(), node->getColumn(), "Redefinition of inherited field " + name);
            }

            //test if the type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                // std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
            }

            //create the symbol in the table
            this->symbolTable->add("variable"+name, children[1]->getSValue());

            if(children.size() == 3) {
                // Has assign
                ASTNode* assignExpr = children[2];
                for(auto p: fieldsAndMethodsName) {
                    std::string checkName = p;

                    // Remove variable if present
                    size_t pos = checkName.find("variable");
                    if (pos != std::string::npos)
                    {
                        checkName = checkName.substr(8, checkName.size() - 8);
                    }

                    // Remove method if present
                    pos = checkName.find("method");
                    if (pos != std::string::npos)
                    {
                        checkName = checkName.substr(6, checkName.size() - 6);
                    }

                    if(assignExpr->doesSubTreeContains(checkName)) {
                        throw CheckerException(node->getLine(), node->getColumn(), "Cannot use class fields and methods in field initializer");
                    }
                }
            }

        } else if (classChildren[i]->getType() == "method") {
            //check if the method is already define in the class
            if (this->symbolTable->lookupInCurrentScope("method" + name)) {
                // std::cerr << "Error line " << node->getLine() << ": The method is already define" << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "The method is already define");
            }

            //check if the return type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end()) {
                // std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
            }

            std::vector<SymbolTableEntry*> *formals = new std::vector<SymbolTableEntry*>();

            //check if the formals are define correctly
            if (children.size() == 4){
//                std::cout << "      There is formal to register : " << std::endl;
                std::vector<std::string> *usedName = new std::vector<std::string>();
                std::vector<ASTNode *> methodFormals = children[3]->getChildren();

                for (int j = 0; j < methodFormals.size(); j++) {
                    std::vector<ASTNode *> formalNode = methodFormals[j]->getChildren();
                    std::string formalName = formalNode[0]->getSValue();
                    std::string formalType = formalNode[1]->getSValue();

                    if (formalName == "self") {
                        throw CheckerException(node->getLine(), node->getColumn(), "cannot use self as an argument name");
                    }

//                    std::cout << "          " << formalName << " : " << formalType << std::endl;

                    //check if the formal name is already used
                    for (int k = 0; k < usedName->size(); k++) {
                        if ((*usedName)[k] == formalName) {
                            // std::cerr << "Error line " << node->getLine() << " : Formal's name already used" << std::endl;
                            throw CheckerException(node->getLine(), node->getColumn(), "Formal's name already used");
                        }
                    }

                    //check if the type is valid
                    if (this->extend->find(formalType) == this->extend->end()) {
                        // std::cerr << "Error line " << node->getLine() << ": Type is not define" << std::endl;
                        throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
                    }

                    //create the formal and add it to the current formals
                    usedName->push_back(formalName);
                    SymbolTableEntry *formal = new SymbolTableEntry("variable" + formalName, formalType);
                    formals->push_back(formal);

//                    std::cout << "          " << "Formal added" << std::endl;
                }
            }

            //check if the method overwrite correctly
            SymbolTableEntry* overwrite = this->symbolTable->lookup("method" + name);
            if(overwrite != NULL) {
                //check if the return type match
                if (overwrite->getType() != children[1]->getSValue()) {
                    // std::cerr << "Error line " << node->getLine() << ": Return type does not match the overritten function" << std::endl;
                    throw CheckerException(node->getLine(), node->getColumn(), "Return type does not match the overritten function");
                }
                //check if the formals number match
                if (overwrite->getFormals()->size() != formals->size()) {
                    // std::cerr << "Error line " << node->getLine() << ": Formals do not match the overritten function" << std::endl;
                    throw CheckerException(node->getLine(), node->getColumn(), "Formals do not match the overritten function");
                }
                //check if the formals match
                std::vector<SymbolTableEntry*> *testedFormals = overwrite->getFormals();
                for (int i = 0; i < formals->size(); i++){
                    if((*testedFormals)[i]->getType() != (*formals)[i]->getType()) {
                        // std::cerr << "Error line " << node->getLine() << ": Formals do not match the overritten function" << std::endl;
                        throw CheckerException(node->getLine(), node->getColumn(), "Formals do not match the overritten function");
                    }
                }
            }

            //create the method in scope
            this->symbolTable->add("method"+name, children[1]->getSValue(), true, formals);//TODO: need to be adapt for method

        } else {
            // std::cerr << "Error line " << node->getLine() << ": Unexpetected expr " << node->getSValue() << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Unexpetected expr " + node->getSValue());
        }
    }

    this->symbolTable->exitScope();
    return true;
}


bool Checker::checkNode(ASTNode *node) {
//    std::cout << node->getType() << std::endl;

    if (node->getType() == "program") {
        std::vector < ASTNode * > children = node->getChildren();

        for (int i = 0; i < children.size(); i++) {
            if (!this->checkNode(children[i])) {
                return false;
            }
        }
        // Check if program contains a class Main
        if(!this->symbolTable->hasClass("Main")) {
            // std::cerr << "Error line " << node->getLine() << ": A program must contain a Main class" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "A program must contain a Main class");
        }
        // Check if Main contains main()
        this->symbolTable->enterScope("Main");
        if( this->symbolTable->lookup("methodmain") == NULL) {
            // std::cerr << "Error line " << node->getLine() << ": The Main class must have a method called main" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "The Main class must have a method called main");
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

        //check that the return type match the expected one
        std::string rType = children[1]->getSValue();
        std::string eType;

        if(children.size() == 2) {
            // No assignment
            eType = this->symbolTable->lookup("variable"+name)->getType();
        } else {
            // With assignment
            if (!this->checkNode(children[2])) {
                return false;
            }
            eType = children[2]->getReturnType();
        }

        if (rType != eType) {
            // std::cerr << "Error line " << node->getLine() << ": Type do not match" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Type do not match");
        }

    } else if (node->getType() == "method") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if the name is already used => sera tjs vrai
        SymbolTableEntry* previous = this->symbolTable->lookupInCurrentScope(name);
        if(previous != NULL) {
            // std::cerr << "Error line " << node->getLine() << ": Method already defined" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Method already defined"); // Same name used inside the current scope
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
        if (!this->isChildOf(children[1]->getSValue(), children[2]->getReturnType()) && !this->isChildOf(children[2]->getReturnType(), children[1]->getSValue())) {
            throw CheckerException(node->getLine(), node->getColumn(), "Return type of the block does not match declared type of the method");
        }
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
            // std::cerr << "Error line " << node->getLine() << ": Formals must have distinct names" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Formals must have distinct names");
        }
        // Add formal to current scope (new scope create in method)
        this->symbolTable->add("variable"+name, children[1]->getSValue());

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
            // std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }
        //check that the 2 other expression have the same type
        std::string exprT = children[1]->getReturnType();
        std::string exprE;
        if (children.size() == 3) {
            exprE = children[2]->getReturnType();
        }
        else {
            exprE = "unit";
        }

        if (exprT != "unit" && exprE != "unit") {
            if (exprT != exprE) { // Not same type
                throw CheckerException(node->getLine(), node->getColumn(), "Types do not match expected");
            }
            node->setReturnType(exprT); // Same type (exactly) MUST add check of parents...
        }
        else { // Both unit
            node->setReturnType("unit");
        }

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
            // std::cerr << "Error line " << node->getLine() << ": Type bool expected" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }
        node->setReturnType("unit");

    } else if (node->getType() == "let") {
        std::vector<ASTNode *> children = node->getChildren();

        if (children.size() == 4){
            // Evaluate assign
            if (!this->checkNode(children[2])) {
                return false;
            }

            // Evaluate expr after IN
            this->symbolTable->enterNewScope();
            std::string name = children[0]->getSValue();

            if(name == "self") {
                throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
            }

            this->symbolTable->add("variable"+name, children[1]->getSValue());

            if (!this->checkNode(children[3])) {
                return false;
            }
            this->symbolTable->exitScope();

            // Check matching types
            // ? Do we need to check ? A let has the value of its body
            // ? the type is only for the variable used inside this body
            // std::string rType = children[1]->getSValue();
            // std::string eType = children[3]->getReturnType();

            // std::cout << rType << " / " << eType << std::endl;

            // if (rType != eType){
            //     std::cerr << "Error line " << node->getLine() << ": Type does not match" << std::endl;
            //     return false;
            // }
        } else {
            // Evaluate expr after IN
            this->symbolTable->enterNewScope();
            std::string name = children[0]->getSValue();

            if(name == "self") {
                throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
            }

            this->symbolTable->add("variable"+name, children[1]->getSValue());

            if (!this->checkNode(children[2])) {
                return false;
            }

            this->symbolTable->exitScope();

            // Check matching types
            std::string rType = children[1]->getSValue();
            std::string eType = children[2]->getReturnType();

            if (rType != eType && !this->isChildOf(rType, eType)){
                throw CheckerException(node->getLine(), node->getColumn(), "Type does not match");
            }
        }

        if(children.size() == 4) {
            node->setReturnType(children[3]->getReturnType());
        } else {
            node->setReturnType(children[2]->getReturnType());
        }

    } else if (node->getType() == "assign") {
        std::vector < ASTNode * > children = node->getChildren();
        std::string name = children[0]->getSValue();

        if(name == "self") {
            throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
        }

        this->symbolTable->enterNewScope();

        if (!this->checkNode(children[1])) {
            return false;
        }

        this->symbolTable->exitScope();

        SymbolTableEntry* identifier = this->symbolTable->lookup("variable"+name);

        // Undefined identifier
        if(identifier == NULL) {
            throw CheckerException(node->getLine(), node->getColumn(), "Use of unboud variable " + name);
        }

        if (identifier->getType() != children[1]->getReturnType() &&
            !this->isChildOf(children[1]->getType(), identifier->getType())) {
            throw CheckerException(node->getLine(), node->getColumn(), "Same type expected");
        }

        node->setReturnType(children[1]->getReturnType());

    } else if (node->getType() == "not") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "bool") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }
        if (children[1]->getReturnType() != "bool") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Same type expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
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
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        node->setReturnType("int32");

    } else if (node->getType() == "neg") {
        std::vector < ASTNode * > children = node->getChildren();
        if (!this->checkNode(children[0])) {
            return false;
        }
        if (children[0]->getReturnType() != "int32") {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        node->setReturnType("int32");

    } else if (node->getType() == "isnull") {
        std::vector < ASTNode * > children = node->getChildren();

        if (!this->checkNode(children[0])) {
            return false;
        }

        if(!this->isChildOf(children[0]->getReturnType(), "Object")) {
            throw CheckerException(node->getLine(), node->getColumn(), "Type Object expected");
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

        //check the caller
        if (!this->checkNode(children[0])) {
            return false;
        }

        std::string objectType = children[0]->getReturnType();
        // SymbolTableScope *classScope = this->symbolTable->getScope(objectType);
        this->symbolTable->enterScope(objectType);
        std::string methodName = children[1]->getSValue();

        //check if the method if declared
        SymbolTableEntry* method = this->symbolTable->lookup("method"+methodName);

        this->symbolTable->exitScope(objectType);

        if (method == nullptr) {
            // std::cerr << "Error line " << node->getLine() << ": Method does not exist" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Method does not exist");
        }

        //check some args are expected
        std::vector<SymbolTableEntry*>* formals = method->getFormals();
        if (formals->size() > 0 && children.size() != 3) {
            // std::cerr << "Error line " << node->getLine() << ": Invalid number of args" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
        }

        //if there is args
        if (children.size() == 3) {
            std::vector<ASTNode *> args = children[2]->getChildren();

            //check that the formals expected some args
            if (formals->size() == 0) {
                // std::cerr << "Error line " << node->getLine() << ": Invalid number of args" << std::endl;
                throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
            }

            //check the number of args
            if (args.size() != formals->size()) {
                throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
            }

            //check that the args are compatible with the formals
            for (int i = 0; i < args.size(); i++) {
                //check the type of the formals
                this->symbolTable->enterNewScope();
                if (!this->checkNode(args[i])) {
                    return false;
                }
                this->symbolTable->exitScope();


                if ((*formals)[i]->getType() != args[i]->getReturnType() &&
                    !this->isChildOf(args[i]->getReturnType(), (*formals)[i]->getType())) {
                    throw CheckerException(node->getLine(), node->getColumn(), "Type does not match");
                }
            }
        }

        //if the call have n args but the method need them
        if (method->getFormals() == NULL) {
            throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
        }
        node->setReturnType(method->getType());

    } else if (node->getType() == "args") {
        std::cout << "  args " << node->getSValue() << std::endl;

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
        SymbolTableEntry* entry = this->symbolTable->lookup("variable"+objectId);
        if (entry == NULL) {
            // std::cerr << "Error line " << node->getLine() << ": Object Id \""<< objectId <<"\" does not exist" << std::endl;
            throw CheckerException(node->getLine(), node->getColumn(), "Object Id \"" + objectId + "\" does not exist");
        }
        node->setReturnType(entry->getType());

    }else {
        std::cout << "Hello  ";
        std::cout << node->getType();
    }
    return true;
}
