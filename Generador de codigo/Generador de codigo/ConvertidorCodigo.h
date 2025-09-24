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
    Mientras,
    Asignacion,
    CrearVariable,
    Desconocida
};

class ConvertidorCodigo
{
private:
    string textoEntrada;
    string codigoGenerado;
    vector<Mensaje> mensajes;

    Intencion detectarIntencion(string lineaNormalizada);

    string emitirSuma(string lineaOriginal);
    string emitirResta(string lineaOriginal);
    string emitirMultiplicacion(string lineaOriginal);
    string emitirDivision(string lineaOriginal);
    string emitirImprimir(string lineaOriginal);
    string emitirCrearLista(string lineaOriginal);
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

