#pragma once
#include <string>
#include <iostream>
using namespace std;

enum class MessageType {
    Information,
    Warning,
    Error
};

class Message {
private:
    MessageType type;
    string text;

public:
    Message(MessageType type, string content);

    MessageType getType();
    string getText();

    string typeAsString();
    string toString();

    friend ostream& operator<<(ostream& os, Message& msg);
};