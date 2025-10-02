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
		"- \"crear lista enteros con 5 elementos llamada numeros\"\n"
		"- \"ingresar los valores de la lista numeros\"\n"
		"- \"asignar valor 10 al primer elemento de la lista numeros\"\n"
		"- \"recorrer la lista numeros\"\n"
		"- \"crear lista de Estudiante con 3 elementos llamada estudiantes\"\n"
		"- \"ingresar los datos de cada estudiante\"\n"
		"- \"recorrer la lista estudiantes y mostrar nombre y nota\" (campos de la estructura Estudiante)\n"
		"\n"
		"Concatenacion\n"
		"- \"asignar a resultado;\n"
		"\n"
		"Operaciones\n"
		"- \"x sumar y\" -> x += y;\n"
		"- \"x restar 1\" -> x -= 1;\n"
		"- \"x multiplicar por y\" -> x *= y;\n"
		"- \"x dividir por 2\" -> x /= 2;\n"
		"- \"sumar 10 y 20 y asignar a total\" -> total = 10 + 20;\n"
		"\n"
		"Mostrar / Imprimir\n"
		"- \"mostrar \\\"El valor es: \\\"\n"
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
		"- \"ingresar los datos de cada estudiante\" (podras usar nombres con espacios)\n"
		"- \"recorrer la lista y mostrar nombre y nota\"\n"
		"\n"
		"Consejos\n"
		"- Escribe sin acentos en palabras clave: numero, funcion, mientras, sino, mostrar, imprimir.\n"
		"- Usa comillas \"\\\"...\\\"\" para texto literal.\n"
		"- Para concatenar en Mostrar/Imprimir usa \"y\" (ej.: \"mostrar \\\"Hola\\\" y nombre\").\n"
		"- Evita duplicar salidas: si ya concatenas con \"y\", no repitas la misma variable en otra parte.\n"
		"\n"
		"- Muestra un mensaje ANTES de leer (guia al usuario):\n"
		"  Variable:\n"
		"    mostrar \\\"Ingrese un valor para 'edad':\\\"\\n"
		"    ingresar valor edad\n"
		"  Lista:\n"
		"    mostrar \\\"Ingrese 3 valores para 'numeros':\\\"\\n"
		"    ingresar los valores de la lista numeros\n"
		"  Lista de estructuras:\n"
		"    mostrar \\\"Ingrese 2 estudiantes (nombre y edad). Para cada estudiante:\\\"\\n"
		"    mostrar \\\" - Escriba el nombre y presione Enter\\\"\\n"
		"    mostrar \\\" - Luego ingrese la edad y presione Enter\\\"\\n"
		"    ingresar los datos de cada Estudiante\n"
		"\n"
		"- Declara SIEMPRE las variables antes de usarlas.\n"
		"- Las listas aceptan ordinales: primer, segundo, tercero... hasta decimo; luego usa indices numericos.\n"
		"- Manten coherencia de tamanos (si dices 3 elementos, muestra \\\"Ingrese 3 valores...\\\").\n"
		"- Para textos con espacios (nombres): el ingreso ya usa getline(cin >> ws, ...); escribe normal.\n"
		"- Condicional en una linea funciona si termina en Mostrar/Imprimir:\n"
		"  ej.: si x mayor que 10 mostrar \\\"Mayor\\\"\n"
		"\n"
		"- Evita usar como NOMBRE DE VARIABLE palabras reservadas:\n"
		"  si, sino, mientras, repetir, crear, variable, estructura, lista, funcion,\n"
		"  retornar, imprimir, mostrar, asignar, sumar, restar, multiplicar, dividir,\n"
		"  por, a, parametro, numero, entero, decimal, texto, string, int, float, bool,\n"
		"  igual, mayor, menor, verdadero, falso.\n"
		"- Evita tambien nombres que el parser usa como tokens:\n"
		"  y (conjuncion), i (indice), primer/segundo/.../decimo (ordinales),\n"
		"  elementos, valores, lista, llamada.\n"
		"- Prefiere nombres descriptivos: edad, total, promedio, buscado, encontrado,\n"
		"  contador, acumulador, notas, estudiantes, resultado, cantidad, limite, valorEntrada.\n"
		"\n"
		"- Tip de seguridad:\n"
		"  Para factorial con int, usa valores pequenos (<= 12) para evitar overflow.\n"
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