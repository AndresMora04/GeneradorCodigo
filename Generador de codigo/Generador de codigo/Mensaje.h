#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class TipoMensaje {
    Informacion,
    Advertencia,
    Error
};

class Mensaje
{
private:
    TipoMensaje tipo;
    string texto;
public:
    Mensaje(TipoMensaje tipoMensaje, string contenido);

    TipoMensaje getTipo();
    string getTexto();

    string tipoComoTexto();


};

