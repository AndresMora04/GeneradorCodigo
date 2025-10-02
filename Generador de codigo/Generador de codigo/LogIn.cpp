#include "LogIn.h"

LogIn::LogIn(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setObjectName("LogInView");
    setWindowTitle("Convertidor Lenguaje Natural a C++");

    ui.btnLogin->setProperty("kind", "primary"); 
    if (auto title = findChild<QLabel*>("lbTitle"))
        title->setProperty("role", "title");
    if (auto card = findChild<QFrame*>("frameCard"))
        card->setProperty("variant", "card");

    ui.txtInstructions->setReadOnly(true);
    ui.txtInstructions->setFont(QFont("Consolas", 10));
    ui.txtInstructions->setText(
        "Guia rapida (ejemplos)\n"
        "\n"
        "Programa\n"
        "- \"Comenzar programa\"\n"
        "- \"Terminar programa\"\n"
        "\n"
        "Variables y listas\n"
        "- \"crear variable entero numero con valor inicial 10\"\n"
        "- \"ingresar valor numero\"\n"
        "- \"crear lista de enteros con 5 elementos llamada numeros\"\n"
        "- \"ingresar los valores de la lista numeros\"\n"
        "- \"asignar valor 10 al primer elemento de la lista numeros\"\n"
        "\n"
        "Operaciones\n"
        "- \"x sumar y\" -> x += y;\n"
        "- \"x restar 1\" -> x -= 1;\n"
        "- \"x multiplicar por y\" -> x *= y;\n"
        "- \"x dividir por 2\" -> x /= 2;\n"
        "- \"calcular sumar 10 y 20 y asignar a total\" -> total = 10 + 20;\n"
        "\n"
        "Mostrar / Imprimir\n"
        "- \"mostrar \\\"El valor es: \\\" y numero\"\n"
        "- \"imprimir numero\"\n"
        "\n"
        "Condicionales\n"
        "- \"si numero mayor que 50 mostrar \\\"Mayor que 50\\\"\"\n"
        "- Comparadores: mayor que, menor que, mayor o igual, menor o igual,\n"
        "  igual a/igual que, distinto de/diferente de/no igual\n"
        "- \"sino\" (para else)\n"
        "\n"
        "Bucles\n"
        "- \"repetir 3 veces mostrar \\\"Hola\\\"\"\n"
        "- \"mientras n mayor que 1\" (el cuerpo va en las siguientes lineas)\n"
        "\n"
        "Funciones\n"
        "- \"definir funcion numero entero factorial con parametro numero entero n\"\n"
        "  (cuerpo con variables/while/retornar)\n"
        "- \"retornar resultado\"\n"
        "- \"asignar valor a r con llamar funcion factorial(x)\"\n"
        "\n"
        "Estructuras\n"
        "- \"crear estructura Estudiante con nombre (cadena de texto), edad (entero) y nota (decimal)\"\n"
        "- \"crear lista de Estudiante con 3 elementos llamada alumnos\"\n"
        "- \"ingresar los datos de cada estudiante\" (usa getline para nombres con espacios)\n"
        "- \"recorrer la lista y mostrar nombre y nota\"\n"
        "\n"
        "Consejos\n"
        "- Usa comillas \"\\\"...\\\"\" para texto.\n"
        "- Para concatenar en mostrar: usa \"y\" (ej: \"mostrar \\\"Hola\\\" y nombre\").\n"
        "- Las listas permiten: primer, segundo, tercero... hasta decimo; despues usa indices numericos.\n"
    );

    auto repolish = [](QWidget* w) {
        if (!w) return;
        w->style()->unpolish(w);
        w->style()->polish(w);
        w->update();
        };
    repolish(this);
    repolish(ui.btnLogin);
    repolish(findChild<QLabel*>("lbTitle"));
    repolish(findChild<QFrame*>("frameCard"));
    repolish(ui.txtInstructions);

    connect(ui.btnLogin, &QPushButton::clicked, this, &LogIn::onLogin);
}
LogIn::~LogIn() {}

void LogIn::onLogin()
{
	if (!mainView) {
		mainView = new MainView(this);
	}
	this->hide();
	mainView->show();
}