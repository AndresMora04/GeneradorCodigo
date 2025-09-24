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
        case Intencion::Suma: codigo = emitirSuma(lineaOriginal); break;
        case Intencion::Resta: codigo = emitirResta(lineaOriginal); break;
        case Intencion::Multiplicacion: codigo = emitirMultiplicacion(lineaOriginal); break;
        case Intencion::Division: codigo = emitirDivision(lineaOriginal); break;
        case Intencion::Imprimir: codigo = emitirImprimir(lineaOriginal); break;
        case Intencion::CrearLista: codigo = emitirCrearLista(lineaOriginal); break;
        case Intencion::Repetir: codigo = emitirRepetir(lineaOriginal); break;
        case Intencion::Mientras: codigo = emitirMientras(lineaOriginal); break;
        case Intencion::CrearVariable: codigo = emitirCrearVariable(lineaOriginal); break;
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

Intencion ConvertidorCodigo::detectarIntencion(string lineaNormalizada) {
    if (lineaNormalizada.find("repetir") != string::npos) return Intencion::Repetir;
    if (lineaNormalizada.find("mientras") != string::npos) return Intencion::Mientras;
    if (lineaNormalizada.find("crear variable") != string::npos) return Intencion::CrearVariable;
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

    string nombreVar = "resultadoSuma" + to_string(contadorSuma++);
    string codigo = "int " + nombreVar + " = ";

    for (size_t i = 0; i < numeros.size(); i++) {
        codigo += to_string(numeros[i]);
        if (i < numeros.size() - 1) codigo += " + ";
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

    string nombreVar = "resultadoResta" + to_string(contadorResta++);
    string codigo = "int " + nombreVar + " = " + to_string(numeros[0]);

    for (size_t i = 1; i < numeros.size(); i++) {
        codigo += " - " + to_string(numeros[i]);
    }
    codigo += ";";
    return codigo;
}

string ConvertidorCodigo::emitirMultiplicacion(string lineaOriginal) {
    Utilidad util;
    vector<int> numeros = util.extraerNumerosEnteros(lineaOriginal);

    if (numeros.size() < 2) {
        return "// Error: se necesitan al menos 2 números para multiplicar en -> " + lineaOriginal;
    }

    string nombreVar = "resultadoMultiplicacion" + to_string(contadorMultiplicacion++);
    string codigo = "int " + nombreVar + " = " + to_string(numeros[0]);

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

    string nombreVar = "resultadoDivision" + to_string(contadorDivision++);
    string codigo = "int " + nombreVar + " = " + to_string(numeros[0]);

    for (size_t i = 1; i < numeros.size(); i++) {
        codigo += " / " + to_string(numeros[i]);
    }
    codigo += ";";
    return codigo;
}

string ConvertidorCodigo::emitirImprimir(string lineaOriginal) {
    size_t posicionComilla = lineaOriginal.find("\"");

    if (posicionComilla != string::npos) {
        size_t ultimaComilla = lineaOriginal.find_last_of("\"");
        if (ultimaComilla > posicionComilla) {
            string mensaje = lineaOriginal.substr(posicionComilla, ultimaComilla - posicionComilla + 1);
            return "cout << " + mensaje + " << endl;";
        }
    }

    vector<string> palabras = Utilidad::dividirEnPalabras(lineaOriginal);
    for (const string& palabra : palabras) {
        if (!buscarVariable(palabra).empty()) {
            return "cout << " + palabra + " << endl;";
        }
    }

    return "// Error: no se pudo determinar qué imprimir en -> " + lineaOriginal;
}

string ConvertidorCodigo::emitirCrearLista(string lineaOriginal) {
    return "// Código para CREAR LISTA pendiente -> " + lineaOriginal;
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
        if (!buscarVariable(palabra).empty()) {
            nombreVar = palabra;
            break;
        }
    }

    if (buscarVariable(nombreVar).empty()) {
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

string ConvertidorCodigo::emitirCrearVariable(string lineaOriginal) {
    Utilidad util;
    vector<string> palabras = util.dividirEnPalabras(lineaOriginal);

    string tipo = "int";
    string nombre = "variable";
    string valor = "0";

    for (size_t i = 0; i < palabras.size(); i++) {
        if (palabras[i] == "decimal") {
            tipo = "float";
            if (i + 1 < palabras.size()) nombre = palabras[i + 1]; // nombre después de "decimal"
        }
        else if (palabras[i] == "entero") {
            tipo = "int";
            if (i + 1 < palabras.size()) nombre = palabras[i + 1]; // nombre después de "entero"
        }
        else if (palabras[i] == "texto" || palabras[i] == "string") {
            tipo = "string";
            if (i + 1 < palabras.size()) nombre = palabras[i + 1]; // nombre después de "texto"
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


string ConvertidorCodigo::buscarVariable(string nombre) {
    for (const auto& v : variablesDeclaradas) {
        if (v.nombre == nombre) {
            return v.nombre;
        }
    }
    return "";
}