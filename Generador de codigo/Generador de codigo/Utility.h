#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Utility {
public:
    static string toLowerCase(string text);
    static string trimSpaces(string text);
    static vector<string> splitLines(string text);
    static vector<int> extractIntegers(string line);
    static vector<string> splitWords(string line);
};