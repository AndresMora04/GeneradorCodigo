#include "Variable.h"

Variable::Variable()
    : type(""), name(""), value(""), size(0), isList(false) {
}

Variable::Variable(string type, string name, string value, int size, bool isList)
    : type(type), name(name), value(value), size(size), isList(isList) {
}

string Variable::getType() const { return type; }
string Variable::getName() const { return name; }
string Variable::getValue() const { return value; }
int Variable::getSize() const { return size; }
bool Variable::getIsList() const { return isList; }

void Variable::setType(const string& type) { this->type = type; }
void Variable::setName(const string& name) { this->name = name; }
void Variable::setValue(const string& value) { this->value = value; }
void Variable::setSize(int size) { this->size = size; }
void Variable::setIsList(bool isList) { this->isList = isList; }