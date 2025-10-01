#pragma once
#include <string>
#include <vector>
#include "Utility.h"
#include "Message.h"
#include "Intention.h"
#include "Variable.h"

using namespace std;

class CodeConverter
{
private:
    string inputText;
    string generatedCode;
    vector<Message> messages;
    vector<Variable> declaredVariables;

    int sumCounter = 0;
    int subtractCounter = 0;
    int multiplyCounter = 0;
    int divideCounter = 0;
    int listCounter = 0;

    bool insideBlock = false;

    Intention detectIntention(string normalizedLine);
    string normalizeWord(const string& word);
    Variable* findVariable(string name);
    int getIndexFromWordOrNumber(string originalLine, vector<string> words);
    bool isReservedWord(string& word);

	string emitDefineFunction(string originalLine);
	string emitReturn(string originalLine);
	string emitCallFunction(string originalLine);

    string emitStart(string originalLine);
    string emitEnd(string originalLine);

    string emitSum(string originalLine);
    string emitSubtract(string originalLine);
    string emitMultiply(string originalLine);
    string emitDivide(string originalLine);

    string emitPrint(string originalLine);

    string emitCreateVariable(string originalLine);
    string emitInputValue(string originalLine);

    string emitCreateList(string originalLine);
    string emitInputList(string originalLine);
    string emitAssignListElement(string originalLine);
    string emitIterateList(string originalLine);

    string emitIf(string originalLine);
    string emitElse(string originalLine);

    string emitRepeat(string originalLine);
    string emitWhile(string originalLine);

public:
    CodeConverter();
    CodeConverter(string inputText);

    void setInputText(string newText);
    string getInputText();
    string getGeneratedCode();
    vector<Message> getMessages();

    bool process();
    string buildProgram();
};