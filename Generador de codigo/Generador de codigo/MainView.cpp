#include "MainView.h"

MainView::MainView(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle("Natural Language to C++ Converter");

	connect(ui.btnLoad, &QPushButton::clicked, this, &MainView::onLoadFile);
	connect(ui.btnConvert, &QPushButton::clicked, this, &MainView::onConvert);
	connect(ui.btnSave, &QPushButton::clicked, this, &MainView::onSaveFile);
}

MainView::~MainView() {}

void MainView::onLoadFile()
{
	QString path = QFileDialog::getOpenFileName(
		this,
		"Select File",
		QDir::homePath(),
		"Text Files (*.txt)"
	);

	if (path.isEmpty()) return;

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Error", "Could not open the file.");
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	ui.txtInput->setPlainText(content);

	string text = content.toStdString();
	converter.setInputText(text);

	qDebug().noquote() << "File loaded:\n" << content;
}

void MainView::onConvert()
{
	string text = ui.txtInput->toPlainText().toStdString();
	converter.setInputText(text);

	ui.txtOutput->clear();

	if (converter.process()) {
		string code = converter.buildProgram();
		ui.txtOutput->setPlainText(QString::fromStdString(code));
		ui.btnSave->setEnabled(true);
		statusBar()->showMessage("Conversion successful", 3000);
	}
	else {
		ui.txtOutput->setPlainText("// Could not convert the provided text.\n");
		ui.btnSave->setEnabled(false);
		statusBar()->showMessage("Conversion error", 3000);
	}

	vector<Message> msgs = converter.getMessages();
	QString allMsgs;
	for (auto& m : msgs) {
		QString prefix;
		if (m.getType() == MessageType::Error) prefix = "[Error] ";
		else if (m.getType() == MessageType::Warning) prefix = "[Warning] ";
		else if (m.getType() == MessageType::Information) prefix = "[Info] ";

		allMsgs += "// " + prefix + QString::fromStdString(m.getText()) + "\n";
	}

	if (!allMsgs.isEmpty()) {
		ui.txtOutput->append("\n// Messages:\n" + allMsgs);
	}
}

void MainView::onSaveFile()
{
	QString path = QFileDialog::getSaveFileName(
		this,
		"Save Generated C++ File",
		QDir::homePath() + "/GeneratedProgram.cpp",
		"C++ Code (*.cpp)"
	);

	if (path.isEmpty()) return;

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Error", "Could not save the file.");
		return;
	}

	QTextStream out(&file);
	out << ui.txtOutput->toPlainText();
	file.close();

	statusBar()->showMessage("File saved at " + path, 3000);
}