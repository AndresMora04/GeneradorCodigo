#include "Mensaje.h"

Mensaje::Mensaje(TipoMensaje tipoMensaje, string contenido)
    : tipo(tipoMensaje), texto(contenido) {
}

TipoMensaje Mensaje::getTipo() {
    return tipo;
}

string Mensaje::getTexto() {
    return texto;
}

string Mensaje::tipoComoTexto() {
    if (tipo == TipoMensaje::Informacion) {
        return "Informacion";
    }
    else if (tipo == TipoMensaje::Advertencia) {
        return "Advertencia";
    }
    else if (tipo == TipoMensaje::Error) {
        return "Error";
    }
    else {
        return "Desconocido";
    }
}