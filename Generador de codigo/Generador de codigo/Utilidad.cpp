#include "Utilidad.h"

string Utilidad::convertirAMinusculas(string texto) {
    string resultado = texto;
    for (size_t i = 0; i < resultado.size(); i++) {
        char caracter = resultado[i];
        if (caracter >= 'A' && caracter <= 'Z') {
            resultado[i] = caracter + 32;
        }
    }
    return resultado;
}

string Utilidad::recortarEspacios(string texto) {
    int inicio = 0;
    int fin = texto.size() - 1;

    while (inicio <= fin && (texto[inicio] == ' ' || texto[inicio] == '\t' || texto[inicio] == '\n' || texto[inicio] == '\r')) {
        inicio++;
    }

    while (fin >= inicio && (texto[fin] == ' ' || texto[fin] == '\t' || texto[fin] == '\n' || texto[fin] == '\r')) {
        fin--;
    }

    if (inicio > fin) return "";
    return texto.substr(inicio, fin - inicio + 1);
}

vector<string> Utilidad::dividirEnLineas(string texto) {
    vector<string> lineas;
    string lineaActual = "";

    for (size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {
            lineas.push_back(recortarEspacios(lineaActual));
            lineaActual = "";
        }
        else {
            lineaActual += texto[i];
        }
    }

    if (!lineaActual.empty()) {
        lineas.push_back(recortarEspacios(lineaActual));
    }

    return lineas;
}

vector<int> Utilidad::extraerNumerosEnteros(string linea) {
    vector<int> numeros;
    string numeroActual = "";

    for (size_t i = 0; i < linea.size(); i++) {
        char caracter = linea[i];
        if (caracter >= '0' && caracter <= '9') {
            numeroActual += caracter;
        }
        else {
            if (!numeroActual.empty()) {
                numeros.push_back(stoi(numeroActual));
                numeroActual = "";
            }
        }
    }

    if (!numeroActual.empty()) {
        numeros.push_back(stoi(numeroActual));
    }

    return numeros;
}

vector<string> Utilidad::dividirEnPalabras(string linea) {
    vector<string> palabras;
    string palabraActual = "";

    for (size_t i = 0; i < linea.size(); i++) {
        char caracter = linea[i];
        if (caracter == ' ' || caracter == '\t') {
            if (!palabraActual.empty()) {
                palabras.push_back(palabraActual);
                palabraActual = "";
            }
        }
        else {
            palabraActual += caracter;
        }
    }

    if (!palabraActual.empty()) {
        palabras.push_back(palabraActual);
    }

    return palabras;
}
