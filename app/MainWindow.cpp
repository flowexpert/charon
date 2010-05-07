#include "MainWindow.h"

#include <QFile>
#include <QtGui>
#include <MyTabWidget.h>
#include <QComboBox>

MainWindow::MainWindow(QWidget* p)
		: QMainWindow(p)
{
	QWidget* myCentralWidget = new QWidget;
	setCentralWidget(myCentralWidget);
	// restore window geometry
	_readSettings();

	QGridLayout* layoutmain = new QGridLayout;
	myCentralWidget->setLayout(layoutmain);
	MyTabWidget *tabWidget = new MyTabWidget;
	layoutmain->addWidget(tabWidget,1,1);

	QWidget* page0 = new QWidget;
	tabWidget->addTab(page0, tr("Load"));
	QWidget* page1 = new QWidget;
	tabWidget->addTab(page1, tr("General"));
	QWidget* page2 = new QWidget;
	tabWidget->addTab(page2, tr("Slots"));
	QWidget* page3 = new QWidget;
	tabWidget->addTab(page3, tr("Parameters"));

	// page 0
	QVBoxLayout* background0 = new QVBoxLayout;
	QWidget* page0hi = new QWidget;
	QWidget* page0lo = new QWidget;
	background0->addWidget(page0hi,6);
	background0->addWidget(page0lo,1);
	QGridLayout* layout0 = new QGridLayout;
	page0->setLayout(background0);
	page0hi->setLayout(layout0);
	QHBoxLayout* buttonlayout0 = new QHBoxLayout;
	page0lo->setLayout(buttonlayout0);
	QPushButton* nextButton0 = new QPushButton (
			tr("&Create new plugin from scratch >"));
	QLabel* welcome = new QLabel(
			tr(
				"<h2 align=center>Welcome to the plugin template generator!"
				"</h2><br><br><br>"
				"This setup wizzard belongs to charon and helps to create "
				"simple plugins with common code parts.<br><br><br>"
				"If you want to load an existing plugin "
				"you can type in its path or use the browse button.<br>"
				"Otherwise continue without loading."));
	QLabel* pluginLoadText = new QLabel(tr("Path of plugin to load:"));
	QPushButton* load = new QPushButton(tr("&Load existing plugin >"));
	QPushButton* browse2 = new QPushButton(tr("&Browse"));
	//_inputFile = new QLineEdit();

	_inputFile1 = new QComboBox;
	_inputFile1->setEditable(true);
	_inputFile1->setInsertPolicy(QComboBox::NoInsert);


	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	_inputFile1 -> addItem(settings.value("recentInputDir1", QDir::homePath())
		.toString());
	_inputFile1 -> addItem(settings.value("recentInputDir2")
		.toString());
	_inputFile1 -> addItem(settings.value("recentInputDir3")
		.toString());
	_inputFile1 -> addItem(settings.value("recentInputDir4")
		.toString());
	_inputFile1 -> addItem(settings.value("recentInputDir5")
		.toString());

	layout0->setRowStretch(1,1);
	layout0->addWidget(welcome,2,1,1,3);
	layout0->setRowStretch(3,2);
	layout0->addWidget(pluginLoadText,4,1);
	//layout0->addWidget(_inputFile,5,1);
	layout0->addWidget(_inputFile1,5,1);
	layout0->addWidget(browse2,5,2);
	layout0->addWidget(load,5,3);
	layout0->setRowStretch(6,2);
	layout0->setColumnStretch(1,1);
	buttonlayout0->addStretch();
	buttonlayout0->addWidget(nextButton0);

	connect(nextButton0,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(load,SIGNAL(clicked()),tabWidget, SLOT(nextPage()));
	connect(load,SIGNAL(clicked()),this, SLOT(_load()));
	connect(browse2,SIGNAL(clicked()),this,SLOT(_selectInputFile()));

	// page 1
	QVBoxLayout* background1 = new QVBoxLayout;
	QWidget* page1hi = new QWidget;
	QWidget* page1lo = new QWidget;
	background1->addWidget(page1hi,6);
	background1->addWidget(page1lo,1);
	QGridLayout* layout1 = new QGridLayout;
	page1->setLayout(background1);
	page1hi->setLayout(layout1);
	QHBoxLayout* buttonlayout1 = new QHBoxLayout;
	page1lo->setLayout(buttonlayout1);

	QLabel* label = new QLabel;

	QLabel* author = new QLabel (tr("Author:"));
	_inputAuthorName = new QLineEdit(
			settings.value("recentAuthor", tr("Unknown")).toString());

	QLabel* name = new QLabel (tr("Name of Plugin:"));
	_inputName = new QLineEdit(tr("plugin1"));

	QLabel* save = new QLabel (tr("Saved in:"));
	_inputDir = new QLineEdit(
			settings.value("recentOutputDir", QDir::homePath()).toString());
	// add directory completer for output directory
	QCompleter* completer = new QCompleter(this);
	completer->setModel(new QDirModel(QStringList(), QDir::AllDirs,
			QDir::Name, completer));
	_inputDir->setCompleter(completer);
	QPushButton* browse = new QPushButton (tr("Browse"));

	_templated = new QComboBox;
	_templated->addItem(tr("Templated"));
	_templated->addItem(tr("Non-Templated"));

	QLabel* pluginDocLabel = new QLabel(tr("Plugin Description:"));
	_pluginDoc = new QTextEdit;

	label->setText(tr("Please choose a name and directory to save."));
	layout1->addWidget(label,1,1,1,3,Qt::AlignCenter);
	layout1->setRowStretch(2,1);
	layout1->addWidget(author,3,1);
	layout1->addWidget(_inputAuthorName,3,2);
	layout1->addWidget(name,4,1);
	layout1->addWidget(_inputName,4,2);
	layout1->addWidget(save,5,1);
	layout1->addWidget(_inputDir,5,2);
	layout1->addWidget(browse,5,3);
	layout1->setRowStretch(6,1);
	layout1->addWidget(_templated,7,2);
	layout1->addWidget(pluginDocLabel,8,1);
	layout1->addWidget(_pluginDoc,8,2);
	layout1->setRowStretch(9,1);
	layout1->setColumnStretch(2,1);

	QPushButton* previousButton = new QPushButton(tr("< &Back"));
	QPushButton* nextButton1 = new QPushButton (tr("&Continue >"));
	buttonlayout1->addStretch();
	buttonlayout1->addWidget(previousButton);
	buttonlayout1->addWidget(nextButton1);

	connect(previousButton, SIGNAL(clicked()), tabWidget, SLOT(previousPage()));
	connect(nextButton1, SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(browse, SIGNAL(clicked()), this, SLOT(_selectOutputDir()));

	// page 2
	QVBoxLayout* background2 = new QVBoxLayout;
	QWidget* page2hi = new QWidget;
	QWidget* page2lo = new QWidget;
	background2->addWidget(page2hi,6);
	background2->addWidget(page2lo,1);
	QGridLayout* layout2 = new QGridLayout;
	page2->setLayout(background2);
	page2hi->setLayout(layout2);
	QHBoxLayout* buttonlayout2 = new QHBoxLayout;
	page2lo->setLayout(buttonlayout2);
	QPushButton* nextButton2 = new QPushButton (tr("&Continue >"));
	QPushButton* add = new QPushButton (tr("&Add"));
	QPushButton* remove = new QPushButton (tr("&Remove"));
	QPushButton* add2 = new QPushButton (tr("&Add"));
	QPushButton* remove2 = new QPushButton (tr("&Remove"));
	QPushButton* previousButton2 = new QPushButton(tr("< &Back"));
	QLabel* inputLabel = new QLabel (tr("Input Slots"));
	QLabel* outputLabel = new QLabel (tr("Output Slots"));
	_optionalSlot = new QCheckBox("optional slots");
	_multiSlot = new QCheckBox("multi slots");
	_table1 = new QTableWidget(0, 3);
	QStringList longerList = (QStringList() << "Name"  << "Documentation" << "Typ");
	_table1->setHorizontalHeaderLabels(longerList);
	_table1->verticalHeader()->hide();
	_table1->horizontalHeader()->setStretchLastSection(true);
	_table1->resizeColumnsToContents();
	_table1->setColumnWidth(0, 200);
	_table1->setColumnWidth(2, 300);

	_editRowCount(0);
	_editRowCount(0);

	_table2 = new QTableWidget(0, 3);
	_table2->setHorizontalHeaderLabels(longerList);
	_table2->verticalHeader()->hide();
	_table2->horizontalHeader()->setStretchLastSection(true);
	_table2->resizeColumnsToContents();
	_table2->setColumnWidth(0, 200);
	_table2->setColumnWidth(2, 300);

	_editRowCount(1);
	_editRowCount(1);

	layout2->addWidget(inputLabel,1,1,1,3);
	layout2->addWidget(_optionalSlot,2,1);
	layout2->addWidget(_multiSlot,3,1);
	layout2->addWidget(_table1,4,1,1,3);
	layout2->addWidget(add,5,1);
	layout2->addWidget(remove,5,2);
	layout2->setColumnStretch(3,1);

	layout2->addWidget(outputLabel,7,1,1,3);
	layout2->addWidget(_table2,8,1,1,3);
	layout2->addWidget(add2,9,1);
	layout2->addWidget(remove2,9,2);
	layout2->setRowMinimumHeight(6,10);

	buttonlayout2->addStretch();
	buttonlayout2->addWidget(previousButton2);
	buttonlayout2->addWidget(nextButton2);

	signalMapper = new QSignalMapper(this);
	connect(add, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add, 0);
	connect(signalMapper, SIGNAL(mapped(int)),
		this, SIGNAL(clicked(int)));
	connect(add2, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add2, 1);
	connect(remove, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove, 4);
	connect(remove2, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove2, 5);

	connect(previousButton2, SIGNAL(clicked()), tabWidget, SLOT(previousPage()));
	connect(nextButton2,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(this,SIGNAL(clicked(int)), this, SLOT(_editRowCount(int)));

	// page 3
	QVBoxLayout* background3 = new QVBoxLayout;
	QWidget* page3hi = new QWidget;
	QWidget* page3lo = new QWidget;
	background3->addWidget(page3hi,6);
	background3->addWidget(page3lo,1);
	QGridLayout* layout3 = new QGridLayout;
	page3->setLayout(background3);
	page3hi->setLayout(layout3);
	QHBoxLayout* buttonlayout3 = new QHBoxLayout;
	page3lo->setLayout(buttonlayout3);
	QPushButton* createButton = new QPushButton(tr("&Create"));
	QPushButton* add3 = new QPushButton (tr("&Add"));
	QPushButton* remove3 = new QPushButton (tr("&Remove"));
	QPushButton* previousButton3 = new QPushButton(tr("< &Back"));
	QLabel* parameterLabel = new QLabel (tr("In this table you can define parameter. \n\n"
						"Warning: String values have to be encapsulated into \"\"\n"));
	_table3 = new QTableWidget(0, 4, page3);
	QStringList paramlist = (
			QStringList() << "Name" << "Documentation"
			<< "Typ" << "Default" );
	_table3->setHorizontalHeaderLabels(paramlist);
	layout3->addWidget(parameterLabel,1,1,1,3);
	layout3 -> addWidget(_table3,2,1,1,3);
	layout3->addWidget(add3,3,1);
	layout3->addWidget(remove3,3,2);
	layout3->setColumnStretch(3,1);

	buttonlayout3->addStretch();
	buttonlayout3->addWidget(previousButton3);
	buttonlayout3->addWidget(createButton);

	_editRowCount(2);
	_editRowCount(2);


	QLabel* parameterListLabel = new QLabel (tr("ParameterList "));
	QPushButton* add4 = new QPushButton (tr("&Add"));
	QPushButton* remove4 = new QPushButton (tr("&Remove"));
	_table4 = new QTableWidget(0, 4, page3);
	QStringList paramlist2 = (
			QStringList() << "Name" << "Documentation" << "Typ" << "Default" );
	_table4->setHorizontalHeaderLabels(paramlist2);
	layout3->addWidget(parameterListLabel,4,1,1,3);
	layout3 -> addWidget(_table4,5,1,1,3);
	layout3->addWidget(add4,6,1);
	layout3->addWidget(remove4,6,2);


	_editRowCount(3);
	_editRowCount(3);


	connect(add3, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add3, 2);
	connect(remove3, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove3, 6);
	connect(add4, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add4, 3);
	connect(remove4, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove4, 7);

	_table3->horizontalHeader()->setStretchLastSection(true);
	_table3->verticalHeader()->hide();
	_table4->horizontalHeader()->setStretchLastSection(true);
	_table4->verticalHeader()->hide();


	connect(previousButton3, SIGNAL(clicked()), tabWidget, SLOT(previousPage()));
	connect(createButton, SIGNAL(clicked()), this, SLOT(_save()));
}

MainWindow::~MainWindow() {
}

void MainWindow::_changeExisting() {
	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}

	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());

	QString pluginFile;
	if (_inputFile->text().trimmed().section(".",-1,-1) == "h"
		||_inputFile->text().trimmed().section(".",-1,-1) == "hxx"
		||_inputFile->text().trimmed().section(".",-1,-1) == "cpp")
		pluginFile = _inputFile->text().trimmed().section(".",0,-2);
	else
		pluginFile = _inputFile->text().trimmed();

	QFile loadHeaderFile(QString("%1.h").arg(pluginFile));
		if (!loadHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

	QTextStream loadH(&loadHeaderFile);

	QFile loadCppFile(QString("%1.cpp").arg(pluginFile));
	if (!loadCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream loadCodeCpp(&loadCppFile);

	QFile loadHxxFile(QString("%1.hxx").arg(pluginFile));
	if (!loadHxxFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//return;
	}

	QTextStream loadCodeHxx(&loadHxxFile);

	QStringList filesToChange;
	filesToChange.append(loadH.readAll());
	filesToChange.append(loadCodeCpp.readAll());
	filesToChange.append(loadCodeHxx.readAll());

	QString codeString = filesToChange.join("@@@@@@@@@@@");

	codeString.replace("public:","public: \n\n\t@In/Out@");
	codeString.replace("public:","public: \n\n\t@Parameter@");
	codeString.replace("public:","public: \n\n\t@ParameterList@");

	// Zeilenweises Einlesen des Plugins um die alten Parameter und Slots zu entfernen
	// und den richtigen Ort zum Einfuegen im .hxx File zu finden

	QTextStream Code(&codeString);
	QString changedCode;

	while(!Code.atEnd()){

		QString tempString;
		tempString = Code.readLine();
		QString tempString2 = tempString;
		tempString2.replace(" ","");

		QString nameOfPlugin = _inputFile->text().section("/",-1,-1).section(".",0,-2);
		QString constructor = nameOfPlugin.prepend("::");
		tempString.append("\n");


		if(tempString2.contains(constructor)){

			for(int count = 0; !tempString.contains("{") && count < 7; count++){

				tempString.append("\n");
				tempString.append(Code.readLine());
			}

			tempString.replace("{","\t@ParameterListDefault@\n\t@multi/optional@\n{"
						"\n\n\t@addParameter@\n\n\t@addParameterList@\n\n\t@add-In/Out@");
		}

		if(tempString2.contains("Parameter<") || tempString2.contains("_addParameter(") ||
		   tempString2.contains("ParameterList<") ||
		   tempString2.contains("InputSlot<") || tempString2.contains("OutputSlot<") ||
		tempString2.contains("_addInputSlot") || tempString2.contains("_addOutputSlot")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
			}

			tempString.clear();
		}
		changedCode.append(tempString);
	}


	for(int i = 0; i < _table1->rowCount(); i++) {

		QTableWidgetItem* IName = _table1->item(i,0);
		QTableWidgetItem* IDoc = _table1->item(i,1);
		QTableWidgetItem* ITyp = _table1->item(i,2);

		IName->text();

		if(!QString(IName->text()).isEmpty()){


			changedCode.replace("@In/Out@",QString("/// @Documentation@ \n\tInputSlot < @Typ@ > @I/O-Name@;"
				"\n\t@In/Out@"));
			changedCode.replace("@multi/optional@",QString(",@I/O-Name@(@optional@, @multi@) "
				"\n\t\t\t@multi/optional@"));


			changedCode.replace(QString("@Documentation@"),IDoc->text().replace("\n","\n\t/// ").trimmed());
			changedCode.replace(QString("@Typ@"),ITyp->text().trimmed());
			changedCode.replace("@I/O-Name@",IName->text().trimmed());

			if(_multiSlot->isChecked())
				changedCode.replace("@multi@","true");
			else
				changedCode.replace("@multi@","false");

			if(_optionalSlot->isChecked())
				changedCode.replace("@optional@","true");
			else
				changedCode.replace("@optional@","false");


			changedCode.replace("@add-In/Out@",QString("ParameteredObject::_addInputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

			changedCode.replace(QString("@Documentation@"),IDoc->text().replace("\n"," <br> ").trimmed());
			changedCode.replace(QString("@Typ@"),ITyp->text().trimmed());
			changedCode.replace(QString("@IOName@"),IName->text().trimmed());
		}

	}
	for(int k = 0; k < _table2->rowCount(); k++) {



		QTableWidgetItem* OName = _table2->item(k,0);
		QTableWidgetItem* ODoc = _table2->item(k,1);
		QTableWidgetItem* OTyp = _table2->item(k,2);

		if(!QString(OName->text()).isEmpty()){

			changedCode.replace("@In/Out@",
						QString("/// @Documentation@ \n\tOutputSlot < @Typ@ > @I/O-Name@;"
						"\n\t@In/Out@"));

			changedCode.replace(QString("@Documentation@"),ODoc->text().replace("\n","\n\t/// ").trimmed());
			changedCode.replace(QString("@Typ@"),OTyp->text().trimmed());
			changedCode.replace("@I/O-Name@",OName->text().trimmed());

			changedCode.replace("@add-In/Out@",QString("ParameteredObject::_addOutputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

			changedCode.replace(QString("@Documentation@"),ODoc->text().replace("\n"," <br> ").trimmed());
			changedCode.replace(QString("@Typ@"),OTyp->text().trimmed());
			changedCode.replace(QString("@IOName@"),OName->text().trimmed());
		}

	}
	changedCode.replace("@In/Out@","");
	changedCode.replace("@add-In/Out@","");
	changedCode.replace("@multi/optional@","");




	for (int j = 0; j < _table3->rowCount(); j++ ) {

		QTableWidgetItem* paraName = _table3->item(j,0);
		QTableWidgetItem* paraDoc = _table3->item(j,1);
		QTableWidgetItem* paraTyp = _table3->item(j,2);
		QTableWidgetItem* paraDefault = _table3->item(j,3);

		if(!QString(paraName->text()).isEmpty()){
			changedCode.replace("@Parameter@",QString("/// @Documentation@ \n\tParameter < @Typ@ >"
				" @ParameterName@;\n\t@Parameter@"));
			changedCode.replace("@ParameterName@",paraName->text().trimmed());
			changedCode.replace("@Typ@",paraTyp->text().trimmed());
			changedCode.replace("@Documentation@",paraDoc->text()
					.replace("\n","\n\t/// ").trimmed());
			}




		if(!QString(paraName->text()).isEmpty()){

			changedCode.replace("@addParameter@",QString("ParameteredObject::_addParameter "
				"(@ParaName@, \"@ParaName@\", \"@Documentation@\", "
				"@Default@);\n\t@addParameter@"));
			changedCode.replace("@ParaName@",paraName->text().trimmed());
			changedCode.replace("@Documentation@",paraDoc->text()
					.replace("\n"," <br> ").trimmed());
			changedCode.replace("@Default@",paraDefault->text().trimmed());
		}

	}
	changedCode.replace("@Parameter@","");
	changedCode.replace("@addParameter@","");

	for (int l = 0; l < _table4->rowCount(); l++ ) {

		QTableWidgetItem* paraListName = _table4->item(l,0);
		QTableWidgetItem* paraListDoc = _table4->item(l,1);
		QTableWidgetItem* paraListTyp = _table4->item(l,2);
		QTableWidgetItem* paraListDefault = _table4->item(l,3);

		if(!QString(paraListName->text()).isEmpty()){
			changedCode.replace("@ParameterList@",QString("/// @Documentation@ \n\tParameterList < @Typ@ >"
				" @ParameterName@;\n\t@ParameterList@"));
			changedCode.replace("@ParameterListDefault@",QString(" ,@ParameterName@(@Default@)"
				"\n\t@ParameterListDefault@"));
			changedCode.replace("@ParameterName@",paraListName->text().trimmed());
			changedCode.replace("@Typ@",paraListTyp->text().trimmed());
			changedCode.replace("@Documentation@",paraListDoc->text()
					.replace("\n","\n\t/// ").trimmed());
			changedCode.replace("@Default@",paraListDefault->text().trimmed());
			}




		if(!QString(paraListName->text()).isEmpty()){

			changedCode.replace("@addParameterList@",QString("ParameteredObject::_addParameter "
				"(@ParaListName@, \"@ParaListName@\", \"@Documentation@\" "
				");\n\t@addParameterList@"));
			changedCode.replace("@ParaListName@",paraListName->text().trimmed());
			changedCode.replace("@Documentation@",paraListDoc->text()
					.replace("\n"," <br> ").trimmed());
		}

	}
	changedCode.replace("@ParameterList@","");
	changedCode.replace("@addParameterList@","");
	changedCode.replace("@ParameterListDefault@","");



	QStringList editedNewFiles;
	editedNewFiles = changedCode.split("@@@@@@@@@@@");


	if (loadHxxFile.exists()) {
		QFile templatedHeaderFile(QString("%1.h").arg(pluginFile));
		if (!templatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;



		QTextStream out(&templatedHeaderFile);
		out  << editedNewFiles.at(0) << "\n\n\n" ;


		QFile templatedCppFile(QString("%1.cpp").arg(pluginFile));

		if (!templatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out2(&templatedCppFile);
		out2 << editedNewFiles.at(1) << "\n\n\n";


		QFile templatedHxxFile(QString("%1.hxx").arg(pluginFile));
		if (!templatedHxxFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out3(&templatedHxxFile);
		out3  << editedNewFiles.at(2) << "\n\n\n";

		}

	else {
		QFile nontemplatedHeaderFile(QString("%1.h").arg(pluginFile));

		if (!nontemplatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

		QTextStream out4(&nontemplatedHeaderFile);
		out4 << editedNewFiles.at(0) << "\n\n\n";


		QFile nontemplatedCppFile(QString("%1.cpp").arg(pluginFile));
		if (!nontemplatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out5(&nontemplatedCppFile);
		out5 << editedNewFiles.at(1) << "\n\n\n";

		}

	QMessageBox::information(this, tr("Changed"),
				 tr("Your plugin has been changed."));
}

void MainWindow::_save() {

	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}

	///message box zu verify if the button was clicked by purpose


	QFile outFile(outDir.absoluteFilePath(QString("%1.h")
				.arg(_inputName->text())));





	QMessageBox msgBox(
			QMessageBox::Question,
			tr("How to create the plugin?"),
			tr("Do you really want to create the plugin?"));
	QPushButton *overwriteButton = msgBox.addButton(tr("&Overwrite"), QMessageBox::AcceptRole);
	QPushButton *changeButton = msgBox.addButton(tr("Chan&ge"), QMessageBox::AcceptRole);
	QPushButton *acceptButton = msgBox.addButton(tr("Crea&te"), QMessageBox::AcceptRole);
	QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
	if (outFile.exists()){
		msgBox.setText("The output file you choose does already exist!\n"
			       "Please select one of the following options.\n\n"
			       "Overwrite: Overwrites the existing plugin, losing the previous data.\n"
			       "Change: Changes only the slots and parameters. \n"
			       "Cancel: Cancels the plugin creation, leaving the files unchanged.");

		msgBox.removeButton(acceptButton);
		}
	else{
		msgBox.removeButton(overwriteButton);
		msgBox.removeButton(changeButton);
		}

	msgBox.setDefaultButton(cancelButton);
	msgBox.exec();

	if (msgBox.clickedButton() == cancelButton) {
	// cancels plugin generation
		return;
		} else if (msgBox.clickedButton() == changeButton){
		_changeExisting();

	}


	else if (msgBox.clickedButton() == acceptButton || overwriteButton)


	{
	///saving settings and creating the plugin


	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());
	settings.setValue("recentAuthor", _inputAuthorName->text());

	QStringList newFiles;

	if (_templated->currentIndex() == 0) {


		QFile templatedPrototypHeaderFile(":/templates/res/TempH.h");
		if (!templatedPrototypHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempHText(&templatedPrototypHeaderFile);


		newFiles.append(TempHText.readAll());

		QFile templatedPrototypCppFile(":/templates/res/TempCpp.cpp");
		if (!templatedPrototypCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempCppText(&templatedPrototypCppFile);

		newFiles.append(TempCppText.readAll());


		QFile templatedPrototypHxxFile(":/templates/res/TempHxx.hxx");
		if (!templatedPrototypHxxFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempHxxText(&templatedPrototypHxxFile);


		newFiles.append(TempHxxText.readAll());





	}
	else{
		QFile nonTemplatedPrototypHeaderFile(":/templates/res/NonTempH.h");
		if (!nonTemplatedPrototypHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream nonTempHText(&nonTemplatedPrototypHeaderFile);



		newFiles.append( nonTempHText.readAll());

		QFile nonTemplatedPrototypCppFile(":/templates/res/NonTempCpp.cpp");
		if (!nonTemplatedPrototypCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream nonTempCppText(&nonTemplatedPrototypCppFile);

		newFiles.append( nonTempCppText.readAll());


	}

	QString str = newFiles.join("@@@@@@@@@@@");

	QDate* date = new QDate();


	str.replace(QString("@Author@"),_inputAuthorName->text().trimmed());
	str.replace(QString("@pluginName@"),_inputName->text().trimmed());
	str.replace(QString("@pluginNameUpper@"),_inputName->text().toUpper().trimmed());
	str.replace(QString("@pluginNameLower@"),_inputName->text().toLower().trimmed());
	str.replace("@PluginDoc@", _pluginDoc->toPlainText().replace("\n","\n/// ").trimmed());
	str.replace("@PluginDocu@", _pluginDoc->toPlainText().replace("\n","\"\n\t\t\t\"").trimmed());
	str.replace("@date@",date->currentDate().toString("dd.MM.yyyy"));


	for(int i = 0; i < _table1->rowCount(); i++) {

		QTableWidgetItem* IName = _table1->item(i,0);
		QTableWidgetItem* TempIDoc = _table1->item(i,1);
		QTableWidgetItem* ITyp = _table1->item(i,2);
		QString IDoc = TempIDoc->text();

		for(int ii = 0; ii < 3 ; ii++){
			int changeAt = 75 * (ii + 1) + ii * 8;
			if (IDoc.size() - ii * 8 > 75 * (ii + 1) ){
				IDoc.insert(changeAt,"\"\n\t\"");
			}
		}


		if(!QString(IName->text()).isEmpty()){


			str.replace("@In/Out@",QString("/// @Documentation@ \n\tInputSlot < @Typ@ > @I/O-Name@;"
				"\n\t@In/Out@"));
			str.replace("@multi/optional@",QString(",@I/O-Name@(@optional@, @multi@) "
				"\n\t\t\t@multi/optional@"));


			str.replace(QString("@Documentation@"),TempIDoc->text().replace("\n","\n\t/// ").trimmed());
			str.replace(QString("@Typ@"),ITyp->text().trimmed());
			str.replace("@I/O-Name@",IName->text().trimmed());
			if(_multiSlot->isChecked())
				str.replace("@multi@","true");
			else
				str.replace("@multi@","false");

			if(_optionalSlot->isChecked())
				str.replace("@optional@","true");
			else
				str.replace("@optional@","false");



			str.replace("@add-In/Out@",QString("ParameteredObject::_addInputSlot(@IOName@,"
				" \"@IOName@\", \n\t\"@Documentation@\", \n\t\"@Typ@\"); \n\t@add-In/Out@"));

			str.replace(QString("@Documentation@"),IDoc.trimmed());
			str.replace(QString("@Typ@"),ITyp->text().trimmed());
			str.replace(QString("@IOName@"),IName->text().trimmed());
		}

	}
	for(int k = 0; k < _table2->rowCount(); k++) {



		QTableWidgetItem* OName = _table2->item(k,0);
		QTableWidgetItem* TempODoc = _table2->item(k,1);
		QTableWidgetItem* OTyp = _table2->item(k,2);
		QString ODoc = TempODoc->text();

		for(int ii = 0; ii < 3 ; ii++){
			int changeAt = 75 * (ii + 1) + ii * 8;
			if (ODoc.size() - ii * 8 > 75 * (ii + 1) ){
				ODoc.insert(changeAt,"\"\n\t\"");
			}
		}

		if(!QString(OName->text()).isEmpty()){

			str.replace("@In/Out@",QString("/// @Documentation@ \n\tOutputSlot < @Typ@ > @I/O-Name@;"
			"\n\t@In/Out@"));


			str.replace(QString("@Documentation@"),TempODoc->text().replace("\n","\n\t/// ").trimmed());
			str.replace(QString("@Typ@"),OTyp->text().trimmed());
			str.replace("@I/O-Name@",OName->text().trimmed());

			str.replace("@add-In/Out@",QString("ParameteredObject::_addOutputSlot(@IOName@,"
				" \"@IOName@\", \n\t\"@Documentation@\", \n\t\"@Typ@\"); \n\t@add-In/Out@"));

			str.replace(QString("@Documentation@"),ODoc.trimmed());
			str.replace(QString("@Typ@"),OTyp->text().trimmed());
			str.replace(QString("@IOName@"),OName->text().trimmed());
		}

	}
	str.replace("@In/Out@","");
	str.replace("@add-In/Out@","");
	str.replace("@multi/optional@","");




	for (int j = 0; j < _table3->rowCount(); j++ ) {

		QTableWidgetItem* paraName = _table3->item(j,0);
		QTableWidgetItem* TempParaDoc = _table3->item(j,1);
		QTableWidgetItem* paraTyp = _table3->item(j,2);
		QTableWidgetItem* paraDefault = _table3->item(j,3);
		QString paraDoc = TempParaDoc->text();

		for(int ii = 0; ii < 3 ; ii++){
			int changeAt = 75 * (ii + 1) + ii * 8;
			if (paraDoc.size() - ii * 8 > 75 * (ii + 1) ){
				paraDoc.insert(changeAt,"\"\n\t\"");
			}
		}


		if(!QString(paraName->text()).isEmpty()){
			str.replace("@Parameter@",QString("/// @Documentation@ \n\tParameter < @Typ@ >"
				" @ParameterName@;\n\t@Parameter@"));
			str.replace("@ParameterName@",paraName->text().trimmed());
			str.replace("@Typ@",paraTyp->text().trimmed());
			str.replace("@Documentation@",TempParaDoc->text()
					.replace("\n","\n\t/// ").trimmed());
			}




		if(!QString(paraName->text()).isEmpty()){

			str.replace("@addParameter@",QString("ParameteredObject::_addParameter "
				"(@ParaName@, \"@ParaName@\", \n\t\"@Documentation@\", "
				"\n\t@Default@);\n\t@addParameter@"));
			str.replace("@ParaName@",paraName->text().trimmed());
			str.replace("@Documentation@",paraDoc.trimmed());
			str.replace("@Default@",paraDefault->text().trimmed());
		}

	}
	str.replace("@Parameter@","");
	str.replace("@addParameter@","");



	for (int l = 0; l < _table4->rowCount(); l++ ) {

		QTableWidgetItem* paraListName = _table4->item(l,0);
		QTableWidgetItem* TempParaListDoc = _table4->item(l,1);
		QTableWidgetItem* paraListTyp = _table4->item(l,2);
		QTableWidgetItem* paraListDefault = _table4->item(l,3);
		QString paraListDoc = TempParaListDoc->text();

		for(int ii = 0; ii < 3 ; ii++){
			int changeAt = 75 * (ii + 1) + ii * 8;
			if (paraListDoc.size() - ii * 8 > 75 * (ii + 1) ){
				paraListDoc.insert(changeAt,"\"\n\t\"");
			}
		}

		if(!QString(paraListName->text()).isEmpty()){
			str.replace("@ParameterList@",QString("/// @Documentation@ \n\tParameterList < @Typ@ >"
				" @ParameterName@;\n\t@ParameterList@"));
			str.replace("@ParameterListDefault@",QString(" ,@ParameterName@(@Default@)"
				"\n\t@ParameterListDefault@"));
			str.replace("@ParameterName@",paraListName->text().trimmed());
			str.replace("@Typ@",paraListTyp->text().trimmed());
			str.replace("@Documentation@",TempParaListDoc->text()
					.replace("\n","\n\t/// ").trimmed());
			str.replace("@Default@",paraListDefault->text().trimmed());
			}




		if(!QString(paraListName->text()).isEmpty()){

			str.replace("@addParameterList@",QString("ParameteredObject::_addParameter "
				"(@ParaListName@, \"@ParaListName@\", \n\t\"@Documentation@\" "
				");\n\t@addParameterList@"));
			str.replace("@ParaListName@",paraListName->text().trimmed());
			str.replace("@Documentation@",paraListDoc.trimmed());
		}

	}
	str.replace("@ParameterList@","");
	str.replace("@addParameterList@","");
	str.replace("@ParameterListDefault@","");




	QStringList editedNewFiles;
	editedNewFiles = str.split("@@@@@@@@@@@");


	if (_templated->currentIndex() == 0) {
		QFile templatedHeaderFile(outDir.absoluteFilePath(QString("%1.h")
				.arg(_inputName->text())));
		if (!templatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;



		QTextStream out(&templatedHeaderFile);
		out  << editedNewFiles.at(0) << "\n\n\n" ;


		QFile templatedCppFile(outDir.absoluteFilePath(QString("%1.cpp")
			.arg(_inputName->text())));

		if (!templatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out2(&templatedCppFile);
		out2 << editedNewFiles.at(1) << "\n\n\n";


		QFile templatedHxxFile(outDir.absoluteFilePath(QString("%1.hxx")
				.arg(_inputName->text())));
		if (!templatedHxxFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out3(&templatedHxxFile);
		out3  << editedNewFiles.at(2) << "\n\n\n";

		}

	else {
		QFile nontemplatedHeaderFile(outDir.absoluteFilePath(QString("%1.h")
			.arg(_inputName->text())));

		if (!nontemplatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

		QTextStream out4(&nontemplatedHeaderFile);
		out4 << editedNewFiles.at(0) << "\n\n\n";


		QFile nontemplatedCppFile(outDir.absoluteFilePath(QString("%1.cpp")
				.arg(_inputName->text())));
		if (!nontemplatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out5(&nontemplatedCppFile);
		out5 << editedNewFiles.at(1) << "\n\n\n";

		}
	QMessageBox::information(this, tr("Completed"),
				 tr("Your plugin has been generated."));
	}




}

void MainWindow::_selectOutputDir() {
	QString path = _inputDir->text();
	if (path.isEmpty() || !QFileInfo(path).isDir())
		path = QDir::homePath();
	QString dir = QFileDialog::getExistingDirectory(
			this, tr("Select output directory"),
			path,
			QFileDialog::ShowDirsOnly);
	if(!dir.isEmpty())
		_inputDir->setText(dir);
}


void MainWindow::_selectInputFile() {
	QString path = _inputFile1->itemText(_inputFile1->currentIndex()).trimmed().section("/",0,-2);
	if (path.isEmpty() || !QFileInfo(path).isDir()){
		path = QDir::homePath();
	}
	QString dir = QFileDialog::getOpenFileName(
			this, tr("Select output directory"),
			path);
	if(!dir.isEmpty()){
		_inputFile1->insertItem(0,dir);
		_inputFile1->setCurrentIndex(0);
	}
}



void MainWindow::_writeSettings() {
	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");

	settings.beginGroup("MainWindow");
	settings.setValue("geometry", saveGeometry());
	if (!isMaximized()) {
		settings.setValue("size", size());
		settings.setValue("pos", pos());
	}
	settings.endGroup();
}

void MainWindow::_readSettings() {
	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");

	settings.beginGroup("MainWindow");
	if (settings.contains("geometry"))
		restoreGeometry(settings.value("geometry").toByteArray());
	else {
		resize(settings.value("size", QSize(400, 400)).toSize());
		move(settings.value("pos", QPoint(200, 200)).toPoint());
	}
	settings.endGroup();
}

void MainWindow::_editRowCount(int table) {
	if(table == 0){
		int row = _table1->rowCount();
		_table1->insertRow(row);
		_table1->setItem(row, 0, new QTableWidgetItem());
		_table1->setItem(row, 1, new QTableWidgetItem());
		_table1->setItem(row, 2, new QTableWidgetItem());
	}
	if(table == 1){
		int row = _table2->rowCount();
		_table2->insertRow(row);
		_table2->setItem(row, 0, new QTableWidgetItem());
		_table2->setItem(row, 1, new QTableWidgetItem());
		_table2->setItem(row, 2, new QTableWidgetItem());
	}
	if(table == 2){
		int row = _table3->rowCount();
		_table3->insertRow(row);
		_table3->setItem(row, 0, new QTableWidgetItem());
		_table3->setItem(row, 1, new QTableWidgetItem());
		_table3->setItem(row, 2, new QTableWidgetItem());
		_table3->setItem(row, 3, new QTableWidgetItem());
	}
	if(table == 3){
		int row = _table4->rowCount();
		_table4->insertRow(row);
		_table4->setItem(row, 0, new QTableWidgetItem());
		_table4->setItem(row, 1, new QTableWidgetItem());
		_table4->setItem(row, 2, new QTableWidgetItem());
		_table4->setItem(row, 3, new QTableWidgetItem());
	}
	if(table == 4){
		if(_table1->currentRow()!= -1)
			_table1->removeRow(_table1->currentRow());
		else
			_table1->removeRow(_table1->rowCount() -1);
	}
	if(table == 5){
		if(_table2->currentRow()!= -1)
			_table2->removeRow(_table2->currentRow());
		else
			_table2->removeRow(_table2->rowCount() -1);
	}
	if(table == 6){
		if(_table3->currentRow()!= -1)
			_table3->removeRow(_table3->currentRow());
		else
			_table3->removeRow(_table3->rowCount() -1);
	}
	if(table == 7){
		if(_table4->currentRow()!= -1)
			_table4->removeRow(_table4->currentRow());
		else
			_table4->removeRow(_table4->rowCount() -1);
	}


}

void MainWindow::_load() {


	QString pluginFile;
	if (_inputFile1->itemText(_inputFile1->currentIndex()).trimmed().section(".",-1,-1) == "h"
	    ||_inputFile1->itemText(_inputFile1->currentIndex()).trimmed().section(".",-1,-1) == "hxx"
	    ||_inputFile1->itemText(_inputFile1->currentIndex()).trimmed().section(".",-1,-1) == "cpp")

		pluginFile = _inputFile1->itemText(_inputFile1->currentIndex()).trimmed().section(".",0,-2);
	else
		pluginFile = _inputFile1->itemText(_inputFile1->currentIndex()).trimmed();


	QFile inputFileName(QString("%1.h").arg(pluginFile));



	if (!inputFileName.exists()) {
		qWarning("Input plugin does not exist!");
		return;
	}

	for(; 0 < _table4->rowCount();) {
	_editRowCount(7);
	}
	for(; 0 < _table3->rowCount();) {
	_editRowCount(6);
	}
	for(; 0 < _table2->rowCount();) {
	_editRowCount(5);
	}
	for(; 0 < _table1->rowCount();) {
	_editRowCount(4);
	}







	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentInputDir5", settings.value("recentInputDir4").toString());
	settings.setValue("recentInputDir4", settings.value("recentInputDir3").toString());
	settings.setValue("recentInputDir3", settings.value("recentInputDir2").toString());
	settings.setValue("recentInputDir2", settings.value("recentInputDir1").toString());
	settings.setValue("recentInputDir1", pluginFile);


	QFile loadHeaderFile(QString("%1.h").arg(pluginFile));
		if (!loadHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

	QTextStream loadH(&loadHeaderFile);

	qWarning() << pluginFile;

	QFile loadCppFile(QString("%1.cpp").arg(pluginFile));
	if (!loadCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream loadCodeCpp(&loadCppFile);


	QFile loadHxxFile(QString("%1.hxx").arg(pluginFile));
	if (!loadHxxFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//return;
	}

	QTextStream loadCodeHxx(&loadHxxFile);

	QString codeFiles;
	codeFiles.append(loadH.readAll());
	codeFiles.append(loadCodeCpp.readAll());
	codeFiles.append(loadCodeHxx.readAll());

	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}

	QFile nontemplatedHeaderFile(outDir.absoluteFilePath(QString("%1.h")
			.arg("blub")));

	if (!nontemplatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out4(&nontemplatedHeaderFile);
	out4 << codeFiles << "\n\n\n";

	QTextStream Code(&codeFiles);

	int incount = 0;
	int outcount = 0;
	int paracount = 0;
	int paralistcount = 0;
	while(!Code.atEnd()){
		QString tempString;
		tempString = Code.readLine();


		QString tempString2 = tempString;
		tempString2.replace(" ","");

		if(tempString2.contains("Parameter<")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
			}

			tempString.replace(";","");
			_editRowCount(2);

			tempString.remove(0,tempString.indexOf("<") + 1);
			QString paraTypx = tempString.section(">",0,-2);
			QString paraNamex = tempString.section(">",-1,-1);
			_table3->item(paracount,0)->setText(paraNamex.trimmed());
			_table3->item(paracount,2)->setText(paraTypx.trimmed());



			paracount++;

		}


		if(tempString2.contains("_addParameter(")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
				tempString.remove("\t");
			}

			tempString.replace(";","");
			tempString.remove(QRegExp("\\)\\s*$"));
			tempString.remove("\"");
			//tempString.remove(QRegExp("^.*\\("));
			tempString.remove(0,tempString.indexOf("(") + 1);
			QString paraNamex = tempString.section(",",0,0);
			QString paraDocux = tempString.section(",",2,2);
			QString paraDefaultx = tempString.section(",",3,3);



			for(int table = 0; table < _table3->rowCount(); table++){
				QTableWidgetItem* editParaName = _table3->item(table,0);

				if(editParaName->text() == paraNamex.trimmed()){
					_table3->item(table,1)->setText(paraDocux);
					_table3->item(table,3)->setText(paraDefaultx);

				}
			}
		}

		if(tempString2.contains("ParameterList<")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
			}

			tempString.replace(";","");
			_editRowCount(3);

			tempString.remove(0,tempString.indexOf("<") + 1);
			QString paraTypx = tempString.section(">",0,-2);
			QString paraNamex = tempString.section(">",-1,-1);
			_table4->item(paralistcount,0)->setText(paraNamex.trimmed());
			_table4->item(paralistcount,2)->setText(paraTypx.trimmed());



			paralistcount++;

		}


		if(tempString2.contains("_addParameter(")){


			QString paraNamex = tempString.section(",",0,0);
			QString paraDocux = tempString.section(",",2,2);
			QString paraDefaultx = tempString.section(",",3,3);



			for(int table = 0; table < _table4->rowCount(); table++){
				QTableWidgetItem* editParaName = _table4->item(table,0);

				if(editParaName->text() == paraNamex.trimmed()){
					_table4->item(table,1)->setText(paraDocux);
					_table4->item(table,3)->setText(paraDefaultx);

				}
			}
		}


		if(tempString2.contains("InputSlot(")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
			}

			tempString.replace(";","");

			_editRowCount(0);
			tempString.remove(0,tempString.indexOf("(") + 1);
			tempString.remove("\"");
			tempString.remove(tempString.lastIndexOf(")"), 1);

			QString slotNamex = tempString.section(",",0,0);
			QString slotTypx = tempString.section(",",3,3);
			QString slotDocux = tempString.section(",",2,2);


			_table1->item(incount,1)->setText(slotDocux.trimmed());
			_table1->item(incount,2)->setText(slotTypx.trimmed());
			_table1->item(incount,0)->setText(slotNamex.trimmed());

			incount++;
		}
		if(tempString2.contains("OutputSlot(")){

			for(int count = 0; !tempString.contains(";") && count < 5; count++){

				tempString.append(Code.readLine());
			}

			tempString.replace(";","");

			_editRowCount(1);
			tempString.remove(0,tempString.indexOf("(") + 1);
			tempString.remove("\"");
			tempString.remove(tempString.lastIndexOf(")"), 1);

			QString slotNamex = tempString.section(",",0,0);
			QString slotTypx = tempString.section(",",3,3);
			QString slotDocux = tempString.section(",",2,2);


			_table2->item(outcount,1)->setText(slotDocux.trimmed());
			_table2->item(outcount,2)->setText(slotTypx.trimmed());
			_table2->item(outcount,0)->setText(slotNamex.trimmed());


			outcount++;
		}
	}

	QMessageBox::information(
			this, tr("Load"), tr("Your plugin has been loaded."));
}


void MainWindow::closeEvent(QCloseEvent* cEvent)
{
	if (QMessageBox::question(
			0, tr("confirm close"),
			tr("Do you really want to quit?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No) == QMessageBox::No) {
		cEvent->ignore();
	} else {
		_writeSettings();
		QMainWindow::closeEvent(cEvent);
	}
}

#include "MainWindow.moc"
