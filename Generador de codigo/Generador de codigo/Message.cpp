#include "Message.h"

Message::Message(MessageType type, string content)
	: type(type), text(content) {
}

MessageType Message::getType() {
	return type;
}
string Message::getText() {
	return text;
}

string Message::typeAsString() {
	switch (type) {
	case MessageType::Information: return "Informacion";
	case MessageType::Warning:     return "Advertencia";
	case MessageType::Error:       return "Error";
	default:                       return "Desconocido";
	}
}

string Message::toString() {
	return "[" + typeAsString() + "] " + text;
}

ostream& operator<<(ostream& os, Message& msg) {
	os << msg.toString();
	return os;
}