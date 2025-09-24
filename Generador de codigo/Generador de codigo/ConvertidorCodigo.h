#pragma once
#include <string>
#include <vector>
#include "Utilidad.h"
#include "Mensaje.h"
using namespace std;

enum class Intencion {
    Suma,
    Resta,
    Multiplicacion,
    Division,
    Imprimir,
    CrearLista,
    Si,
    Sino,
    Para,
	Repetir,
    Mientras,
    Asignacion,
    CrearVariable,
    Desconocida
};

struct Variable {
    string tipo;
    string nombre;
    string valor;
};

class ConvertidorCodigo
{
private:
    string textoEntrada;
    string codigoGenerado;
    vector<Mensaje> mensajes;
    vector<Variable> variablesDeclaradas;

    int contadorSuma = 0;
    int contadorResta = 0;
    int contadorMultiplicacion = 0;
    int contadorDivision = 0;

    Intencion detectarIntencion(string lineaNormalizada);

    string emitirSuma(string lineaOriginal);
    string emitirResta(string lineaOriginal);
    string emitirMultiplicacion(string lineaOriginal);
    string emitirDivision(string lineaOriginal);
    string emitirImprimir(string lineaOriginal);
    string emitirCrearLista(string lineaOriginal);
	string emitirRepetir(string lineaOriginal);
	string emitirMientras(string lineaOriginal);
	string emitirCrearVariable(string lineaOriginal);
    string buscarVariable(string nombre);
public:
    ConvertidorCodigo();
    ConvertidorCodigo(string textoEntrada);

    void setTexto(string nuevoTexto);
    string getTexto();

    string getCodigoGenerado();
    vector<Mensaje> getMensajes();

    bool procesar();
    string construirPrograma();
};

