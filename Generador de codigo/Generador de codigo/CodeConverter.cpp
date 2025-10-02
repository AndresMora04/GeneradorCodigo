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
	blockDepth = 0;
	generatedCode.clear();
	messages.clear();
	declaredVariables.clear();
	headersCode.clear();
	functionsCode.clear();
	mainCode.clear();
	inFunction = false;
	sawMainStart = false;
	Utility util;

	vector<string> lines = util.splitLines(inputText);

	for (size_t i = 0; i < lines.size(); i++) {
		string originalLine = lines[i];
		string normalizedLine = util.toLowerCase(util.trimSpaces(originalLine));

		if (normalizedLine.empty()) continue;

		Intention intention = detectIntention(normalizedLine);
		string code = "";

		string maybeClose = closeBlockIfNeeded(normalizedLine);
		if (!maybeClose.empty()) {
			std::string* out = inFunction ? &functionsCode : &mainCode;
			*out += maybeClose + "\n";
		}

		switch (intention) {
		case Intention::startProgram: code = emitStart(originalLine); break;
		case Intention::endProgram: code = emitEnd(originalLine); break;
		case Intention::assignValue: code = emitAssign(originalLine); break;
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
		case Intention::forEachLoop: code = emitForEach(originalLine); break;
		case Intention::defineFunction: code = emitDefineFunction(originalLine); break;
		case Intention::returnStatement: code = emitReturn(originalLine); break;
		case Intention::callFunction: code = emitCallFunction(originalLine); break;
		case Intention::calculate: code = emitCalculate(originalLine); break;
		case Intention::createStruct: code = emitCreateStruct(originalLine); break;
		case Intention::createCustomList: code = emitCreateCustomList(originalLine); break;
		case Intention::inputStructItems: code = emitInputStructItems(originalLine); break;
		case Intention::traverseStructListPrint: code = emitTraverseStructListPrint(originalLine); break;
		default:
			messages.push_back(Message(MessageType::Warning,
				"Unrecognized instruction in line: " + originalLine));
			break;
		}

		if (!code.empty()) {
			std::string* out = &mainCode;
			if (intention == Intention::defineFunction || intention == Intention::returnStatement || intention == Intention::createStruct || inFunction) {
				out = &functionsCode;
			}
			*out += code + "\n";

			messages.push_back(Message(MessageType::Information,
				"Processed line: " + originalLine));
		}
		else {
			bool hasError = false;
			for (auto& msg : messages) {
				if (msg.getType() == MessageType::Error &&
					msg.getText().find(originalLine) != string::npos) {
					hasError = true;
					break;
				}
			}
			if (!hasError) {
				if (intention == Intention::startProgram) {
					messages.push_back(Message(MessageType::Information,
						"Processed line: " + originalLine));
				}
				else {
					messages.push_back(Message(MessageType::Warning,
						"No code generated for line: " + originalLine));
				}
			}
		}

	}

	while (blockDepth > 0) {
		string* out = inFunction ? &functionsCode : &mainCode;
		*out += "}\n";
		blockDepth--;
	}

	if (sawMainStart) {
		headersCode =
			"#include <iostream>\n"
			"#include <vector>\n"
			"#include <string>\n"
			"using namespace std;\n\n";
	}

	if (sawMainStart && mainCode.find("int main() {") == std::string::npos) {
		mainCode = "int main() {\n" + mainCode;
	}

	generatedCode = headersCode + functionsCode + mainCode;

	if (inFunction) {
		generatedCode += "}\n";
		inFunction = false;
	}

	return !generatedCode.empty();
}


string CodeConverter::buildProgram() {
	return generatedCode;
}

Intention CodeConverter::detectIntention(string normalizedLine) {
	if (normalizedLine.find("comenzar programa") != string::npos) return Intention::startProgram;
	if (normalizedLine.find("terminar programa") != string::npos)  return Intention::endProgram;

	if (normalizedLine.find("definir funcion") != string::npos)    return Intention::defineFunction;
	if (normalizedLine.find("retornar") != string::npos)            return Intention::returnStatement;

	if (normalizedLine.find("crear estructura") != string::npos)    return Intention::createStruct;
	if (normalizedLine.find("crear lista de") != string::npos)      return Intention::createCustomList;
	if (normalizedLine.find("crear lista") != string::npos ||
		normalizedLine.find("crear una lista") != string::npos)     return Intention::createList;

	if (normalizedLine.find("ingresar los datos de cada") != string::npos) return Intention::inputStructItems;
	if ((normalizedLine.find("ingresar valor de cada") != string::npos && normalizedLine.find("lista") != string::npos) ||
		normalizedLine.find("ingresar los valores de la lista") != string::npos)
		return Intention::inputList;

	if (normalizedLine.find("recorrer la lista") != string::npos &&
		normalizedLine.find("mostrar") != string::npos)
		return Intention::traverseStructListPrint;

	if (normalizedLine.find("recorrer la lista") != string::npos) {
		if (normalizedLine.find("sumar") != string::npos || normalizedLine.find("total") != string::npos)
			return Intention::iterateList;
		return Intention::forEachLoop;
	}

	if (normalizedLine.find("crear variable") != string::npos)      return Intention::createVariable;
	if (normalizedLine.find("ingresar valor") != string::npos)      return Intention::inputValue;

	if (normalizedLine.find("calcular") != string::npos)            return Intention::calculate;

	if (normalizedLine.find("asignar valor") != string::npos &&
		normalizedLine.find("lista") != string::npos)
		return Intention::assignListElement;

	if (normalizedLine.find("llamar funcion") != string::npos)      return Intention::callFunction;

	if (normalizedLine.rfind("si ", 0) == 0)                        return Intention::ifBlock;
	if (normalizedLine == "sino" || normalizedLine.find("sino") != string::npos) return Intention::elseBlock;
	if (normalizedLine.find("mientras") != string::npos)            return Intention::whileLoop;
	if (normalizedLine.find("repetir") != string::npos)             return Intention::repeatLoop;

	if (normalizedLine.find("sumar") != string::npos)               return Intention::sum;
	if (normalizedLine.find("restar") != string::npos)              return Intention::subtract;
	if (normalizedLine.find("multiplicar") != string::npos)         return Intention::multiply;
	if (normalizedLine.find("dividir") != string::npos)             return Intention::divide;

	if (normalizedLine.find("asignar") != string::npos)             return Intention::assignValue;

	if (normalizedLine.find("imprimir") != string::npos ||
		normalizedLine.find("mostrar") != string::npos)             return Intention::print;

	return Intention::unknown;
}

string CodeConverter::emitStart(string /*originalLine*/) {
	sawMainStart = true;
	return "";
}

string CodeConverter::emitEnd(string) {
	string code;
	while (blockDepth > 0) { code += "}\n"; blockDepth--; }
#ifdef _WIN32
	code += "system(\"pause\");\n";
#else
	code += "// Presiona Enter para finalizar\ncin.get();\n";
#endif
	code += "return 0;\n}";
	return code;
}

string CodeConverter::emitSum(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	{
		int k = -1;
		for (int i = 0; i < (int)words.size(); ++i) {
			if (util.toLowerCase(words[i]) == "sumar") { k = i; break; }
		}
		if (k > 0) {
			string lhs;
			for (int i = k - 1; i >= 0; --i) {
				Variable* v = findVariable(words[i]);
				if (v && !v->getIsList()) { lhs = v->getName(); break; }
			}
			if (!lhs.empty()) {
				int j = k + 1;
				string rhs;

				if (j < (int)words.size()) {
					if (Variable* vr = findVariable(words[j]); vr && !vr->getIsList()) {
						rhs = vr->getName();
					}
				}
				if (rhs.empty()) {
					vector<int> nums = util.extractIntegers(originalLine);
					if (!nums.empty()) rhs = to_string(nums.back());
				}

				if (!rhs.empty()) {
					return lhs + " += " + rhs + ";";
				}
				else {
					messages.push_back(Message(MessageType::Error,
						"No se pudo determinar el valor a sumar en -> " + originalLine));
				}
			}
		}
	}

	string target = "";
	string operation = "";
	bool foundSomething = false;
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó la variable destino en -> " + originalLine));
		hasError = true;
	}

	for (string& word : words) {
		if (isReservedWord(word)) continue;

		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " + ";
			operation += var->getName();
			foundSomething = true;
		}
		else if (isalpha(static_cast<unsigned char>(word[0])) && word != "asignar" && word != "a" && word != "y") {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + word + "' no fue declarada antes."));
			hasError = true;
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
		hasError = true;
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable destino '" + target + "' no fue declarada antes."));
			hasError = true;
		}
	}

	if (hasError) return "";

	return target + " = " + operation + ";";
}

string CodeConverter::emitSubtract(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	int k = -1;
	for (int i = 0; i < (int)words.size(); ++i) {
		if (util.toLowerCase(words[i]) == "restar") { k = i; break; }
	}
	if (k > 0) {
		string lhs;
		for (int i = k - 1; i >= 0; --i) {
			Variable* v = findVariable(words[i]);
			if (v && !v->getIsList()) { lhs = v->getName(); break; }
		}

		if (!lhs.empty()) {
			int j = k + 1;
			string rhs;

			if (j < (int)words.size()) {
				if (Variable* vr = findVariable(words[j]); vr && !vr->getIsList()) {
					rhs = vr->getName();
				}
			}
			if (rhs.empty()) {
				string lower = util.toLowerCase(originalLine);
				size_t posRest = lower.find("restar");
				if (posRest == string::npos) posRest = 0;
				string tail = originalLine.substr(posRest);
				vector<int> nums = util.extractIntegers(tail);
				if (!nums.empty()) rhs = to_string(nums.front());
			}

			if (!rhs.empty()) {
				return lhs + " -= " + rhs + ";";
			}
			else {
				messages.push_back(Message(MessageType::Error,
					"No se pudo determinar el valor a restar en -> " + originalLine));
			}
		}
	}

	string target = "";
	string operation = "";
	bool foundSomething = false;
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó la variable destino en -> " + originalLine));
		hasError = true;
	}

	for (string& word : words) {
		if (isReservedWord(word)) continue;

		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " - ";
			operation += var->getName();
			foundSomething = true;
		}
		else if (isalpha(static_cast<unsigned char>(word[0])) && word != "asignar" && word != "a" && word != "y") {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + word + "' no fue declarada antes."));
			hasError = true;
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
		hasError = true;
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable destino '" + target + "' no fue declarada antes."));
			hasError = true;
		}
	}

	if (hasError) return "";

	return target + " = " + operation + ";";
}

string CodeConverter::emitMultiply(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	int k = -1;
	for (int i = 0; i < (int)words.size(); ++i) {
		if (util.toLowerCase(words[i]) == "multiplicar") { k = i; break; }
	}
	if (k > 0) {
		string lhs;
		for (int i = k - 1; i >= 0; --i) {
			Variable* v = findVariable(words[i]);
			if (v && !v->getIsList()) { lhs = v->getName(); break; }
		}

		if (!lhs.empty()) {
			int j = k + 1;
			if (j < (int)words.size() && util.toLowerCase(words[j]) == "por") ++j;

			string rhs;

			if (j < (int)words.size()) {
				Variable* vr = findVariable(words[j]);
				if (vr && !vr->getIsList()) rhs = vr->getName();
			}

			if (rhs.empty()) {
				vector<int> nums = util.extractIntegers(originalLine);
				if (!nums.empty()) rhs = to_string(nums.back());
			}

			if (rhs.empty()) {
				messages.push_back(Message(MessageType::Error,
					"No se pudo determinar el valor a multiplicar en -> " + originalLine));
				return "";
			}

			return lhs + " *= " + rhs + ";";
		}
	}

	string target = "";
	string operation = "";
	bool foundSomething = false;
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó la variable destino en -> " + originalLine));
		hasError = true;
	}

	for (string& word : words) {
		if (isReservedWord(word)) continue;

		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " * ";
			operation += var->getName();
			foundSomething = true;
		}
		else if (isalpha(static_cast<unsigned char>(word[0])) && word != "asignar" && word != "a" && word != "y") {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + word + "' no fue declarada antes."));
			hasError = true;
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
		hasError = true;
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable destino '" + target + "' no fue declarada antes."));
			hasError = true;
		}
	}

	if (hasError) return "";

	return target + " = " + operation + ";";
}

string CodeConverter::emitDivide(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	{
		int k = -1;
		for (int i = 0; i < (int)words.size(); ++i) {
			if (util.toLowerCase(words[i]) == "dividir") { k = i; break; }
		}
		if (k > 0) {
			string lhs;
			for (int i = k - 1; i >= 0; --i) {
				Variable* v = findVariable(words[i]);
				if (v && !v->getIsList()) { lhs = v->getName(); break; }
			}

			if (!lhs.empty()) {
				int j = k + 1;
				if (j < (int)words.size() && util.toLowerCase(words[j]) == "por") ++j;

				string rhs;

				if (j < (int)words.size()) {
					if (Variable* vr = findVariable(words[j]); vr && !vr->getIsList()) {
						rhs = vr->getName();
					}
				}
				if (rhs.empty()) {
					vector<int> nums = util.extractIntegers(originalLine);
					if (!nums.empty()) rhs = to_string(nums.back());
				}

				if (!rhs.empty()) {
					return lhs + " /= " + rhs + ";";
				}
				else {
					messages.push_back(Message(MessageType::Error,
						"No se pudo determinar el divisor en -> " + originalLine));
					return "";
				}
			}
		}
	}

	string target = "";
	string operation = "";
	bool foundSomething = false;
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó la variable destino en -> " + originalLine));
		hasError = true;
	}

	for (string& word : words) {
		if (isReservedWord(word)) continue;

		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty()) operation += " / ";
			operation += var->getName();
			foundSomething = true;
		}
		else if (isalpha(static_cast<unsigned char>(word[0])) && word != "asignar" && word != "a" && word != "y") {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + word + "' no fue declarada antes."));
			hasError = true;
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
		hasError = true;
	}

	if (!target.empty()) {
		Variable* var = findVariable(target);
		if (!var) {
			messages.push_back(Message(MessageType::Error,
				"La variable destino '" + target + "' no fue declarada antes."));
			hasError = true;
		}
	}

	if (hasError) return "";

	return target + " = " + operation + ";";
}

string CodeConverter::emitPrint(string originalLine) {
	Utility util;
	vector<string> parts;

	size_t pos = 0;
	while ((pos = originalLine.find("\"", pos)) != string::npos) {
		size_t end = originalLine.find("\"", pos + 1);
		if (end == string::npos) break;
		string text = originalLine.substr(pos, end - pos + 1);
		if (find(parts.begin(), parts.end(), text) == parts.end())
			parts.push_back(text);
		pos = end + 1;
	}

	vector<string> words = util.splitWords(originalLine);
	for (size_t i = 0; i + 1 < words.size(); i++) {
		string lw = util.toLowerCase(words[i]);
		if (lw == "y") {
			string candidate = words[i + 1];

			if (candidate == "i") {
				if (find(parts.begin(), parts.end(), string("i")) == parts.end())
					parts.push_back("i");
				continue;
			}

			if (Variable* var = findVariable(candidate)) {
				string name = var->getName();
				if (find(parts.begin(), parts.end(), name) == parts.end())
					parts.push_back(name);
			}
		}
	}

	if (parts.empty()) {
		string lower = util.toLowerCase(originalLine);
		bool startsWithImprimir = (lower.rfind("imprimir ", 0) == 0);
		bool startsWithMostrar = (lower.rfind("mostrar ", 0) == 0);

		if (startsWithImprimir || startsWithMostrar) {
			if (words.size() >= 2) {
				string tok = words[1];

				if (tok == "i") {
					return "cout << i << endl;";
				}

				if (Variable* v = findVariable(tok)) {
					return "cout << " + v->getName() + " << endl;";
				}
			}
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

string CodeConverter::emitAssign(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string value = "";
	string target = "";
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		string lw = util.toLowerCase(words[i]);
		if (lw == "asignar") {
			if (i + 1 < words.size()) {
				value = words[i + 1];
			}
			for (size_t j = i + 1; j < words.size(); j++) {
				string lw2 = util.toLowerCase(words[j]);
				if (lw2 == "a" && j + 1 < words.size()) {
					target = words[j + 1];
					break;
				}
			}
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó variable destino en -> " + originalLine));
		return "";
	}

	string lowerValue = util.toLowerCase(value);
	if (lowerValue == "verdadero" || lowerValue == "true")
		value = "true";
	else if (lowerValue == "falso" || lowerValue == "false")
		value = "false";

	vector<int> nums = util.extractIntegers(value);
	if (!nums.empty()) {
		value = to_string(nums[0]);
	}

	if (nums.empty() && value != "true" && value != "false" && value.find("\"") == string::npos) {
		Variable* varValue = findVariable(value);
		if (!varValue) {
			messages.push_back(Message(MessageType::Error,
				"La variable origen '" + value + "' no fue declarada antes."));
			hasError = true;
		}
	}

	Variable* varTarget = findVariable(target);
	if (!varTarget) {
		messages.push_back(Message(MessageType::Error,
			"La variable destino '" + target + "' no fue declarada antes."));
		hasError = true;
	}

	if (hasError) return "";

	return target + " = " + value + ";";
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
	vector<string> words = util.splitWords(originalLine);
	string lowerLine = util.toLowerCase(originalLine);

	string cmp = "<";
	if (lowerLine.find("mayor o igual") != string::npos) cmp = ">=";
	else if (lowerLine.find("menor o igual") != string::npos) cmp = "<=";
	else if (lowerLine.find("distinto de") != string::npos ||
		lowerLine.find("diferente de") != string::npos ||
		lowerLine.find("no igual") != string::npos) cmp = "!=";
	else if (lowerLine.find("mayor que") != string::npos) cmp = ">";
	else if (lowerLine.find("menor que") != string::npos) cmp = "<";
	else if (lowerLine.find("igual que") != string::npos ||
		lowerLine.find("igual a") != string::npos ||
		lowerLine.find("igual") != string::npos) cmp = "==";

	string lhs, rhs;
	string candidateName;

	for (const string& w : words) {
		if (w == "i") {
			lhs = "i";
			break;
		}
		if (w.find('[') != string::npos && w.find(']') != string::npos) {
			lhs = w;
			break;
		}
		if (Variable* v = findVariable(w)) {
			if (!v->getIsList()) { lhs = v->getName(); break; }
		}
		if (candidateName.empty() && !w.empty() && isalpha(static_cast<unsigned char>(w[0]))) {
			candidateName = w;
		}
	}

	if (lhs.empty()) {
		if (!candidateName.empty()) {
			messages.push_back(Message(MessageType::Error,
				"La variable '" + candidateName + "' no fue declarada antes."));
		}
		else {
			messages.push_back(Message(MessageType::Error,
				"La condición 'Mientras ...' requiere una variable existente (ej.: 'Mientras n mayor que 1')."));
		}
		return "";
	}

	vector<int> nums = util.extractIntegers(originalLine);
	if (!nums.empty()) {
		rhs = to_string(nums.front());
	}
	else {
		for (const string& w : words) {
			if (w == "i" && lhs != "i") { rhs = "i"; break; }
			if (w.find('[') != string::npos && w.find(']') != string::npos) {
				if (w != lhs) { rhs = w; break; }
			}
			if (Variable* v = findVariable(w)) {
				if (!v->getIsList()) {
					if (v->getName() != lhs) { rhs = v->getName(); break; }
				}
			}
		}
	}

	if (rhs.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró valor a comparar en -> " + originalLine));
		return "";
	}

	string code = "while (" + lhs + " " + cmp + " " + rhs + ") {";
	blockDepth++;
	inWhileCtx = true;
	whileVarNameFlat = util.toLowerCase(lhs);
	return code;
}

string CodeConverter::emitIf(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);
	string lowerLine = util.toLowerCase(originalLine);

	string op = "==";
	if (lowerLine.find("mayor o igual") != string::npos) op = ">=";
	else if (lowerLine.find("menor o igual") != string::npos) op = "<=";
	else if (lowerLine.find("distinto de") != string::npos ||
		lowerLine.find("diferente de") != string::npos ||
		lowerLine.find("no igual") != string::npos) op = "!=";
	else if (lowerLine.find("mayor que") != string::npos) op = ">";
	else if (lowerLine.find("menor que") != string::npos) op = "<";
	else if (lowerLine.find("igual que") != string::npos ||
		lowerLine.find("igual a") != string::npos ||
		lowerLine.find("igual") != string::npos) op = "==";

	vector<string> quoted;
	{
		size_t pos = 0;
		while (true) {
			size_t a = originalLine.find('"', pos);
			if (a == string::npos) break;
			size_t b = originalLine.find('"', a + 1);
			if (b == string::npos) break;
			quoted.push_back(originalLine.substr(a, b - a + 1));
			pos = b + 1;
		}
	}

	string leftOperand, rightOperand;


	for (string& w : words) {
		if (isReservedWord(w)) continue;

		string lw = util.toLowerCase(w);
		if (lw == "verdadero" || lw == "true") {
			if (leftOperand.empty()) leftOperand = "true";
			else if (rightOperand.empty()) rightOperand = "true";
			continue;
		}
		if (lw == "falso" || lw == "false") {
			if (leftOperand.empty()) leftOperand = "false";
			else if (rightOperand.empty()) rightOperand = "false";
			continue;
		}

		if (w == "i") {
			if (leftOperand.empty()) leftOperand = "i";
			else if (rightOperand.empty()) rightOperand = "i";
			continue;
		}

		if (w.find('[') != string::npos && w.find(']') != string::npos) {
			if (leftOperand.empty()) leftOperand = w;
			else if (rightOperand.empty()) rightOperand = w;
			continue;
		}

		if (Variable* v = findVariable(w)) {
			if (!v->getIsList()) {
				if (leftOperand.empty()) leftOperand = v->getName();
				else if (rightOperand.empty()) rightOperand = v->getName();
			}
			continue;
		}
	}

	for (const auto& q : quoted) {
		if (leftOperand.empty()) leftOperand = q;
		else if (rightOperand.empty()) { rightOperand = q; break; }
	}

	vector<int> nums = util.extractIntegers(originalLine);
	for (int n : nums) {
		string s = to_string(n);
		if (leftOperand.empty()) leftOperand = s;
		else if (rightOperand.empty()) { rightOperand = s; break; }
	}

	if (leftOperand.empty() || rightOperand.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró valor/variable suficiente para la comparación en -> " + originalLine));
		return "";
	}

	string code = "if (" + leftOperand + " " + op + " " + rightOperand + ") {";
	blockDepth++;
	inIf = true;
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

string CodeConverter::emitCalculate(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string target = "";
	string operation = "";
	bool foundSomething = false;
	bool hasError = false;

	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "asignar" && i + 2 < words.size() && words[i + 1] == "a") {
			target = words[i + 2];
			break;
		}
		if (words[i] == "a" && i + 1 < words.size()) {
			target = words[i + 1];
			break;
		}
	}

	if (target.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó la variable destino en -> " + originalLine));
		return "";
	}

	string op = "";
	if (originalLine.find("sumar") != string::npos) op = "+";
	else if (originalLine.find("restar") != string::npos) op = "-";
	else if (originalLine.find("multiplicar") != string::npos) op = "*";
	else if (originalLine.find("dividir") != string::npos) op = "/";

	for (string& word : words) {
		if (isReservedWord(word)) continue;

		Variable* var = findVariable(word);
		if (var != nullptr) {
			if (word == target) continue;
			if (!operation.empty() && !op.empty()) operation += " " + op + " ";
			operation += var->getName();
			foundSomething = true;
		}
	}

	vector<int> numbers = util.extractIntegers(originalLine);
	for (int n : numbers) {
		if (!operation.empty() && !op.empty()) operation += " " + op + " ";
		operation += to_string(n);
		foundSomething = true;
	}

	if (!foundSomething) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron números o variables para calcular en -> " + originalLine));
		return "";
	}

	Variable* var = findVariable(target);
	if (!var) {
		messages.push_back(Message(MessageType::Error,
			"La variable destino '" + target + "' no fue declarada antes."));
		return "";
	}

	return target + " = " + operation + ";";
}

string CodeConverter::emitCreateStruct(string originalLine) {
	Utility util;
	string lower = util.toLowerCase(originalLine);

	string structName;
	{
		size_t pos = lower.find("crear estructura");
		if (pos != string::npos) {
			vector<string> w = util.splitWords(originalLine.substr(pos + 17));
			if (!w.empty()) structName = w[0];
		}
	}
	if (structName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó el nombre de la estructura en -> " + originalLine));
		return "";
	}

	vector<pair<string, string>> fields;

	size_t pos = 0;
	while (true) {
		size_t a = originalLine.find('(', pos);
		if (a == string::npos) break;
		size_t b = originalLine.find(')', a + 1);
		if (b == string::npos) break;
		string inside = util.toLowerCase(util.trimSpaces(originalLine.substr(a + 1, b - a - 1)));

		string fieldName;
		if (a > 0) {
			string left = originalLine.substr(0, a);
			vector<string> wl = util.splitWords(left);
			if (!wl.empty()) fieldName = wl.back();
		}

		string cppType = "int";
		if (inside.find("texto") != string::npos || inside.find("cadena") != string::npos || inside.find("string") != string::npos)
			cppType = "string";
		else if (inside.find("decimal") != string::npos || inside.find("float") != string::npos || inside.find("double") != string::npos)
			cppType = "float";
		else if (inside.find("entero") != string::npos || inside.find("int") != string::npos)
			cppType = "int";

		if (fieldName.empty()) {
			messages.push_back(Message(MessageType::Warning,
				"No se pudo determinar el nombre del campo para tipo '" + inside + "' en -> " + originalLine));
		}
		else {
			fields.push_back({ fieldName, cppType });
		}
		pos = b + 1;
	}

	if (fields.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontraron campos de la estructura en -> " + originalLine));
		return "";
	}

	structFields[structName] = fields;

	string code = "struct " + structName + " {\n";
	for (auto& f : fields) {
		if (f.second == "string") code += "    std::string " + f.first + ";\n";
		else                      code += "    " + f.second + " " + f.first + ";\n";
	}
	code += "};";
	return code;
}

string CodeConverter::emitCreateCustomList(string originalLine) {
	Utility util;
	string lower = util.toLowerCase(originalLine);

	string typeName;
	{
		size_t pos = lower.find("crear lista de");
		if (pos != string::npos) {
			vector<string> w = util.splitWords(originalLine.substr(pos + 15));
			if (!w.empty()) typeName = w[0];
		}
	}
	if (typeName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó el tipo para la lista en -> " + originalLine));
		return "";
	}

	vector<int> nums = util.extractIntegers(originalLine);
	if (nums.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó tamaño de la lista en -> " + originalLine));
		return "";
	}
	int size = nums[0];

	string listName;
	{
		vector<string> w = util.splitWords(originalLine);
		for (size_t i = 0; i + 1 < w.size(); ++i) {
			if (util.toLowerCase(w[i]) == "llamada") { listName = w[i + 1]; break; }
		}
		if (listName.empty()) {
			string ln = util.toLowerCase(typeName);
			listName = ln + "s";
		}
	}

	Variable newVar(typeName, listName, "", size, true);
	declaredVariables.push_back(newVar);

	return "vector<" + typeName + "> " + listName + "(" + to_string(size) + ");";
}

string CodeConverter::emitInputStructItems(string originalLine) {
	Utility util;
	string lower = util.toLowerCase(originalLine);

	string singularType;
	{
		size_t pos = lower.find("cada ");
		if (pos != string::npos) {
			vector<string> w = util.splitWords(originalLine.substr(pos + 5));
			if (!w.empty()) singularType = w[0];
		}
	}
	if (singularType.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se pudo determinar el tipo en -> " + originalLine));
		return "";
	}

	string listName, structName;
	for (auto& v : declaredVariables) {
		if (v.getIsList()) {
			string t = v.getType();
			if (util.toLowerCase(t) == util.toLowerCase(singularType)) {
				listName = v.getName();
				structName = t;
				break;
			}
		}
	}
	if (listName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró una lista de '" + singularType + "' declarada antes."));
		return "";
	}

	if (structFields.find(structName) == structFields.end()) {
		messages.push_back(Message(MessageType::Error,
			"No hay definición de estructura para '" + structName + "'."));
		return "";
	}
	const auto& fields = structFields[structName];

	string code;
	code += "for (int i = 0; i < " + listName + ".size(); i++) {\n";
	for (auto& f : fields) {
		if (f.second == "string") {
			code += "    getline(cin >> ws, " + listName + "[i]." + f.first + ");\n";
		}
		else {
			code += "    cin >> " + listName + "[i]." + f.first + ";\n";
		}
	}
	code += "}";
	return code;
}

string CodeConverter::emitTraverseStructListPrint(string originalLine) {
	Utility util;
	string lower = util.toLowerCase(originalLine);

	string listName, structName;
	for (auto& v : declaredVariables) {
		if (v.getIsList()) {
			if (structFields.find(v.getType()) != structFields.end()) {
				listName = v.getName();
				structName = v.getType();
				break;
			}
		}
	}
	if (listName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se encontró una lista de estructuras para recorrer en -> " + originalLine));
		return "";
	}

	vector<string> toPrint;
	{
		size_t pos = lower.find("mostrar");
		if (pos != string::npos) {
			string tail = util.toLowerCase(originalLine.substr(pos + 7));
			vector<string> w = util.splitWords(tail);

			unordered_set<string> valid;
			for (auto& p : structFields[structName]) valid.insert(util.toLowerCase(p.first));
			for (auto& tok : w) {
				string lt = util.toLowerCase(tok);
				if (valid.count(lt)) toPrint.push_back(lt);
			}
		}
	}
	if (toPrint.empty()) {
		for (auto& p : structFields[structName]) toPrint.push_back(p.first);
	}

	string code;
	code += "for (int i = 0; i < " + listName + ".size(); i++) {\n";
	code += "    cout";
	for (size_t k = 0; k < toPrint.size(); ++k) {
		string fieldName = toPrint[k];
		for (auto& p : structFields[structName]) {
			if (util.toLowerCase(p.first) == fieldName) {
				code += " << " + listName + "[i]." + p.first;
				if (k + 1 < toPrint.size()) code += " << \" \"";
				break;
			}
		}
	}
	code += " << endl;\n";
	code += "}";
	return code;
}

string CodeConverter::emitInputList(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string listName;
	for (size_t i = 0; i + 1 < words.size(); ++i) {
		if (util.toLowerCase(words[i]) == "lista") {
			listName = words[i + 1];
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

string CodeConverter::emitForEach(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string listName = "";
	for (size_t i = 0; i < words.size(); i++) {
		if (words[i] == "lista" && i + 1 < words.size()) {
			listName = words[i + 1];
			break;
		}
	}

	if (listName.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se especificó la lista en -> " + originalLine));
		return "";
	}

	Variable* listVar = findVariable(listName);
	if (!listVar || !listVar->getIsList()) {
		messages.push_back(Message(MessageType::Error,
			"La lista '" + listName + "' no fue declarada antes."));
		return "";
	}
	blockDepth++;
	inLoopCtx = true;
	loopListName = util.toLowerCase(listName);
	return "for (int i = 0; i < " + listName + ".size(); i++) {";
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
	if (blockDepth > 0) {
		return "} else {";
	}

	blockDepth++;
	return "else {";
}

string CodeConverter::emitDefineFunction(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	string returnType = "int";
	string functionName = "func";
	string paramType = "int";
	string paramName = "x";

	auto isType = [](const string& w) {
		return w == "entero" || w == "int"
			|| w == "decimal" || w == "float"
			|| w == "texto" || w == "string";
		};

	auto applyType = [&](const string& w, string& outType) {
		if (w == "decimal" || w == "float") outType = "float";
		else if (w == "texto" || w == "string") outType = "string";
		else outType = "int";
		};

	for (size_t i = 0; i < words.size(); ++i) {
		const string& w = words[i];

		if (isType(w)) {
			applyType(w, returnType);
		}

		if (w == "funcion") {
			size_t j = i + 1;

			if (j < words.size() && words[j] == "numero") ++j;

			if (j < words.size() && isType(words[j])) {
				applyType(words[j], returnType);
				++j;
			}

			if (j < words.size()) {
				functionName = words[j];
			}
		}

		if (w == "parametro") {
			size_t j = i + 1;

			if (j < words.size() && words[j] == "numero") ++j;

			if (j < words.size() && isType(words[j])) {
				applyType(words[j], paramType);
				++j;
			}

			if (j < words.size()) {
				paramName = words[j];
			}
		}
	}

	declaredVariables.emplace_back(paramType, paramName, "0", 0, false);

	inFunction = true;

	return returnType + " " + functionName + "(" + paramType + " " + paramName + ") {";
}

string CodeConverter::emitReturn(string originalLine) {
	Utility util;
	vector<string> words = util.splitWords(originalLine);

	if (words.size() < 2) {
		messages.push_back(Message(MessageType::Error,
			"No se indicó valor a retornar en -> " + originalLine));
		return "";
	}

	string value = words.back();

	string code;

	while (blockDepth > 0) {
		code += "}\n";
		blockDepth--;
	}

	code += "return " + value + ";";
	if (inFunction) {
		code += "\n}";
		inFunction = false;
	}
	return code;
}

string CodeConverter::emitCallFunction(string originalLine) {
	Utility util;
	string lower = util.toLowerCase(originalLine);

	string target;
	{
		size_t apos = lower.find(" a ");
		if (apos != string::npos) {
			vector<string> w = util.splitWords(originalLine.substr(apos + 3));
			if (!w.empty()) target = w[0];
		}
	}

	string func;
	{
		size_t fpos = lower.find("funcion");
		if (fpos != string::npos) {
			string tail = originalLine.substr(fpos + 7);
			vector<string> w = util.splitWords(tail);
			if (!w.empty()) {
				func = w[0];
				size_t par = func.find('(');
				if (par != string::npos) func = func.substr(0, par);
			}
		}
	}

	string arg;
	{
		size_t l = originalLine.find('(');
		size_t r = (l == string::npos) ? string::npos : originalLine.find(')', l + 1);
		if (l != string::npos && r != string::npos && r > l + 1) {
			arg = util.trimSpaces(originalLine.substr(l + 1, r - l - 1));
		}
	}

	if (target.empty() || func.empty()) {
		messages.push_back(Message(MessageType::Error,
			"No se pudo interpretar la llamada a función en -> " + originalLine));
		return "";
	}

	return target + " = " + func + "(" + arg + ");";
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
	"de","la","el","los","las",
	"un","una","unos","unas",
	"cada","en","con","valor",
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

bool CodeConverter::isReservedWord(string& word) {
	static unordered_set<string> reserved = {
		"sumar","restar","multiplicar","dividir","por",
		"asignar","a","y","si","sino",
		"mientras","repetir","crear","variable","estructura",
		"lista","funcion","retornar","imprimir","mostrar","parametro"
	};
	Utility u;
	string lower = u.toLowerCase(word);
	return reserved.count(lower) > 0;
}

string CodeConverter::closeBlockIfNeeded(string nextLine) {
	if (nextLine.find("sino") != string::npos || nextLine.find("else") != string::npos)
		return "";

	if (blockDepth <= 0) return "";

	Utility util;
	string low = util.toLowerCase(nextLine);

	if (inLoopCtx) {
		bool mentionsIndex = (low.find("[i]") != string::npos);
		bool mentionsListIndex = (!loopListName.empty() && low.find(util.toLowerCase(loopListName) + "[") != string::npos);
		bool mentionsIbare = (low.find(" i ") != string::npos) ||
			(low.rfind("i ", 0) == 0) ||
			(!low.empty() && low.back() == 'i');

		if (mentionsIndex || mentionsListIndex || mentionsIbare) {
			return "";
		}
		blockDepth--;
		inLoopCtx = false;
		loopListName.clear();
		return "}";
	}

	if (inWhileCtx) {
		bool mentionsWhileVar = (!whileVarNameFlat.empty() && low.find(whileVarNameFlat) != string::npos);

		bool looksLikeBody =
			(low.find("sumar") != string::npos) ||
			(low.find("restar") != string::npos) ||
			(low.find("multiplicar") != string::npos) ||
			(low.find("dividir") != string::npos) ||
			(low.find("asignar") != string::npos) ||
			(low.find("imprimir") != string::npos) ||
			(low.find("mostrar") != string::npos) ||
			(low.find("ingresar valor") != string::npos);

		if (mentionsWhileVar || looksLikeBody) {
			return "";
		}
		blockDepth--;
		inWhileCtx = false;
		whileVarNameFlat.clear();
		return "}";
	}

	if (inIf) {
		bool looksLikeBody =
			(low.find("imprimir") != string::npos) ||
			(low.find("mostrar") != string::npos) ||
			(low.find("asignar") != string::npos) ||
			(low.find("sumar") != string::npos) ||
			(low.find("restar") != string::npos) ||
			(low.find("multiplicar") != string::npos) ||
			(low.find("dividir") != string::npos);

		if (looksLikeBody) {
			return "";
		}
		blockDepth--;
		inIf = false;
		return "}";
	}

	blockDepth--;
	return "}";
}










