#include "MainView.h"

MainView::MainView(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("Convertidor Lenguaje Natural → C++");

    connect(ui.btnCargar, &QPushButton::clicked, this, &MainView::onCargar);
    connect(ui.btnConvertir, &QPushButton::clicked, this, &MainView::onConvertir);
    connect(ui.btnGuardar, &QPushButton::clicked, this, &MainView::onGuardar);
}

MainView::~MainView() {}

void MainView::onCargar()
{
  QString ruta = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo",
        QDir::homePath(),
        "Archivos de texto (*.txt)"
    );

  if (ruta.isEmpty()) return;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream in(&archivo);
    QString contenido = in.readAll();
    archivo.close();

    ui.txtCargado->setPlainText(contenido);

    string texto = contenido.toStdString();
    convertidor.setTexto(texto);

    qDebug().noquote() << "Archivo cargado:\n" << contenido;

}

void MainView::onConvertir()
{
    string texto = ui.txtCargado->toPlainText().toStdString();
    convertidor.setTexto(texto);

    if (convertidor.procesar()) {
        string codigo = convertidor.construirPrograma();
        ui.txtConvertido->setPlainText(QString::fromStdString(codigo));
        ui.btnGuardar->setEnabled(true);
        statusBar()->showMessage("Conversión exitosa", 3000);
    }
    else {
        ui.txtConvertido->setPlainText("// No se pudo convertir el texto cargado o escrito.\n");
        ui.btnGuardar->setEnabled(false);
        statusBar()->showMessage("Error en la conversión", 3000);
    }
}

void MainView::onGuardar()
{

    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar C++ generado",
        QDir::homePath() + "/ProgramaGenerado.cpp",
        "Código C++ (*.cpp)"
    );

    if (ruta.isEmpty()) return;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo.");
        return;
    }

    QTextStream out(&archivo);
    out << ui.txtConvertido->toPlainText();
    archivo.close();

    statusBar()->showMessage("Archivo guardado en " + ruta, 3000);
}