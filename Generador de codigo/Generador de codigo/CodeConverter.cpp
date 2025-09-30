#include "CodeConverter.h"

CodeConverter::CodeConverter() : inputText(""), generatedCode("") {}

CodeConverter::CodeConverter(string inputText)
	: inputText(inputText), generatedCode("") {
}

void CodeConverter::setInputText(string newText) {
	inputText = newText;
}

string CodeConverter::getInputText() {
	return inputText;
}

string CodeConverter::getGeneratedCode() {
	return generatedCode;
}

vector<Message> CodeConverter::getMessages() {
	return messages;
}

bool CodeConverter::process() {
	Utility util;
	generatedCode.clear();
	messages.clear();
	declaredVariables.clear();

	vector<string> lines = util.splitLines(inputText);

	for (size_t i = 0; i < lines.size(); i++) {
		string originalLine = lines[i];
		string normalizedLine = util.toLowerCase(util.trimSpaces(originalLine));

		if (normalizedLine.empty()) continue;

		Intention intention = detectIntention(normalizedLine);
		string code = "";

		switch (intention) {
		case Intention::startProgram: code = emitStart(originalLine); break;
		case Intention::endProgram: code = emitEnd(originalLine); break;
		case Intention::sum: code = emitSum(originalLine); break;
		case Intention::subtract: code = emitSubtract(originalLine); break;
		case Intention::multiply: code = emitMultiply(originalLine); break;
		case Intention::divide: code = emitDivide(originalLine); break;
		case Intention::print: code = emitPrint(originalLine); break;
		case Intention::createList: code = emitCreateList(originalLine); break;
		case Intention::assignListElement: code = emitAssignListElement(originalLine); break;
		case Intention::repeatLoop: code = emitRepeat(originalLine); break;
		case Intention::whileLoop: code = emitWhile(originalLine); break;
		case Intention::createVariable: code = emitCreateVariable(originalLine); break;
		case Intention::ifBlock: code = emitIf(originalLine); break;
		case Intention::elseBlock: code = emitElse(originalLine); break;
		case Intention::inputValue: code = emitInputValue(originalLine); break;
		case Intention::inputList: code = emitInputList(originalLine); break;
		case Intention::iterateList: code = emitIterateList(originalLine); break;
		default:
			messages.push_back(Message(MessageType::Warning,
				"Unrecognized instruction in line: " + originalLine));
			break;
		}

		if (!code.empty()) {
			generatedCode += code + "\n";
			messages.push_back(Message(MessageType::Information,
				"Processed line: " + originalLine));
		}
	}

	return !generatedCode.empty();
}

string CodeConverter::buildProgram() {
	return generatedCode;
}

Intention CodeConverter::detectIntention(string normalizedLine) {
	if (normalizedLine.find("comenzar programa") != string::npos) return Intention::startProgram;
	if (normalizedLine.find("terminar programa") != string::npos) return Intention::endProgram;

	if (normalizedLine.find("ingresar valor de cada") != string::npos &&
		normalizedLine.find("lista") != string::npos) {
		return Intention::inputList;
	}
	if (normalizedLine.find("asignar valor") != string::npos &&
		normalizedLine.find("lista") != string::npos) {
		return Intention::assignListElement;
	}
	if (normalizedLine.find("recorrer la lista") != string::npos)
		return Intention::iterateList;

	if (normalizedLine.rfind("si ", 0) == 0) return Intention::ifBlock;
	if (normalizedLine == "sino" || normalizedLine.find("sino") != string::npos) return Intention::elseBlock;
	if (normalizedLine.find("repetir") != string::npos) return Intention::repeatLoop;
	if (normalizedLine.find("mientras") != string::npos) return Intention::whileLoop;

	if (normalizedLine.find("crear variable") != string::npos) return Intention::createVariable;
	if (normalizedLine.find("ingresar valor") != string::npos) return Intention::inputValue;

	if (normalizedLine.find("sumar") != string::npos) return Intention::sum;
	if (normalizedLine.find("restar") != string::npos) return Intention::subtract;
	if (normalizedLine.find("multiplicar") != string::npos) return Intention::multiply;
	if (normalizedLine.find("dividir") != string::npos) return Intention::divide;

	if (normalizedLine.find("crear lista") != string::npos ||
		normalizedLine.find("crear una lista") != string::npos) {
		return Intention::createList;
	}

	if (normalizedLine.find("imprimir") != string::npos ||
		normalizedLine.find("mostrar") != string::npos) return Intention::print;

	return Intention::unknown;
}

string CodeConverter::emitStart(string originalLine) {
	return "#include <iostream>\n#include <vector>\nusing namespace std;\n\nint main() {";
}

string CodeConverter::emitEnd(string originalLine) {
	return "return 0;\n}";
}

string CodeConverter::emitSum(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string target = "";
	string operation = "";
	bool foundSomething = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "a" && i + 1 < words.size()) {
			target = words[i + 1];
		}
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
		}
	}

	for (const string& word : words) {
		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " + ";
			operation += var->getName();
			foundSomething = true;
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	for (int n : numbers) {
		if (!operation.empty()) operation += " + ";
		operation += to_string(n);
		foundSomething = true;
	}

	if (!foundSomething) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron números o variables para sumar en -> " + originalLine));
		return "";
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + target + "' no fue declarada antes."));
			return "";
		}
		return target + " = " + operation + ";";
	}

	string tempName = "resultSum" + to_string(sumCounter++);
	return "int " + tempName + " = " + operation + ";";
}

string CodeConverter::emitSubtract(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string target = "";
	string operation = "";
	bool foundSomething = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "a" && i + 1 < words.size()) {
			target = words[i + 1];
		}
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
		}
	}

	for (const string& word : words) {
		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " - ";
			operation += var->getName();
			foundSomething = true;
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	for (int n : numbers) {
		if (!operation.empty()) operation += " - ";
		operation += to_string(n);
		foundSomething = true;
	}

	if (!foundSomething) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron números o variables para restar en -> " + originalLine));
		return "";
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + target + "' no fue declarada antes."));
			return "";
		}
		return target + " = " + operation + ";";
	}

	string tempName = "resultSubtract" + to_string(subtractCounter++);
	return "int " + tempName + " = " + operation + ";";
}

string CodeConverter::emitMultiply(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string target = "";
	string operation = "";
	bool foundSomething = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "a" && i + 1 < words.size()) {
			target = words[i + 1];
		}
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
		}
	}

	for (const string& word : words) {
		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " * ";
			operation += var->getName();
			foundSomething = true;
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	for (int n : numbers) {
		if (!operation.empty()) operation += " * ";
		operation += to_string(n);
		foundSomething = true;
	}

	if (!foundSomething) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron números o variables para multiplicar en -> " + originalLine));
		return "";
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + target + "' no fue declarada antes."));
			return "";
		}
		return target + " = " + operation + ";";
	}

	string tempName = "resultMultiply" + to_string(multiplyCounter++);
	return "int " + tempName + " = " + operation + ";";
}

string CodeConverter::emitDivide(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string target = "";
	string operation = "";
	bool foundSomething = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "a" && i + 1 < words.size()) {
			target = words[i + 1];
			continue;
		}
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			continue;
		}
	}

	for (const string& word : words) {
		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (var->getName() == target) continue;
			if (!operation.empty()) operation += " / ";
			operation += var->getName();
			foundSomething = true;
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	for (int n : numbers) {
		if (!operation.empty()) operation += " / ";
		operation += to_string(n);
		foundSomething = true;
	}

	if (!foundSomething) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron números o variables para dividir en -> " + originalLine));
		return "";
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + target + "' no fue declarada antes."));
			return "";
		}
		return target + " = " + operation + ";";
	}

	string tempName = "resultDivide" + to_string(divideCounter++);
	return "int " + tempName + " = " + operation + ";";
}

string CodeConverter::emitPrint(string originalLine) {
	Utility util;
	vector<string> parts;

	size_t pos = 0;
	while ((pos = originalLine.find("\"", pos)) != string::npos) {
		size_t end = originalLine.find("\"", pos + 1);
		if (end == string::npos) break;
		string text = originalLine.substr(pos, end - pos + 1);
		parts.push_back(text);
		pos = end + 1;
	}

	vector<string> words = util.splitWords(originalLine);
	for (const string& word : words) {
		Variable* var = findVariable(word);
		if (var != nullptr) {
			parts.push_back(var->getName());
		}
	}

	if (parts.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se pudo determinar qué imprimir en -> " + originalLine));
		return "";
	}

	string code = "cout";
	for (const string& p : parts) {
		code += " << " + p;
	}
	code += " << endl;";

	return code;
}

string CodeConverter::emitCreateList(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);
	vector<int> numbers = util.extractIntegers(originalLine);

	string type = "";
	for (const string& w : words) {
		if (w == "entero" || w == "enteros" || w == "int") type = "int";
		else if (w == "decimal" || w == "decimales" || w == "float") type = "float";
		else if (w == "texto" || w == "string" || w == "strings") type = "string";
	}

	if (type.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó tipo de la lista en -> " + originalLine));
		return "";
	}

	if (numbers.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó tamaño de la lista en -> " + originalLine));
		return "";
	}
	int size = numbers[0];

	string listName = "";
	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "llamada" && i + 1 < words.size()) {
			listName = words[i + 1];
			break;
		}
	}
	if (listName.empty() && !words.empty()) {
		listName = words.back();
	}

	if (listName == "enteros" || listName == "decimales" ||
		listName == "strings" || listName == "elementos" ||
		listName == "valores") {
		messages.push_back(Message(MessageType::Error,
			"Falta nombre válido para la lista en -> " + originalLine));
		return "";
	}

	Variable newVar(type, listName, "", size, true);
	declaredVariables.push_back(newVar);

	return "vector<" + type + "> " + listName + "(" + to_string(size) + ");";
}

string CodeConverter::emitAssignListElement(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string listName = "";
	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "lista" && i + 1 < words.size()) {
			listName = words[i + 1];
		}
	}

	if (listName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"Falta nombre de lista en -> " + originalLine));
		return "";
	}

	Variable* listVar = findVariable(listName);
	if (!listVar || !listVar->getIsList()) {
		messages.push_back(Message(MessageType::Error,
			"La lista '" + listName + "' no fue declarada antes."));
		return "";
	}

	int index = -1;
	for (const string& w : words) {
		if (w == "primer" || w == "primero") index = 0;
		else if (w == "segundo") index = 1;
		else if (w == "tercero") index = 2;
		else if (w == "cuarto") index = 3;
		else if (w == "quinto") index = 4;
		else if (w == "sexto") index = 5;
		else if (w == "séptimo" || w == "septimo") index = 6;
		else if (w == "octavo") index = 7;
		else if (w == "noveno") index = 8;
		else if (w == "décimo" || w == "decimo") index = 9;
	}
	if (index == -1) {
		vector<int> numbers = util.extractIntegers(originalLine);
		if (!numbers.empty()) {
			index = numbers[0] - 1;
		}
	}

	if (index < 0 || index >= listVar->getSize()) {
		messages.push_back(Message(MessageType::Error,
			"El índice excede el tamaño de la lista '" + listName + "' en -> " + originalLine));
		return "";
	}

	string valueAssigned = "";
	if (listVar->getType() == "string") {
		size_t posQuote = originalLine.find("\"");
		if (posQuote != string::npos) {
			size_t lastQuote = originalLine.find_last_of("\"");
			if (lastQuote > posQuote) {
				valueAssigned = originalLine.substr(posQuote, lastQuote - posQuote + 1);
			}
		}
		if (valueAssigned.empty()) {
			messages.push_back(Message(MessageType::Error,
				"La lista '" + listName + "' es de tipo string, pero no se encontró texto válido."));
			return "";
		}
	}
	else {
		vector<int> numbers = util.extractIntegers(originalLine);
		if (numbers.empty()) {
			messages.push_back(Message(MessageType::Error,
				"La lista '" + listName + "' es de tipo " + listVar->getType() + " pero no se encontró número."));
			return "";
		}
		valueAssigned = to_string(numbers[0]);
	}

	return listName + "[" + to_string(index) + "] = " + valueAssigned + ";";
}

string CodeConverter::emitRepeat(string originalLine) {
	Utility util;
	vector<int> numbers = util.extractIntegers(originalLine);

	if (numbers.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró cuántas veces repetir en -> " + originalLine));
		return "";
	}

	int repetitions = numbers[0];
	size_t posQuote = originalLine.find("\"");
	string message = (posQuote != string::npos)
		? originalLine.substr(posQuote, originalLine.find_last_of("\"") - posQuote + 1)
		: "\"Iteracion\"";

	string code;
	code += "for (int i = 0; i < " + to_string(repetitions) + "; i++) {\n";
	code += "    cout << " + message + " << endl;\n";
	code += "}";
	return code;
}

string CodeConverter::emitWhile(string originalLine) {
	Utility util;
	vector<int> numbers = util.extractIntegers(originalLine);

	if (numbers.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró condición numérica en -> " + originalLine));
		return "";
	}

	int limit = numbers[0];
	string varName = "number";

	vector<string> words = util.splitWords(originalLine);
	for (const string& w : words) {
		Variable* var = findVariable(w);
		if (var != nullptr && !var->getIsList()) {
			varName = var->getName();
			break;
		}
	}

	Variable* var = findVariable(varName);
	if (!var) {
		messages.push_back(Message(MessageType::Error,
			"La variable '" + varName + "' no fue declarada antes."));
		return "";
	}

	string increment = "1";
	if (numbers.size() >= 2) {
		increment = to_string(numbers[1]);
	}

	string code;
	code += "while (" + varName + " < " + to_string(limit) + ") {\n";
	code += "    " + varName + " += " + increment + ";\n";
	code += "}";
	return code;
}

string CodeConverter::emitIf(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);
	vector<int> numbers = util.extractIntegers(originalLine);

	if (numbers.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró número de comparación en -> " + originalLine));
		return "";
	}

	string variableName = "number";
	string op = ">";

	for (const string& w : words) {
		Variable* var = findVariable(w);
		if (var != nullptr && !var->getIsList()) {
			variableName = var->getName();
			break;
		}
	}

	if (originalLine.find("mayor que") != string::npos) op = ">";
	else if (originalLine.find("menor que") != string::npos) op = "<";
	else if (originalLine.find("igual a") != string::npos) op = "==";

	int value = numbers[0];

	string code;
	code += "if (" + variableName + " " + op + " " + to_string(value) + ") {";
	insideBlock = true;
	return code;
}

string CodeConverter::emitCreateVariable(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string type = "int";
	string name = "variable";
	string value = "0";
	bool typeDetected = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "decimal") {
			type = "float";
			typeDetected = true;
			if (i + 1 < words.size()) name = words[i + 1];
		}
		else if (words[i] == "entero" || words[i] == "int") {
			type = "int";
			typeDetected = true;
			if (i + 1 < words.size()) name = words[i + 1];
		}
		else if (words[i] == "texto" || words[i] == "string") {
			type = "string";
			typeDetected = true;
			if (i + 1 < words.size()) name = words[i + 1];
		}
		else if (words[i] == "booleano" || words[i] == "bool") {
			type = "bool";
			typeDetected = true;
			if (i + 1 < words.size()) name = words[i + 1];
		}
	}

	if (!typeDetected) {
		for (size_t i = 0; i < words.size(); i++) {
			if (words[i] == "variable" && i + 1 < words.size()) {
				name = words[i + 1];
				break;
			}
		}
	}

	if (type == "bool") {
		value = "false";
		for (size_t i = 0; i < words.size(); i++) {
			if (words[i] == "verdadero" || words[i] == "true") value = "true";
			else if (words[i] == "falso" || words[i] == "false") value = "false";
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	if (!numbers.empty()) {
		value = to_string(numbers[0]);
	}

	if (type == "string") {
		value = "\"\"";
		size_t posQuote = originalLine.find("\"");
		if (posQuote != string::npos) {
			size_t lastQuote = originalLine.find_last_of("\"");
			if (lastQuote > posQuote) {
				value = originalLine.substr(posQuote, lastQuote - posQuote + 1);
			}
		}
	}

	Variable newVar(type, name, value);
	declaredVariables.push_back(newVar);

	return type + " " + name + " = " + value + ";";
}

string CodeConverter::emitInputList(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string listName = "";
	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "lista" && i + 1 < words.size()) {
			listName = words.back();
			break;
		}
	}

	if (listName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se especificó el nombre de la lista en -> " + originalLine));
		return "";
	}

	Variable* listVar = findVariable(listName);
	if (!listVar || !listVar->getIsList()) {
		messages.push_back(Message(MessageType::Error,
			"La lista '" + listName + "' no fue declarada antes."));
		return "";
	}

	string code;
	code += "for (int i = 0; i < " + listName + ".size(); i++) {\n";
	code += "    cin >> " + listName + "[i];\n";
	code += "}";
	return code;
}

string CodeConverter::emitIterateList(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string listName = "";
	string accumulator = "";

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "lista" && i + 1 < words.size()) {
			listName = words[i + 1];
		}
	}

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "en" && i + 1 < words.size()) {
			accumulator = words[i + 1];
		}
	}

	if (listName.empty() || accumulator.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se pudo determinar lista o variable acumuladora en -> " + originalLine));
		return "";
	}

	Variable* listVar = findVariable(listName);
	Variable* accumulatorVar = findVariable(accumulator);

	if (!listVar || !listVar->getIsList()) {
		messages.push_back(Message(MessageType::Error,
			"La lista '" + listName + "' no fue declarada antes."));
		return "";
	}
	if (!accumulatorVar) {
		messages.push_back(Message(MessageType::Error,
			"La variable acumuladora '" + accumulator + "' no fue declarada antes."));
		return "";
	}

	return "for (int i = 0; i < " + listName + ".size(); i++) {\n" +
		"    " + accumulator + " += " + listName + "[i];\n" +
		"}";
}

string CodeConverter::emitInputValue(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string variableName = "";
	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "valor" && i + 1 < words.size()) {
			variableName = words[i + 1];
			break;
		}
	}

	if (variableName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó variable en -> " + originalLine));
		return "";
	}

	Variable* var = findVariable(variableName);
	if (!var) {
		messages.push_back(Message(MessageType::Error,
			"La variable '" + variableName + "' no fue declarada antes."));
		return "";
	}

	return "cin >> " + variableName + ";";
}

string CodeConverter::emitElse(string originalLine) {
	insideBlock = true;
	return "else {";
}

int CodeConverter::getIndexFromWordOrNumber(string originalLine, vector<string> words) {
	for (size_t i = 0; i < words.size(); i++) {
		string w = words[i];
		if (w == "primer" || w == "primero") return 0;
		if (w == "segundo") return 1;
		if (w == "tercero") return 2;
		if (w == "cuarto") return 3;
		if (w == "quinto") return 4;
		if (w == "sexto") return 5;
		if (w == "séptimo" || w == "septimo") return 6;
		if (w == "octavo") return 7;
		if (w == "noveno") return 8;
		if (w == "décimo" || w == "decimo") return 9;
	}

	Utility util;
	vector<int> numbers = util.extractIntegers(originalLine);
	if (!numbers.empty()) {
		return numbers[0] - 1;
	}

	return -1;
}

string CodeConverter::normalizeWord(const string& word) {
	static vector<string> stopwords = {
		"de", "la", "el", "los", "las",
		"un", "una", "unos", "unas",
		"cada", "en", "con", "valor"
	};

	for (const auto& stop : stopwords) {
		if (word == stop) return "";
	}
	return word;
}

Variable* CodeConverter::findVariable(string name) {
	string cleanName = normalizeWord(name);
	if (cleanName.empty()) return nullptr;

	Utility util;
	string normalizedName = util.toLowerCase(cleanName);

	for (auto& v : declaredVariables) {
		if (util.toLowerCase(v.getName()) == normalizedName) {
			return &v;
		}
	}
	return nullptr;
}
















