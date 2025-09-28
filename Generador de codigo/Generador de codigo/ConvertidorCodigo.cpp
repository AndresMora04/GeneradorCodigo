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
	variablesDeclaradas.clear();

	vector<string> lineas = util.dividirEnLineas(textoEntrada);

	for (size_t i = 0; i < lineas.size(); i++) {
		string lineaOriginal = lineas[i];
		string lineaNormalizada = util.convertirAMinusculas(util.recortarEspacios(lineaOriginal));

		if (lineaNormalizada.empty()) continue;

		Intencion intencion = detectarIntencion(lineaNormalizada);
		string codigo = "";

		switch (intencion) {
		case Intencion::Comenzar: codigo = emitirComenzar(lineaOriginal); break;
		case Intencion::Terminar: codigo = emitirTerminar(lineaOriginal); break;
		case Intencion::Suma: codigo = emitirSuma(lineaOriginal); break;
		case Intencion::Resta: codigo = emitirResta(lineaOriginal); break;
		case Intencion::Multiplicacion: codigo = emitirMultiplicacion(lineaOriginal); break;
		case Intencion::Division: codigo = emitirDivision(lineaOriginal); break;
		case Intencion::Imprimir: codigo = emitirImprimir(lineaOriginal); break;
		case Intencion::CrearLista: codigo = emitirCrearLista(lineaOriginal); break;
		case Intencion::AsignarElementoLista: codigo = emitirAsignarElementoLista(lineaOriginal); break;
		case Intencion::Repetir: codigo = emitirRepetir(lineaOriginal); break;
		case Intencion::Mientras: codigo = emitirMientras(lineaOriginal); break;
		case Intencion::CrearVariable: codigo = emitirCrearVariable(lineaOriginal); break;
		case Intencion::Si: codigo = emitirSi(lineaOriginal); break;
		case Intencion::Sino: codigo = emitirSino(lineaOriginal); break;
		case Intencion::IngresarValor: codigo = emitirIngresarValor(lineaOriginal); break;
		case Intencion::IngresarLista: codigo = emitirIngresarLista(lineaOriginal); break;
		case Intencion::RecorrerLista:
			codigo = emitirRecorrerLista(lineaOriginal);
			break;
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
	return codigoGenerado;
}

Intencion ConvertidorCodigo::detectarIntencion(string lineaNormalizada) {
	if (lineaNormalizada.find("comenzar programa") != string::npos) return Intencion::Comenzar;
	if (lineaNormalizada.find("terminar programa") != string::npos) return Intencion::Terminar;

	// Casos más específicos primero
	if (lineaNormalizada.find("ingresar valor de cada") != string::npos &&
		lineaNormalizada.find("lista") != string::npos) {
		return Intencion::IngresarLista;
	}
	if (lineaNormalizada.find("asignar valor") != string::npos &&
		lineaNormalizada.find("lista") != string::npos) {
		return Intencion::AsignarElementoLista;
	}
	if (lineaNormalizada.find("recorrer la lista") != string::npos)
		return Intencion::RecorrerLista;

	// Condiciones de control
	if (lineaNormalizada.rfind("si ", 0) == 0) return Intencion::Si;
	if (lineaNormalizada == "sino" || lineaNormalizada.find("sino") != string::npos) return Intencion::Sino;
	if (lineaNormalizada.find("repetir") != string::npos) return Intencion::Repetir;
	if (lineaNormalizada.find("mientras") != string::npos) return Intencion::Mientras;

	// Variables
	if (lineaNormalizada.find("crear variable") != string::npos) return Intencion::CrearVariable;
	if (lineaNormalizada.find("ingresar valor") != string::npos) return Intencion::IngresarValor;

	// Operaciones matemáticas (más genéricas)
	if (lineaNormalizada.find("sumar") != string::npos) return Intencion::Suma;
	if (lineaNormalizada.find("restar") != string::npos) return Intencion::Resta;
	if (lineaNormalizada.find("multiplicar") != string::npos) return Intencion::Multiplicacion;
	if (lineaNormalizada.find("dividir") != string::npos) return Intencion::Division;

	// Crear lista
	if (lineaNormalizada.find("crear lista") != string::npos ||
		lineaNormalizada.find("crear una lista") != string::npos) {
		return Intencion::CrearLista;
	}

	// Imprimir
	if (lineaNormalizada.find("imprimir") != string::npos ||
		lineaNormalizada.find("mostrar") != string::npos) return Intencion::Imprimir;

	return Intencion::Desconocida;
}

string ConvertidorCodigo::emitirComenzar(string lineaOriginal)
{
	return "#include <iostream>\n#include <vector>\nusing namespace std;\n\nint main() {";
}

string ConvertidorCodigo::emitirTerminar(string lineaOriginal)
{
	return "return 0;\n}";
}

string ConvertidorCodigo::emitirSuma(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string destino = "";
	string operacion = "";
	bool encontradoAlgo = false;

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "a" && i + 1 < palabras.size()) {
			destino = palabras[i + 1];
		}
		if (palabras[i] == "asignar" && i + 2 < palabras.size() && palabras[i + 1] == "a") {
			destino = palabras[i + 2];
		}
	}

	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr) {
			if (palabra == destino) continue;
			if (!operacion.empty()) operacion += " + ";
			operacion += var->nombre;
			encontradoAlgo = true;
		}
	}

	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	for (int n : numeros) {
		if (!operacion.empty()) operacion += " + ";
		operacion += to_string(n);
		encontradoAlgo = true;
	}

	if (!encontradoAlgo) {
		return "// Error: no se encontraron números o variables para sumar en -> " + lineaOriginal;
	}

	if (!destino.empty()) {
		Variable* var = buscarVariable(destino);
		if (!var) {
			return "// Error: la variable '" + destino + "' no fue declarada antes.";
		}
		return destino + " = " + operacion + ";";
	}
	else {
		string nombreVar = "resultadoSuma" + to_string(contadorSuma++);
		return "int " + nombreVar + " = " + operacion + ";";
	}
}

string ConvertidorCodigo::emitirResta(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string destino = "";
	string operacion = "";
	bool encontradoAlgo = false;

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "a" && i + 1 < palabras.size()) {
			destino = palabras[i + 1];
		}
		if (palabras[i] == "asignar" && i + 2 < palabras.size() && palabras[i + 1] == "a") {
			destino = palabras[i + 2];
		}
	}

	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr) {
			if (palabra == destino) continue;
			if (!operacion.empty()) operacion += " - ";
			operacion += var->nombre;
			encontradoAlgo = true;
		}
	}

	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	for (int n : numeros) {
		if (!operacion.empty()) operacion += " - ";
		operacion += to_string(n);
		encontradoAlgo = true;
	}

	if (!encontradoAlgo) {
		return "// Error: no se encontraron números o variables para restar en -> " + lineaOriginal;
	}

	if (!destino.empty()) {
		Variable* var = buscarVariable(destino);
		if (!var) {
			return "// Error: la variable '" + destino + "' no fue declarada antes.";
		}
		return destino + " = " + operacion + ";";
	}
	else {
		string nombreVar = "resultadoResta" + to_string(contadorResta++);
		return "int " + nombreVar + " = " + operacion + ";";
	}
}

string ConvertidorCodigo::emitirMultiplicacion(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string destino = "";
	string operacion = "";
	bool encontradoAlgo = false;

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "a" && i + 1 < palabras.size()) {
			destino = palabras[i + 1];
		}
		if (palabras[i] == "asignar" && i + 2 < palabras.size() && palabras[i + 1] == "a") {
			destino = palabras[i + 2];
		}
	}

	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr) {
			if (palabra == destino) continue;
			if (!operacion.empty()) operacion += " * ";
			operacion += var->nombre;
			encontradoAlgo = true;
		}
	}

	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	for (int n : numeros) {
		if (!operacion.empty()) operacion += " * ";
		operacion += to_string(n);
		encontradoAlgo = true;
	}

	if (!encontradoAlgo) {
		return "// Error: no se encontraron números o variables para multiplicar en -> " + lineaOriginal;
	}

	if (!destino.empty()) {
		Variable* var = buscarVariable(destino);
		if (!var) {
			return "// Error: la variable '" + destino + "' no fue declarada antes.";
		}
		return destino + " = " + operacion + ";";
	}
	else {
		string nombreVar = "resultadoMultiplicacion" + to_string(contadorMultiplicacion++);
		return "int " + nombreVar + " = " + operacion + ";";
	}
}

string ConvertidorCodigo::emitirDivision(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string destino = "";
	string operacion = "";
	bool encontradoAlgo = false;

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "a" && i + 1 < palabras.size()) {
			destino = palabras[i + 1];
			continue;
		}
		if (palabras[i] == "asignar" && i + 2 < palabras.size() && palabras[i + 1] == "a") {
			destino = palabras[i + 2];
			continue;
		}
	}

	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr) {
			if (var->nombre == destino) continue;
			if (!operacion.empty()) operacion += " / ";
			operacion += var->nombre;
			encontradoAlgo = true;
		}
	}

	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	for (int n : numeros) {
		if (!operacion.empty()) operacion += " / ";
		operacion += to_string(n);
		encontradoAlgo = true;
	}

	if (!encontradoAlgo) {
		return "// Error: no se encontraron números o variables para dividir en -> " + lineaOriginal;
	}

	if (!destino.empty()) {
		Variable* var = buscarVariable(destino);
		if (!var) {
			return "// Error: la variable '" + destino + "' no fue declarada antes.";
		}
		return destino + " = " + operacion + ";";
	}
	else {
		string nombreVar = "resultadoDivision" + to_string(contadorDivision++);
		return "int " + nombreVar + " = " + operacion + ";";
	}
}

string ConvertidorCodigo::emitirImprimir(string lineaOriginal) {
	Utilidad util;
	vector<string> partes;

	size_t pos = 0;
	while ((pos = lineaOriginal.find("\"", pos)) != string::npos) {
		size_t fin = lineaOriginal.find("\"", pos + 1);
		if (fin == string::npos) break;
		string texto = lineaOriginal.substr(pos, fin - pos + 1);
		partes.push_back(texto);
		pos = fin + 1;
	}

	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);
	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr) {
			partes.push_back(var->nombre);
		}
	}

	if (partes.empty()) {
		return "// Error: no se pudo determinar qué imprimir en -> " + lineaOriginal;
	}

	string codigo = "cout";
	for (const string& p : partes) {
		codigo += " << " + p;
	}
	codigo += " << endl;";

	return codigo;
}

string ConvertidorCodigo::emitirCrearLista(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	string tipo = "";
	for (const string& p : palabras) {
		if (p == "entero" || p == "enteros" || p == "int") tipo = "int";
		else if (p == "decimal" || p == "decimales" || p == "float") tipo = "float";
		else if (p == "texto" || p == "string" || p == "strings") tipo = "string";
	}
	if (tipo.empty()) {
		return "// Error: no se indicó tipo de la lista en -> " + lineaOriginal;
	}

	if (numeros.empty()) {
		return "// Error: no se indicó tamaño de la lista en -> " + lineaOriginal;
	}
	int tamano = numeros[0];

	string nombreLista = "";
	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "llamada" && i + 1 < palabras.size()) {
			nombreLista = palabras[i + 1];
			break;
		}
	}
	if (nombreLista.empty()) {
		if (!palabras.empty()) {
			nombreLista = palabras.back();
		}
	}

	if (nombreLista == "enteros" || nombreLista == "decimales" ||
		nombreLista == "strings" || nombreLista == "elementos" ||
		nombreLista == "valores") {
		return "// Error: falta nombre para la lista en -> " + lineaOriginal;
	}

	Variable nueva;
	nueva.tipo = tipo;
	nueva.nombre = nombreLista;
	nueva.valor = "";
	nueva.tamano = tamano;
	nueva.esLista = true;
	variablesDeclaradas.push_back(nueva);

	return "vector<" + tipo + "> " + nombreLista + "(" + to_string(tamano) + ");";
}

string ConvertidorCodigo::emitirAsignarElementoLista(string lineaOriginal)
{
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string nombreLista = "";
	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "lista" && i + 1 < palabras.size()) {
			nombreLista = palabras[i + 1];
		}
	}
	if (nombreLista.empty()) {
		return "// Error: falta nombre de lista en -> " + lineaOriginal;
	}

	Variable* listaEncontrada = nullptr;
	for (auto& v : variablesDeclaradas) {
		if (v.nombre == nombreLista && v.esLista) {
			listaEncontrada = &v;
			break;
		}
	}
	if (!listaEncontrada) {
		return "// Error: la lista '" + nombreLista + "' no fue declarada antes.";
	}

	int indice = -1;
	for (size_t i = 0; i < palabras.size(); i++) {
		string p = palabras[i];
		if (p == "primer" || p == "primero") indice = 0;
		else if (p == "segundo") indice = 1;
		else if (p == "tercero") indice = 2;
		else if (p == "cuarto") indice = 3;
		else if (p == "quinto") indice = 4;
		else if (p == "sexto") indice = 5;
		else if (p == "séptimo" || p == "septimo") indice = 6;
		else if (p == "octavo") indice = 7;
		else if (p == "noveno") indice = 8;
		else if (p == "décimo" || p == "decimo") indice = 9;
	}
	if (indice == -1) {
		vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
		if (!numeros.empty()) {
			indice = numeros[0] - 1;
		}
	}
	if (indice == -1) {
		return "// Error: índice no reconocido en -> " + lineaOriginal;
	}

	if (indice < 0 || indice >= listaEncontrada->tamano) {
		return "// Error: el índice " + to_string(indice + 1) +
			" excede el tamaño de la lista '" + nombreLista +
			"' (máx: " + to_string(listaEncontrada->tamano) + ").";
	}

	string valorAsignado = "";
	if (listaEncontrada->tipo == "string") {
		size_t posComilla = lineaOriginal.find("\"");
		if (posComilla != string::npos) {
			size_t ultimaComilla = lineaOriginal.find_last_of("\"");
			if (ultimaComilla > posComilla) {
				valorAsignado = lineaOriginal.substr(posComilla, ultimaComilla - posComilla + 1);
			}
		}
		if (valorAsignado.empty()) {
			return "// Error: la lista '" + nombreLista + "' es de tipo string, pero no se encontró texto válido en -> " + lineaOriginal;
		}
	}
	else {
		vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
		if (numeros.empty()) {
			return "// Error: la lista '" + nombreLista + "' es de tipo " + listaEncontrada->tipo + ", pero no se encontró número en -> " + lineaOriginal;
		}
		valorAsignado = to_string(numeros[0]);
	}

	return nombreLista + "[" + to_string(indice) + "] = " + valorAsignado + ";";
}



string ConvertidorCodigo::emitirRepetir(string lineaOriginal) {
	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.empty()) {
		return "// Error: no se encontró cuántas veces repetir -> " + lineaOriginal;
	}

	int repeticiones = numeros[0];

	size_t posComilla = lineaOriginal.find("\"");
	string mensaje = (posComilla != string::npos)
		? lineaOriginal.substr(posComilla, lineaOriginal.find_last_of("\"") - posComilla + 1)
		: "\"Iteracion\"";

	string codigo;
	codigo += "for (int i = 0; i < " + to_string(repeticiones) + "; i++) {\n";
	codigo += "    cout << " + mensaje + " << endl;\n";
	codigo += "}";
	return codigo;
}

string ConvertidorCodigo::emitirMientras(string lineaOriginal) {
	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.empty()) {
		return "// Error: no se encontró un número de condición en -> " + lineaOriginal;
	}

	int limite = numeros[0];

	string nombreVar = "numero";
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);
	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr && !var->esLista) {
			nombreVar = var->nombre;
			break;
		}
	}

	Variable* var = buscarVariable(nombreVar);
	if (var == nullptr) {
		return "// Error: la variable '" + nombreVar + "' no fue declarada antes.";
	}

	string incremento = "1";
	if (numeros.size() >= 2) {
		incremento = to_string(numeros[1]);
	}

	string codigo;
	codigo += "while (" + nombreVar + " < " + to_string(limite) + ") {\n";
	codigo += "    " + nombreVar + " += " + incremento + ";\n";
	codigo += "}";
	return codigo;
}

string ConvertidorCodigo::emitirSi(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

	if (numeros.empty()) {
		return "// Error: no se encontró número de comparación en -> " + lineaOriginal;
	}

	string variable = "numero";
	string operador = ">";

	for (const string& palabra : palabras) {
		Variable* var = buscarVariable(palabra);
		if (var != nullptr && !var->esLista) {
			variable = var->nombre;
			break;
		}
	}

	if (lineaOriginal.find("mayor que") != string::npos) operador = ">";
	else if (lineaOriginal.find("menor que") != string::npos) operador = "<";
	else if (lineaOriginal.find("igual a") != string::npos) operador = "==";

	int valor = numeros[0];

	string codigo;
	codigo += "if (" + variable + " " + operador + " " + to_string(valor) + ") {";
	dentroDeBloque = true;
	return codigo;
}

string ConvertidorCodigo::emitirCrearVariable(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string tipo = "int";
	string nombre = "variable";
	string valor = "0";

	bool tipoDetectado = false;

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "decimal") {
			tipo = "float";
			tipoDetectado = true;
			if (i + 1 < palabras.size()) nombre = palabras[i + 1];
		}
		else if (palabras[i] == "entero" || palabras[i] == "int") {
			tipo = "int";
			tipoDetectado = true;
			if (i + 1 < palabras.size()) nombre = palabras[i + 1];
		}
		else if (palabras[i] == "texto" || palabras[i] == "string") {
			tipo = "string";
			tipoDetectado = true;
			if (i + 1 < palabras.size()) nombre = palabras[i + 1];
		}
	}

	if (!tipoDetectado) {
		for (size_t i = 0; i < palabras.size(); i++) {
			if (palabras[i] == "variable" && i + 1 < palabras.size()) {
				nombre = palabras[i + 1];
				break;
			}
		}
	}

	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	if (!numeros.empty()) {
		valor = to_string(numeros[0]);
	}

	if (tipo == "string") {
		valor = "\"\"";
		size_t posComilla = lineaOriginal.find("\"");
		if (posComilla != string::npos) {
			size_t ultimaComilla = lineaOriginal.find_last_of("\"");
			if (ultimaComilla > posComilla) {
				valor = lineaOriginal.substr(posComilla, ultimaComilla - posComilla + 1);
			}
		}
	}

	Variable nueva;
	nueva.tipo = tipo;
	nueva.nombre = nombre;
	nueva.valor = valor;
	variablesDeclaradas.push_back(nueva);

	return tipo + " " + nombre + " = " + valor + ";";
}

string ConvertidorCodigo::emitirIngresarLista(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string nombreLista = "";
	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "lista" && i + 1 < palabras.size()) {
			nombreLista = palabras.back();
			break;
		}
	}

	if (nombreLista.empty()) {
		return "// Error: no se especificó el nombre de la lista en -> " + lineaOriginal;
	}

	Variable* listaEncontrada = buscarVariable(nombreLista);
	if (!listaEncontrada || !listaEncontrada->esLista) {
		return "// Error: la lista '" + nombreLista + "' no fue declarada antes.";
	}

	string codigo;
	codigo += "for (int i = 0; i < " + nombreLista + ".size(); i++) {\n";
	codigo += "    cin >> " + nombreLista + "[i];\n";
	codigo += "}";
	return codigo;
}

string ConvertidorCodigo::emitirRecorrerLista(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string nombreLista = "";
	string variableAcumuladora = "";

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "lista" && i + 1 < palabras.size()) {
			nombreLista = palabras[i + 1];
		}
	}

	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "en" && i + 1 < palabras.size()) {
			variableAcumuladora = palabras[i + 1];
		}
	}

	if (nombreLista.empty() || variableAcumuladora.empty()) {
		return "// Error: no se pudo determinar lista o variable acumuladora en -> " + lineaOriginal;
	}

	Variable* listaEncontrada = buscarVariable(nombreLista);
	Variable* acumuladorEncontrado = buscarVariable(variableAcumuladora);

	if (!listaEncontrada || !listaEncontrada->esLista) {
		return "// Error: la lista '" + nombreLista + "' no fue declarada antes.";
	}
	if (!acumuladorEncontrado) {
		return "// Error: la variable acumuladora '" + variableAcumuladora + "' no fue declarada antes.";
	}

	return "for (int i = 0; i < " + nombreLista + ".size(); i++) {\n" +
		"    " + variableAcumuladora + " += " + nombreLista + "[i];\n" +
		"}";
}

string ConvertidorCodigo::emitirIngresarValor(string lineaOriginal) {
	Utilidad util;
	vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

	string nombreVariable = "";
	for (size_t i = 0; i < palabras.size(); i++) {
		if (palabras[i] == "valor" && i + 1 < palabras.size()) {
			nombreVariable = palabras[i + 1];
			break;
		}
	}

	if (nombreVariable.empty()) {
		return "// Error: no se indicó variable en -> " + lineaOriginal;
	}

	Variable* var = nullptr;
	for (auto& v : variablesDeclaradas) {
		if (v.nombre == nombreVariable) {
			var = &v;
			break;
		}
	}
	if (!var) {
		return "// Error: la variable '" + nombreVariable + "' no fue declarada antes.";
	}

	return "cin >> " + nombreVariable + ";";
}

string ConvertidorCodigo::emitirSino(string lineaOriginal) {
	dentroDeBloque = true;
	return "else {";
}


int ConvertidorCodigo::obtenerIndiceDesdePalabraONumero(string lineaOriginal, vector<string> palabras)
{
	for (size_t i = 0; i < palabras.size(); i++) {
		string p = palabras[i];
		if (p == "primer" || p == "primero") return 0;
		if (p == "segundo") return 1;
		if (p == "tercero") return 2;
		if (p == "cuarto") return 3;
		if (p == "quinto") return 4;
		if (p == "sexto") return 5;
		if (p == "séptimo" || p == "septimo") return 6;
		if (p == "octavo") return 7;
		if (p == "noveno") return 8;
		if (p == "décimo" || p == "decimo") return 9;
	}

	Utilidad util;
	vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);
	if (!numeros.empty()) {
		return numeros[0] - 1;
	}

	return -1;
}

string ConvertidorCodigo::normalizarPalabra(const string& palabra) {
	static vector<string> stopwords = {
	"de", "la", "el", "los", "las",
	"un", "una", "unos", "unas",
	"cada", "en", "con", "valor"
	};

	for (const auto& stop : stopwords) {
		if (palabra == stop) return "";
	}
	return palabra;
}

Variable* ConvertidorCodigo::buscarVariable(string nombre) {
	string limpio = normalizarPalabra(nombre);
	if (limpio.empty()) return nullptr;

	Utilidad util;
	string nombreNormalizado = util.convertirAMinusculas(limpio);

	for (auto& v : variablesDeclaradas) {
		if (util.convertirAMinusculas(v.nombre) == nombreNormalizado) {
			return &v;
		}
	}
	return nullptr;
}
