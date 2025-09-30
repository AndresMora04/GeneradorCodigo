#pragma once
#include <string>
using namespace std;

class Variable
{
private:
	string type;
	string name;
	string value;
	int size;
	bool isList;

public:
	Variable();
	Variable(string type, string name, string value = "", int size = 0, bool isList = false);

	string getType() const;
	string getName() const;
	string getValue() const;
	int getSize() const;
	bool getIsList() const;

	void setType(const string& type);
	void setName(const string& name);
	void setValue(const string& value);
	void setSize(int size);
	void setIsList(bool isList);
};

