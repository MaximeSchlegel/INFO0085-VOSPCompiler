#include "checker.h"
#include "../utils/graph.h"


Checker::Checker() {
    this->primary = new std::map<std::string, std::vector<std::string> *>();
    this->primary->emplace("int32", new std::vector<std::string>());
    this->primary->emplace("bool", new std::vector<std::string>());
    this->primary->emplace("string", new std::vector<std::string>());
    this->primary->emplace("unit", new std::vector<std::string>());
    this->primary->emplace("Object", new std::vector<std::string>());
}

int Checker::check(ASTNode *root) {
    this->registerClasses(root);
    return 0;
}


extern std::string filename;

void Checker::registerClasses(ASTNode *root) {
    // doit ajouter la liste des classes aux types,
    // determiner si il y a des cycles dans l'héritage

    std::vector<ASTNode *> rootChildren = root->getChildren();
    std::vector<std::string *> *viewedclass = new std::vector<std::string *>();
    viewedclass->push_back(new std::string("Object"));

    for (int i = 0; i < rootChildren.size(); i++) {
        if (rootChildren[i]->getType() == "class") {
            std::string className = rootChildren[i]->getChildren()[rootChildren[i]->getChildren().size() - 1]->getType();
            for (int i )
            std::cout << rootChildren[i]->getType() << "  " << rootChildren[i]->getChildren()[rootChildren[i]->getChildren().size() -1]->getType() << std::endl;
        }
    }
}