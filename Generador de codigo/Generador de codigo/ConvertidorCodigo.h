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
	AsignarElementoLista,
	IngresarLista,
	LlenarLista,
	MostrarLista,
	RecorrerLista,
	Si,
	Sino,
	Para,
	Repetir,
	Mientras,
	Asignacion,
	CrearVariable,
	IngresarValor,
	Comenzar,
	Terminar,
	Desconocida
};

struct Variable {
	string tipo;
	string nombre;
	string valor;
	int tamano = 0;
	bool esLista = false;
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
	int contadorLista = 0;

	Intencion detectarIntencion(string lineaNormalizada);

	string emitirComenzar(string lineaOriginal);
	string emitirTerminar(string lineaOriginal);
	string emitirSuma(string lineaOriginal);
	string emitirResta(string lineaOriginal);
	string emitirMultiplicacion(string lineaOriginal);
	string emitirDivision(string lineaOriginal);
	string emitirImprimir(string lineaOriginal);
	string emitirCrearLista(string lineaOriginal);
	string emitirIngresarLista(string lineaOriginal);
	string emitirAsignarElementoLista(string lineaOriginal);
	string emitirRecorrerLista(string lineaOriginal);
	string emitirRepetir(string lineaOriginal);
	string emitirMientras(string lineaOriginal);
	string emitirSi(string lineaOriginal);
	string emitirSino(string lineaOriginal);
	string emitirCrearVariable(string lineaOriginal);
	string emitirIngresarValor(string lineaOriginal);
	Variable* buscarVariable(string nombre);
	int  obtenerIndiceDesdePalabraONumero(string lineaOriginal, vector<string> palabras);
	string normalizarPalabra(const string& palabra);
	bool dentroDeBloque = false;
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

