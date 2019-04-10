#include "checker.h"

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
}