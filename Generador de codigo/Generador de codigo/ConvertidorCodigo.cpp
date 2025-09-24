#include "ConvertidorCodigo.h"

ConvertidorCodigo::ConvertidorCodigo() : textoEntrada(""), codigoGenerado("") {}

ConvertidorCodigo::ConvertidorCodigo(string textoEntrada)
	: textoEntrada(textoEntrada), codigoGenerado("") {
}

void ConvertidorCodigo::setTexto(string nuevoTexto) {
	textoEntrada = nuevoTexto;
}

string ConvertidorCodigo::getTexto() {
	return textoEntrada;
}

string ConvertidorCodigo::getCodigoGenerado() {
	return codigoGenerado;
}

vector<Mensaje> ConvertidorCodigo::getMensajes() {
	return mensajes;
}

bool ConvertidorCodigo::procesar() {
	Utilidad util;
	codigoGenerado.clear();
	mensajes.clear();

	vector<string> lineas = util.dividirEnLineas(textoEntrada);

	for (size_t i = 0; i < lineas.size(); i++) {
		string lineaOriginal = lineas[i];
		string lineaNormalizada = util.convertirAMinusculas(util.recortarEspacios(lineaOriginal));

		if (lineaNormalizada.empty()) continue;

		Intencion intencion = detectarIntencion(lineaNormalizada);
		string codigo = "";

		switch (intencion) {
		case Intencion::Suma: codigo = emitirSuma(lineaOriginal); break;
		case Intencion::Resta: codigo = emitirResta(lineaOriginal); break;
		case Intencion::Multiplicacion: codigo = emitirMultiplicacion(lineaOriginal); break;
		case Intencion::Division: codigo = emitirDivision(lineaOriginal); break;
		case Intencion::Imprimir: codigo = emitirImprimir(lineaOriginal); break;
		case Intencion::CrearLista: codigo = emitirCrearLista(lineaOriginal); break;
		default:
			mensajes.push_back(Mensaje(TipoMensaje::Advertencia, "No se reconoció la instrucción en la línea: " + lineaOriginal));
			break;
		}

		if (!codigo.empty()) {
			codigoGenerado += codigo + "\n";
			mensajes.push_back(Mensaje(TipoMensaje::Informacion, "Se procesó la línea: " + lineaOriginal));
		}
	}

	return !codigoGenerado.empty();
}

string ConvertidorCodigo::construirPrograma() {
	string programa =
		"#include <iostream>\n"
		"using namespace std;\n\n"
		"int main() {\n";

	programa += codigoGenerado;

	programa += "    return 0;\n}\n";
	return programa;
}

// --- Detectar intención ---
Intencion ConvertidorCodigo::detectarIntencion(string lineaNormalizada) {
	if (lineaNormalizada.find("sumar") != string::npos) return Intencion::Suma;
	if (lineaNormalizada.find("restar") != string::npos) return Intencion::Resta;
	if (lineaNormalizada.find("multiplicar") != string::npos) return Intencion::Multiplicacion;
	if (lineaNormalizada.find("dividir") != string::npos) return Intencion::Division;
	if (lineaNormalizada.find("imprimir") != string::npos || lineaNormalizada.find("mostrar") != string::npos) return Intencion::Imprimir;
	if (lineaNormalizada.find("crear lista") != string::npos) return Intencion::CrearLista;

	return Intencion::Desconocida;
}

string ConvertidorCodigo::emitirSuma(string lineaOriginal) {
	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.empty()) {
		return "// Error: no se encontraron números para sumar en -> " + lineaOriginal;
	}

	string codigo = "int resultado = ";
	for (size_t i = 0; i < numeros.size(); i++) {
		codigo += to_string(numeros[i]);
		if (i < numeros.size() - 1) {
			codigo += " + ";
		}
	}
	codigo += ";";
	return codigo;
}

string ConvertidorCodigo::emitirResta(string lineaOriginal) {
	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.size() < 2) {
		return "// Error: se necesitan al menos 2 números para restar en -> " + lineaOriginal;
	}

	string codigo = "int resultado = " + to_string(numeros[0]);
	for (size_t i = 1; i < numeros.size(); i++) {
		codigo += " - " + to_string(numeros[i]);
	}
	codigo += ";";
	return codigo;
}

string ConvertidorCodigo::emitirMultiplicacion(string lineaOriginal) {
	vector<int> numeros = Utilidad::extraerNumerosEnteros(lineaOriginal);

	if (numeros.size() < 2) {
		return "// Error: se necesitan al menos 2 números para multiplicar en -> " + lineaOriginal;
	}

	string codigo = "int resultado = " + to_string(numeros[0]);
	for (size_t i = 1; i < numeros.size(); i++) {
		codigo += " * " + to_string(numeros[i]);
	}
	codigo += ";";
	return codigo;
}

string ConvertidorCodigo::emitirDivision(string lineaOriginal) {
	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.size() < 2) {
		return "// Error: se necesitan al menos 2 números para dividir en -> " + lineaOriginal;
	}

	string codigo = "int resultado = " + to_string(numeros[0]);
	for (size_t i = 1; i < numeros.size(); i++) {
		codigo += " / " + to_string(numeros[i]);
	}
	codigo += ";";
	return codigo;
}

string ConvertidorCodigo::emitirImprimir(string lineaOriginal){
	size_t posicionComilla = lineaOriginal.find("\"");

	if (posicionComilla != string::npos) {
		size_t ultimaComilla = lineaOriginal.find_last_of("\"");
		if (ultimaComilla > posicionComilla) {
			string mensaje = lineaOriginal.substr(posicionComilla, ultimaComilla - posicionComilla + 1);
			return "cout << " + mensaje + " << endl;";
		}
	}

	if (lineaOriginal.find("resultado") != string::npos) {
		return "cout << resultado << endl;";
	}
	if (lineaOriginal.find("numero") != string::npos) {
		return "cout << numero << endl;";
	}

	return "// Error: no se pudo determinar qué imprimir en -> " + lineaOriginal;
}

string ConvertidorCodigo::emitirCrearLista(string lineaOriginal) {
	return "// Código para CREAR LISTA pendiente -> " + lineaOriginal;
}