#include "LogIn.h"

LogIn::LogIn(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.txtInstrucciones->setReadOnly(true);
    ui.txtInstrucciones->setFont(QFont("Consolas", 10));
    ui.txtInstrucciones->setText(
        "?? Instrucciones básicas:\n"
        "- Crear lista: \"crear lista de enteros con 5 elementos llamada numeros\"\n"
        "- Asignar valor: \"asignar valor 10 al primer elemento de la lista numeros\"\n"
        "- Crear variable: \"crear variable entero numero con valor inicial 10\"\n"
        "- Sumar: \"sumar los números 10, 20 y 30 y mostrar el resultado\"\n"
        "- Condición: \"si numero mayor que 50 mostrar 'Mayor que 50'\"\n"
        "- Ciclo repetir: \"repetir 3 veces imprimir 'Hola'\"\n"
        "- Ciclo mientras: \"mientras numero < 100 incrementar numero en 10\""
    );

    connect(ui.btnIngresar, &QPushButton::clicked, this, &LogIn::onIngresar);
}

LogIn::~LogIn()
{
}

void LogIn::onIngresar()
{
    MainView* mainView = new MainView();
    mainView->show();

    this->close();
}