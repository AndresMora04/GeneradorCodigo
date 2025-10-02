#include "MainView.h"


MainView::MainView(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    loginWindow = qobject_cast<LogIn*>(parent);

    setWindowTitle("Natural Language to C++ Converter");
    setObjectName("MainView");

    ui.btnLoad->setProperty("kind", "secondary");
    ui.btnConvert->setProperty("kind", "primary");
    ui.btnSave->setProperty("kind", "secondary");

    ui.lbTitle->setProperty("role", "title");

    ui.txtInput->setPlaceholderText("Escribe aquí las instrucciones en lenguaje natural…");
    ui.txtOutput->setPlaceholderText("Aquí verás el código C++ generado…");

    ui.txtOutput->setReadOnly(true);
    ui.txtOutput->setFocusPolicy(Qt::NoFocus);

    if (QFrame* f1 = findChild<QFrame*>("frameInput"))  f1->setProperty("variant", "card");
    if (QFrame* f2 = findChild<QFrame*>("frameOutput")) f2->setProperty("variant", "card");

    auto repolish = [](QWidget* w){
        if (!w) return;
        w->style()->unpolish(w);
        w->style()->polish(w);
        w->update();
    };
    repolish(ui.btnLoad);
    repolish(ui.btnConvert);
    repolish(ui.btnSave);
    repolish(ui.lbTitle);
    repolish(ui.txtOutput);
    repolish(findChild<QFrame*>("frameInput"));
    repolish(findChild<QFrame*>("frameOutput"));

    connect(ui.btnLoad,    &QPushButton::clicked, this, &MainView::onLoadFile);
    connect(ui.btnConvert, &QPushButton::clicked, this, &MainView::onConvert);
    connect(ui.btnSave,    &QPushButton::clicked, this, &MainView::onSaveFile);
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
	QString basePath = "C:/GeneratedProject";
	ensureProjectStructure(basePath);

	QString path = basePath + "/GeneratedProgram.cpp";
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

void MainView::onBackToLogin() {
	if (loginWindow) {
		this->hide();
		loginWindow->show();
	}
	else {
		this->close();
	}
}

void MainView::closeEvent(QCloseEvent* event) {
	if (loginWindow) {
		this->hide();
		loginWindow->show();
		event->ignore();
	}
	else {
		QMainWindow::closeEvent(event);
	}
}

void MainView::ensureProjectStructure(const QString& folderPath) {
	QDir dir(folderPath);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QString slnPath = folderPath + "/GeneratedProject.sln";
	if (!QFile::exists(slnPath)) {
		QFile slnFile(slnPath);
		if (slnFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&slnFile);
			out << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
			out << "# Visual Studio Version 17\n";
			out << "VisualStudioVersion = 17.14.36401.2 d17.14\n";
			out << "MinimumVisualStudioVersion = 10.0.40219.1\n";
			out << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"GeneratedProject\", \"GeneratedProject.vcxproj\", \"{GUID-GENERATED}\"\n";
			out << "EndProject\n";
			out << "Global\n";
			out << "    GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
			out << "        Debug|x64 = Debug|x64\n";
			out << "        Release|x64 = Release|x64\n";
			out << "        Debug|x86 = Debug|x86\n";
			out << "        Release|x86 = Release|x86\n";
			out << "    EndGlobalSection\n";
			out << "    GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
			out << "        {GUID-GENERATED}.Debug|x64.ActiveCfg = Debug|x64\n";
			out << "        {GUID-GENERATED}.Debug|x64.Build.0 = Debug|x64\n";
			out << "        {GUID-GENERATED}.Release|x64.ActiveCfg = Release|x64\n";
			out << "        {GUID-GENERATED}.Release|x64.Build.0 = Release|x64\n";
			out << "        {GUID-GENERATED}.Debug|x86.ActiveCfg = Debug|Win32\n";
			out << "        {GUID-GENERATED}.Debug|x86.Build.0 = Debug|Win32\n";
			out << "        {GUID-GENERATED}.Release|x86.ActiveCfg = Release|Win32\n";
			out << "        {GUID-GENERATED}.Release|x86.Build.0 = Release|Win32\n";
			out << "    EndGlobalSection\n";
			out << "    GlobalSection(SolutionProperties) = preSolution\n";
			out << "        HideSolutionNode = FALSE\n";
			out << "    EndGlobalSection\n";
			out << "    GlobalSection(ExtensibilityGlobals) = postSolution\n";
			out << "        SolutionGuid = {NUEVO-GUID}\n";
			out << "    EndGlobalSection\n";
			out << "EndGlobal\n";
		}
	}

	QString projPath = folderPath + "/GeneratedProject.vcxproj";
	if (!QFile::exists(projPath)) {
		QFile projFile(projPath);
		if (projFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&projFile);
			out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			out << "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";

			out << "  <ItemGroup Label=\"ProjectConfigurations\">\n";
			out << "    <ProjectConfiguration Include=\"Debug|Win32\">\n";
			out << "      <Configuration>Debug</Configuration>\n";
			out << "      <Platform>Win32</Platform>\n";
			out << "    </ProjectConfiguration>\n";
			out << "    <ProjectConfiguration Include=\"Release|Win32\">\n";
			out << "      <Configuration>Release</Configuration>\n";
			out << "      <Platform>Win32</Platform>\n";
			out << "    </ProjectConfiguration>\n";
			out << "    <ProjectConfiguration Include=\"Debug|x64\">\n";
			out << "      <Configuration>Debug</Configuration>\n";
			out << "      <Platform>x64</Platform>\n";
			out << "    </ProjectConfiguration>\n";
			out << "    <ProjectConfiguration Include=\"Release|x64\">\n";
			out << "      <Configuration>Release</Configuration>\n";
			out << "      <Platform>x64</Platform>\n";
			out << "    </ProjectConfiguration>\n";
			out << "  </ItemGroup>\n";

			out << "  <PropertyGroup Label=\"Globals\">\n";
			out << "    <VCProjectVersion>17.0</VCProjectVersion>\n";
			out << "    <Keyword>Win32Proj</Keyword>\n";
			out << "    <ProjectGuid>{GUID-GENERATED}</ProjectGuid>\n";
			out << "    <RootNamespace>GeneratedProject</RootNamespace>\n";
			out << "    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>\n";
			out << "  </PropertyGroup>\n";

			out << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n";

			out << "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">\n";
			out << "    <ConfigurationType>Application</ConfigurationType>\n";
			out << "    <UseDebugLibraries>true</UseDebugLibraries>\n";
			out << "    <PlatformToolset>v143</PlatformToolset>\n";
			out << "    <CharacterSet>Unicode</CharacterSet>\n";
			out << "  </PropertyGroup>\n";

			out << "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">\n";
			out << "    <ConfigurationType>Application</ConfigurationType>\n";
			out << "    <UseDebugLibraries>false</UseDebugLibraries>\n";
			out << "    <PlatformToolset>v143</PlatformToolset>\n";
			out << "    <WholeProgramOptimization>true</WholeProgramOptimization>\n";
			out << "    <CharacterSet>Unicode</CharacterSet>\n";
			out << "  </PropertyGroup>\n";

			out << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n";
			out << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n";

			out << "  <ItemGroup>\n";
			out << "    <ClCompile Include=\"GeneratedProgram.cpp\" />\n";
			out << "  </ItemGroup>\n";

			out << "</Project>\n";
		}
	}

	QString filtersPath = folderPath + "/GeneratedProject.vcxproj.filters";
	if (!QFile::exists(filtersPath)) {
		QFile filtersFile(filtersPath);
		if (filtersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&filtersFile);
			out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			out << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
			out << "  <ItemGroup>\n";
			out << "    <ClCompile Include=\"GeneratedProgram.cpp\">\n";
			out << "      <Filter>Source Files</Filter>\n";
			out << "    </ClCompile>\n";
			out << "  </ItemGroup>\n";
			out << "  <ItemGroup>\n";
			out << "    <Filter Include=\"Source Files\">\n";
			out << "      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>\n";
			out << "      <Extensions>cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx</Extensions>\n";
			out << "    </Filter>\n";
			out << "    <Filter Include=\"Header Files\">\n";
			out << "      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>\n";
			out << "      <Extensions>h;hh;hpp;hxx;hm;inl;inc;ipp;xsd</Extensions>\n";
			out << "    </Filter>\n";
			out << "  </ItemGroup>\n";
			out << "</Project>\n";
			filtersFile.close();
		}
	}

	QString userPath = folderPath + "/GeneratedProject.vcxproj.user";
	if (!QFile::exists(userPath)) {
		QFile userFile(userPath);
		if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&userFile);
			out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			out << "<Project ToolsVersion=\"Current\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
			out << "  <PropertyGroup>\n";
			out << "    <LocalDebuggerWorkingDirectory>$(ProjectDir)</LocalDebuggerWorkingDirectory>\n";
			out << "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n";
			out << "  </PropertyGroup>\n";
			out << "</Project>\n";
			userFile.close();
		}
	}
}