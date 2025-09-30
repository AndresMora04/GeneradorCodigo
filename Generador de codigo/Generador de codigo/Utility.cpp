#include "Utility.h"

string Utility::toLowerCase(string text) {
    string result = text;
    for (char& c : result) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

string Utility::trimSpaces(string text) {
    int start = 0;
    int end = static_cast<int>(text.size()) - 1;

    while (start <= end && isspace(static_cast<unsigned char>(text[start]))) {
        start++;
    }
    while (end >= start && isspace(static_cast<unsigned char>(text[end]))) {
        end--;
    }

    if (start > end) return "";
    return text.substr(start, end - start + 1);
}

vector<string> Utility::splitLines(string text) {
    vector<string> lines;
    string currentLine;

    for (char c : text) {
        if (c == '\n') {
            lines.push_back(trimSpaces(currentLine));
            currentLine.clear();
        }
        else {
            currentLine += c;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(trimSpaces(currentLine));
    }

    return lines;
}

vector<int> Utility::extractIntegers(string line) {
    vector<int> numbers;
    string currentNumber;

    for (char c : line) {
        if (isdigit(static_cast<unsigned char>(c))) {
            currentNumber += c;
        }
        else {
            if (!currentNumber.empty()) {
                numbers.push_back(stoi(currentNumber));
                currentNumber.clear();
            }
        }
    }

    if (!currentNumber.empty()) {
        numbers.push_back(stoi(currentNumber));
    }

    return numbers;
}

vector<string> Utility::splitWords(string line) {
    vector<string> words;
    string currentWord;

    for (char c : line) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord.clear();
            }
        }
        else {
            currentWord += c;
        }
    }

    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }

    return words;
}