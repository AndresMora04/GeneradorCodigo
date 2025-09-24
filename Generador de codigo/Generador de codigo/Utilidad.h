#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Utilidad
{
	public:
        static string convertirAMinusculas(string texto);

        static string recortarEspacios(string texto);

        static vector<string> dividirEnLineas(string texto);

        static vector<int> extraerNumerosEnteros(string linea);

        static vector<string> dividirEnPalabras(string linea);
};

