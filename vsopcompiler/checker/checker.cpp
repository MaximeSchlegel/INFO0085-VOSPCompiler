#include "checker.h"

extern std::string filename;

Checker::Checker(ASTNode *root)
{
    this->root = root;
    this->symbolTable = new SymbolTable();

    this->extend = new std::map<std::string, std::string>();

    // Add the native types to the extend table
    this->extend->emplace("int32", "");
    this->extend->emplace("bool", "");
    this->extend->emplace("string", "");
    this->extend->emplace("unit", "");
    this->extend->emplace("Object", "");
    this->extend->emplace("IO", "Object");

    // Add the IO and Oject scoop to the symbol table
    this->symbolTable->enterNewScope("Object", "");
    this->symbolTable->exitScope();
    this->symbolTable->enterNewScope("IO", "Object");
    this->symbolTable->exitScope();

    // Create IO base functions
    SymbolTableEntry *formal;
    std::vector<SymbolTableEntry *> *formals;
    this->symbolTable->enterScope("IO");

    formal = new SymbolTableEntry("variableinput", "string");
    formals = new std::vector<SymbolTableEntry *>();
    formals->push_back(formal);
    this->symbolTable->add("methodprint", "IO", true, formals);

    formal = new SymbolTableEntry("variableinput", "bool");
    formals = new std::vector<SymbolTableEntry *>();
    formals->push_back(formal);
    this->symbolTable->add("methodprintBool", "IO", true, formals);

    formal = new SymbolTableEntry("variableinput", "int32");
    formals = new std::vector<SymbolTableEntry *>();
    formals->push_back(formal);
    this->symbolTable->add("methodprintInt32", "IO", true, formals);

    formals = new std::vector<SymbolTableEntry *>();
    this->symbolTable->add("methodinputLine", "string", true, formals);

    formals = new std::vector<SymbolTableEntry *>();
    this->symbolTable->add("methodinputBool", "bool", true, formals);

    formals = new std::vector<SymbolTableEntry *>();
    this->symbolTable->add("methodinputInt32", "int32", true, formals);

    this->symbolTable->exitScope("IO");
}

bool Checker::isChildOf(std::string subclass, std::string testClass)
{
    if (subclass == testClass)
    {
        return true;
    }

    std::map<std::string, std::string>::iterator entry = this->extend->find(subclass);
    while (entry != this->extend->end())
    {
        if (entry->second == testClass)
        {
            return true;
        }
        else
        {
            entry = this->extend->find(entry->second);
        }
    }
    return false;
}

std::string Checker::getFirstCommonAncestor(std::string c1, std::string c2)
{
    if (this->isChildOf(c1, c2))
    {
        return c2;
    }
    std::string parent = this->extend->at(c2);
    return this->getFirstCommonAncestor(c1, parent);
}

void Checker::check()
{
    this->preprocess(this->root);
    this->checkNode(this->root);
}

void Checker::preprocess(ASTNode *node)
{

    if (node->getType() == "program")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Register all the declared classes and their parent
        for (int i = 0; i < children.size(); i++)
        {
            if (children[i]->getType() == "class")
            {
                std::string className = (children[i]->getChildren())[0]->getSValue();
                std::string classParent = (children[i]->getChildren())[1]->getSValue();

                // Check if we have already encounter this class
                if (this->extend->find(className) != this->extend->end())
                {
                    throw CheckerException(node->getLine(), node->getColumn(), "The class" + className + "has been define several times");
                }

                // Add (class, parent) to extend
                this->extend->emplace(className, classParent);
            }
            else
            {
                // Only class are expected under program node
                throw CheckerException(node->getLine(), node->getColumn(), "Unexpected expr" + node->getSValue());
            }
        }

        // Continue the preprocess in each class to register their method and fields
        for (int i = 0; i < children.size(); i++)
        {
            this->preprocess(children[i]);
        }

        // Check if there is a Main class
        if (this->extend->find("Main") == this->extend->end())
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Can't find the Main class");
        }
        SymbolTableScope *main = this->symbolTable->getScope("Main");

        // Check if there is a Main::main method to launch the program
        SymbolTableEntry *mainMethod = main->lookup("methodmain");
        if (!mainMethod)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Can't find Main::main method");
        }
        // Check if the Main::main formals are correct : none expected
        if (mainMethod->getFormals()->size() != 0)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Main.main() should have no arguments");
        }
        // Check if the output of Main::main is correct : expected int32
        if (mainMethod->getType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Main.main() should return int32");
        }
    }
    else if (node->getType() == "class")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if the class is already define in the symbol table
        if (!this->symbolTable->hasClass(name))
        {
            // Try to register this class
            this->registerClass(name, new std::vector<std::string>(), node->getLine(), node->getColumn());
        }

        // Register the methods and fields of the method
        this->registerMethodAndField(node);
    }
}

void Checker::registerClass(std::string className, std::vector<std::string> *waiting, int line, int col)
{

    std::map<std::string, std::string>::iterator classIt = this->extend->find(className);
    std::map<std::string, std::string>::iterator parentIt = this->extend->find(classIt->second);

    // Check if the parent class is define in the program
    if (parentIt == this->extend->end())
    {
        throw CheckerException(line, col, "La class parente " + classIt->second + " n'est pas definie");
    }

    // Add the current class to the classes needed to be define
    waiting->push_back(className);

    // Check if the parent if define in the symbol table
    if (!this->symbolTable->hasClass(classIt->second))
    {
        // If the parent has not already been define :
        // First check for cyclic definition
        for (int i = 0; i < waiting->size(); i++)
        {
            if ((*waiting)[i] == classIt->second)
            {
                throw CheckerException(line, col, "Definition cyclique");
            }
        }

        // Then try to create the parent class
        this->registerClass(classIt->second, waiting, line, col);
    }

    // Create the scope for the current class under those of the parent one
    this->symbolTable->enterNewScope(className, classIt->second);
    this->symbolTable->exitScope();
    waiting->pop_back();
}

void Checker::registerMethodAndField(ASTNode *node)
{
    std::vector<ASTNode *> classChildren = node->getChildren();
    std::string className = classChildren[0]->getSValue();

    std::vector<std::string> fieldsAndMethodsName;

    // Enter the scope of the class
    this->symbolTable->enterScope(className);

    // Test end register each child of the class
    for (int i = 2; i < classChildren.size(); i++)
    {
        std::vector<ASTNode *> children = classChildren[i]->getChildren();
        std::string name = children[0]->getSValue();

        fieldsAndMethodsName = this->symbolTable->getScope(className)->getNames();

        if (classChildren[i]->getType() == "field")
        {
            // Test if the field is assign;
            if (this->symbolTable->lookupInCurrentScope("variable" + name))
            {
                throw CheckerException(node->getLine(), node->getColumn(), "The field is already define");
            }

            // Check if redefinition of inherited field
            if (this->symbolTable->lookup("variable" + name))
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Redefinition of inherited field " + name);
            }

            // Test if the type is valid
            if (this->extend->find(children[1]->getSValue()) == this->extend->end())
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
            }

            // Create the symbol in the table
            this->symbolTable->add("variable" + name, children[1]->getSValue());

            if (children.size() == 3)
            {
                // The field is assign need to check it
                ASTNode *assignExpr = children[2];
                for (auto p : fieldsAndMethodsName)
                {
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

                    // Check if field or method are used in the field definition
                    if (assignExpr->doesSubTreeContains(checkName))
                    {
                        throw CheckerException(node->getLine(), node->getColumn(), "Cannot use class fields and methods in field initializer");
                    }
                }
            }
        }
        else if (classChildren[i]->getType() == "method")
        {
            // Check if the method is already define in the class
            if (this->symbolTable->lookupInCurrentScope("method" + name))
            {
                throw CheckerException(node->getLine(), node->getColumn(), "The method is already define");
            }

            // Check if the return type is valid (-> define in the program)
            if (this->extend->find(children[1]->getSValue()) == this->extend->end())
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
            }

            std::vector<SymbolTableEntry *> *formals = new std::vector<SymbolTableEntry *>();

            if (children.size() == 4)
            {
                // There is formals
                // Check if the formals are define correctly

                std::vector<std::string> *usedName = new std::vector<std::string>();
                std::vector<ASTNode *> methodFormals = children[3]->getChildren();

                for (int j = 0; j < methodFormals.size(); j++)
                {
                    std::vector<ASTNode *> formalNode = methodFormals[j]->getChildren();
                    std::string formalName = formalNode[0]->getSValue();
                    std::string formalType = formalNode[1]->getSValue();

                    // Check that self is not used has the name of the formals => reserved in vsop
                    if (formalName == "self")
                    {
                        throw CheckerException(node->getLine(), node->getColumn(), "cannot use self as an argument name");
                    }

                    // Check if the formal name is already used
                    for (int k = 0; k < usedName->size(); k++)
                    {
                        if ((*usedName)[k] == formalName)
                        {
                            throw CheckerException(node->getLine(), node->getColumn(), "Formal's name already used");
                        }
                    }

                    // Check if the formal's type is valid
                    if (this->extend->find(formalType) == this->extend->end())
                    {
                        throw CheckerException(node->getLine(), node->getColumn(), "Type is not define");
                    }

                    // Create the formal and add it to the current formals
                    usedName->push_back(formalName);
                    SymbolTableEntry *formal = new SymbolTableEntry("variable" + formalName, formalType);
                    formals->push_back(formal);
                }
            }

            // Check if the method overwrite correctly
            SymbolTableEntry *overwrite = this->symbolTable->lookup("method" + name);
            if (overwrite != NULL)
            {
                // Check if the return type match
                if (overwrite->getType() != children[1]->getSValue())
                {
                    throw CheckerException(node->getLine(), node->getColumn(), "Return type does not match the overritten function");
                }

                // Check if the formals number match
                if (overwrite->getFormals()->size() != formals->size())
                {
                    throw CheckerException(node->getLine(), node->getColumn(), "Formals do not match the overritten function");
                }

                // Check if the each formal match the one of the overwrite method
                std::vector<SymbolTableEntry *> *testedFormals = overwrite->getFormals();
                for (int i = 0; i < formals->size(); i++)
                {
                    if ((*testedFormals)[i]->getType() != (*formals)[i]->getType())
                    {
                        throw CheckerException(node->getLine(), node->getColumn(), "Formals do not match the overritten function");
                    }
                }
            }

            // Create the method in scope
            this->symbolTable->add("method" + name, children[1]->getSValue(), true, formals);
        }
        else
        {
            // We only expect method or field directly under the class
            throw CheckerException(node->getLine(), node->getColumn(), "Unexpetected expr " + node->getSValue());
        }
    }

    this->symbolTable->exitScope();
}

void Checker::checkNode(ASTNode *node)
{

    if (node->getType() == "program")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check each class
        for (int i = 0; i < children.size(); i++)
        {
            this->checkNode(children[i]);
        }

        // Check if program contains a class Main
        if (!this->symbolTable->hasClass("Main"))
        {
            throw CheckerException(node->getLine(), node->getColumn(), "A program must contain a Main class");
        }

        // Check if Main contains main()
        this->symbolTable->enterScope("Main");
        if (this->symbolTable->lookup("methodmain") == NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "The Main class must have a method called main");
        }
        this->symbolTable->exitScope("Main");
    }
    else if (node->getType() == "class")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        this->symbolTable->enterScope(name);

        // Check each child
        for (int i = 2; i < children.size(); i++)
        {
            this->checkNode(children[i]);
        }
        this->symbolTable->exitScope(name);
    }
    else if (node->getType() == "field")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Get the type of the field and of the assign statement
        std::string eType = children[1]->getSValue();
        std::string rType;

        if (children.size() == 2)
        {
            // No assignment
            rType = this->symbolTable->lookup("variable" + name)->getType();
        }
        else
        {
            // With assignment, check it then get it's type
            this->checkNode(children[2]);
            rType = children[2]->getReturnType();
        }

        // Check that the return type match the expected one
        if (!this->isChildOf(rType, eType))
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type do not match");
        }
    }
    else if (node->getType() == "method")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if the name is already used
        SymbolTableEntry *previous = this->symbolTable->lookupInCurrentScope(name);
        if (previous != NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Method already defined"); // Same name used inside the current scope
        }

        // Check if declared in parent scope => sert à rien fait dans le preprocess
        SymbolTableEntry *superMethod = this->symbolTable->lookup(name);
        if (superMethod != NULL)
        {
            // Check prototype
            if (superMethod->getType() == children[1]->getSValue())
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Type of the super method does not march");
            }
        }
        this->symbolTable->enterNewScope();

        // Check the formals
        if (children.size() == 4)
        {
            this->checkNode(children[3]);
        }

        // Check the method_body
        this->checkNode(children[2]);

        this->symbolTable->exitScope();

        // Check the return type
        if (!this->isChildOf(children[2]->getReturnType(), children[1]->getSValue()))
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Return type of the block does not match declared type of the method");
        }
        // Set the return type of the method
        node->setReturnType(children[0]->getSValue());
    }
    else if (node->getType() == "formals")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check each formal
        for (int i = 0; i < children.size(); i++)
        {
            this->checkNode(children[i]);
        }
    }
    else if (node->getType() == "formal")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        // Check if already defined as formal name must be distinct
        SymbolTableEntry *formal = this->symbolTable->lookup(name);
        if (formal != NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Formals must have distinct names");
        }

        // Add formal to current scope (new scope create in method)
        this->symbolTable->add("variable" + name, children[1]->getSValue());
    }
    else if (node->getType() == "block")
    {
        std::vector<ASTNode *> children = node->getChildren();
        this->symbolTable->enterNewScope();

        // Continue checking
        for (int i = 0; i < children.size(); i++)
        {
            this->checkNode(children[i]);
        }

        this->symbolTable->exitScope();
        node->setReturnType(children[children.size() - 1]->getReturnType());
    }
    else if (node->getType() == "if")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Continue checking
        for (int i = 0; i < children.size(); i++)
        {
            this->symbolTable->enterNewScope();
            this->checkNode(children[i]);
            this->symbolTable->exitScope();
        }

        // Check that the first exp is of type bool
        if (children[0]->getReturnType() != "bool")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }

        // Check that the 2 other expression have the same type
        std::string exprT = children[1]->getReturnType();
        std::string exprE;
        if (children.size() == 3)
        {
            exprE = children[2]->getReturnType();
        }
        else
        {
            exprE = "unit";
        }

        // Both class type
        if (this->isChildOf(exprT, "Object") && this->isChildOf(exprT, "Object"))
        {
            std::string ancestor = this->getFirstCommonAncestor(exprT, exprE);
            node->setReturnType(ancestor);
        }
        // One at least equal to unit
        else if (exprT == "unit" || exprE == "unit")
        {
            node->setReturnType("unit");
        }
        // Equal primitive types
        else if (exprT == exprE)
        {
            node->setReturnType(exprT);
        }
        else
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Types do not match expected");
        }
    }
    else if (node->getType() == "while")
    {
        std::vector<ASTNode *> children = node->getChildren();
        // Continue checking
        for (int i = 0; i < children.size(); i++)
        {
            this->symbolTable->enterNewScope();
            this->checkNode(children[i]);
            this->symbolTable->exitScope();
        }

        // Check that the first exp is of type bool
        if (children[0]->getReturnType() != "bool")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }
        node->setReturnType("unit");
    }
    else if (node->getType() == "let")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check type
        std::string type = children[1]->getSValue();
        if (this->extend->find(type) == this->extend->end())
        {
            throw CheckerException(node->getLine(), node->getColumn(), "unknown type " + type);
        }

        if (children.size() == 4)
        {
            // Check the assign statement
            this->checkNode(children[2]);

            // Evaluate expr after IN
            this->symbolTable->enterNewScope();
            std::string name = children[0]->getSValue();

            if (name == "self")
            {
                throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
            }

            this->symbolTable->add("variable" + name, children[1]->getSValue());

            this->checkNode(children[3]);

            this->symbolTable->exitScope();
        }
        else
        {
            // Evaluate expr after IN
            this->symbolTable->enterNewScope();

            std::string name = children[0]->getSValue();

            if (name == "self")
            {
                throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
            }

            this->symbolTable->add("variable" + name, children[1]->getSValue());

            this->checkNode(children[2]);

            this->symbolTable->exitScope();
        }

        if (children.size() == 4)
        {
            node->setReturnType(children[3]->getReturnType());
        }
        else
        {
            node->setReturnType(children[2]->getReturnType());
        }
    }
    else if (node->getType() == "assign")
    {
        std::vector<ASTNode *> children = node->getChildren();
        std::string name = children[0]->getSValue();

        if (name == "self")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "self cannot be shadowed");
        }

        // Check the expr
        this->symbolTable->enterNewScope();
        this->checkNode(children[1]);
        this->symbolTable->exitScope();

        SymbolTableEntry *identifier = this->symbolTable->lookup("variable" + name);

        // Undefined identifier
        if (identifier == NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Use of unboud variable " + name);
        }

        // Check if the type match
        if (!this->isChildOf(identifier->getType(), children[1]->getReturnType()))
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Same type expected");
        }

        node->setReturnType(children[1]->getReturnType());
    }
    else if (node->getType() == "not")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check the expr
        this->checkNode(children[0]);

        // Check the type of the expr
        if (children[0]->getReturnType() != "bool")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "and")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check the 2 expr
        this->checkNode(children[0]);
        this->checkNode(children[1]);

        // Check the type
        if (children[0]->getReturnType() != "bool")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }
        if (children[1]->getReturnType() != "bool")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type bool expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "equal")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check the 2 expr
        this->checkNode(children[0]);
        this->checkNode(children[1]);

        // Check that the type match
        if (children[0]->getReturnType() != children[1]->getReturnType())
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Same type expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "lower")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check the expr
        this->checkNode(children[0]);
        this->checkNode(children[1]);

        // Check the type
        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "lowerequal")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "plus")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "minus")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "times")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "div")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "pow")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);
        this->checkNode(children[1]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }
        if (children[1]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "neg")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);

        if (children[0]->getReturnType() != "int32")
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type int32 expected");
        }

        node->setReturnType("int32");
    }
    else if (node->getType() == "isnull")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);

        if (!this->isChildOf(children[0]->getReturnType(), "Object"))
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Type Object expected");
        }

        node->setReturnType("bool");
    }
    else if (node->getType() == "new")
    {
        std::vector<ASTNode *> children = node->getChildren();

        this->checkNode(children[0]);

        node->setReturnType(children[0]->getReturnType());
    }
    else if (node->getType() == "call")
    {
        std::vector<ASTNode *> children = node->getChildren();

        // Check the caller
        this->checkNode(children[0]);

        std::string objectType = children[0]->getReturnType();
        std::string methodName = children[1]->getSValue();

        this->symbolTable->enterScope(objectType);
        SymbolTableEntry *method = this->symbolTable->lookup("method" + methodName);
        this->symbolTable->exitScope(objectType);

        // Check if the method if declared
        if (method == nullptr)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Method does not exist");
        }

        // Check if formals are declare that there is some args
        std::vector<SymbolTableEntry *> *formals = method->getFormals();
        if (formals->size() > 0 && children.size() != 3)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
        }

        // If there is args
        if (children.size() == 3)
        {
            std::vector<ASTNode *> args = children[2]->getChildren();

            // Check that the formals expected some args
            if (formals->size() == 0)
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
            }

            // Check the number of args
            if (args.size() != formals->size())
            {
                throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
            }

            // Check that the args are compatible with the formals
            for (int i = 0; i < args.size(); i++)
            {
                // Check the type of the formals
                this->symbolTable->enterNewScope();
                this->checkNode(args[i]);
                this->symbolTable->exitScope();

                std::string formalType = (*formals)[i]->getType();
                std::string argType = args[i]->getReturnType();

                if (!this->isChildOf(argType, formalType))
                {
                    throw CheckerException(node->getLine(), node->getColumn(), "Type does not match");
                }
            }
        }

        // If the call have n args but the method need them
        if (method->getFormals() == NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Invalid number of args");
        }

        node->setReturnType(method->getType());
    }
    else if (node->getType() == "args")
    {
        // The checker must not check this type of node
        std::cout << "  args " << node->getSValue() << std::endl;
    }
    else if (node->getType() == "bool")
    {
        node->setReturnType("bool");
    }
    else if (node->getType() == "int32")
    {
        node->setReturnType("int32");
    }
    else if (node->getType() == "unit")
    {
        node->setReturnType("unit");
    }
    else if (node->getType() == "typeid")
    {
        node->setReturnType(node->getSValue());
    }
    else if (node->getType() == "true")
    {
        node->setReturnType("bool");
    }
    else if (node->getType() == "false")
    {
        node->setReturnType("bool");
    }
    else if (node->getType() == "intliteral")
    {
        node->setReturnType("int32");
    }
    else if (node->getType() == "stringliteral")
    {
        node->setReturnType("string");
    }
    else if (node->getType() == "objectid")
    {
        std::string objectId = node->getSValue();

        // Check that the identifier is declare in scope
        SymbolTableEntry *entry = this->symbolTable->lookup("variable" + objectId);
        if (entry == NULL)
        {
            throw CheckerException(node->getLine(), node->getColumn(), "Object Id \"" + objectId + "\" does not exist");
        }

        node->setReturnType(entry->getType());
    }
    else
    {
        // Must not access this => all node type must be declares before
        throw CheckerException(node->getLine(), node->getColumn(), "Unexpected Node :" + node->getType());
    }
}
