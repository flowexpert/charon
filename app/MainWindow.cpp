#include "MainWindow.h"

#include <QFile>
#include <QtGui>
#include <MyLabel.h>
#include <MyTabWidget.h>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent)
{
	QWidget* centralWidget = new QWidget;
	setCentralWidget(centralWidget);
	// restore window geometry
	_readSettings();

	QGridLayout* layoutmain = new QGridLayout(centralWidget);
	centralWidget-> setLayout(layoutmain);
	MyTabWidget *tabWidget = new MyTabWidget(centralWidget);
	layoutmain->addWidget(tabWidget,1,1);

	QWidget* page0 = new QWidget;
	tabWidget->addTab(page0, tr("Load"));
	QWidget* page1 = new QWidget;
	tabWidget->addTab(page1, tr("Directory"));
	QWidget* page2 = new QWidget;
	tabWidget->addTab(page2, tr("Slots"));
	QWidget* page3 = new QWidget;
	tabWidget->addTab(page3, tr("Parameters"));
	QWidget* page5 = new QWidget;
	tabWidget->addTab(page5, tr("tab5"));


	// page 0
	QVBoxLayout* background0 = new QVBoxLayout(page0);
	QWidget* page0hi = new QWidget(page0);
	QWidget* page0lo = new QWidget(page0);
	background0->addWidget(page0hi,6);
	background0->addWidget(page0lo,1);
	QGridLayout* layout0 = new QGridLayout(page0hi);
	page0->setLayout(background0);
	page0hi->setLayout(layout0);
	QHBoxLayout* buttonlayout0 = new QHBoxLayout(page0lo);
	page0lo->setLayout(buttonlayout0);
	QSpacerItem* spacer4 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QPushButton* nextButton4 = new QPushButton (tr("&Weiter"));
	QLabel* welcome = new QLabel(tr("Welcome to the template generator plugin.\n\n\n"
					"If you want to load an already existing plugin"
					" use the browse button to find the header file.\n"
					"Otherwise continue without loading."));
	QPushButton* load = new QPushButton(tr("Load"));
	QPushButton* browse2 = new QPushButton(tr("Browse"));
	_inputFile = new QLineEdit();

	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	_inputFile -> setText(settings.value("recentOutputDir", QDir::homePath())
		.toString());

	layout0->addWidget(welcome,2,1,1,3);
	layout0->addWidget(_inputFile,4,1);
	layout0->addWidget(browse2,4,2);
	layout0->setColumnStretch(3,1);
	layout0->setRowStretch(3,2);
	layout0->setRowStretch(5,2);
	layout0->setRowStretch(1,1);
	buttonlayout0->addItem(spacer4);
	buttonlayout0->addWidget (load);
	buttonlayout0->addWidget(nextButton4);



	connect(nextButton4,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(load,SIGNAL(clicked()),this, SLOT(_load()));
	connect(browse2,SIGNAL(clicked()),this,SLOT(_selectInputFile()));





	// page 1
	QVBoxLayout* background1 = new QVBoxLayout(page1);
	QWidget* page1hi = new QWidget(page1);
	QWidget* page1lo = new QWidget(page1);
	background1->addWidget(page1hi,6);
	background1->addWidget(page1lo,1);
	QGridLayout* layout = new QGridLayout(page1hi);
	page1->setLayout(background1);
	page1hi->setLayout(layout);
	QHBoxLayout* buttonlayout1 = new QHBoxLayout(page1lo);
	page1lo->setLayout(buttonlayout1);
	QPushButton* helloButton = new QPushButton(tr("hello world"));


	MyLabel* label = new MyLabel;
	QSpacerItem* spacer1 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QPushButton* nextButton1 = new QPushButton (tr("&Weiter"));
	QPushButton* browse = new QPushButton (tr("Browse"));
	QPushButton* exitButton = new QPushButton(tr("&Exit"));
	QLabel* name = new QLabel (tr("Name of plug in:"));
	QLabel* save = new QLabel (tr("saved in:"));
	QLabel* author = new QLabel (tr("Author:"));
	_pluginDoc = new QTextEdit(page1);
	QLabel* pluginDocLabel = new QLabel(tr("plugin description:"));


	//_check1 = new QCheckBox(tr(""));
	_templated = new QComboBox();
	_templated->addItem(tr("Templated"));
	_templated->addItem(tr("Non-Templated"));

	//QLabel* checktemplated = new QLabel(tr("templated Plugin"));
	_inputAuthorName = new QLineEdit(page1);
	_inputDir = new QLineEdit(page1);
	_inputName = new QLineEdit (page1);
	_inputDir -> setText(settings.value("recentOutputDir", QDir::homePath())
		.toString());
	_inputName -> setText(tr("plugin1"));
	_inputAuthorName -> setText (tr("Unknown"));
	// add directory completer for output directory
	QCompleter* completer = new QCompleter(this);
	completer->setModel(new QDirModel(QStringList(), QDir::AllDirs,
			QDir::Name, completer));
	_inputDir->setCompleter(completer);

/*
	QGroupBox *groupBox = new QGroupBox(page1);


	QGridLayout *vbox = new QGridLayout;
	vbox->addWidget(author,3,1,1,2);
	vbox->addWidget(_inputAuthorName,3,3);
	vbox->addWidget(name,4,1,1,2);
	vbox->addWidget(save,5,1,1,2);
	vbox->addWidget(_inputName,4,3);
	vbox->addWidget(_inputDir,5,3);
	vbox->addWidget(browse,5,4);
	groupBox->setLayout(vbox);
*/


	label->setText(tr("Please choose a name and directory to save"));
	layout->addWidget(label,1,1,1,3,Qt::AlignCenter);
	layout->addWidget(_templated,10,3);
	//layout->addWidget(checktemplated,10,1,1,2);
	layout->addWidget(author,3,1,1,2);
	layout->addWidget(_inputAuthorName,3,3);
	layout->addWidget(name,4,1,1,2);
	layout->addWidget(save,5,1,1,2);
	layout->addWidget(_inputName,4,3);
	layout->addWidget(_inputDir,5,3);
	layout->addWidget(browse,5,4);

	//layout->addWidget(groupBox,3,1,3,3);
	buttonlayout1->addItem(spacer1);
	buttonlayout1->addWidget(exitButton);
	buttonlayout1->addWidget(nextButton1);
	layout->setColumnStretch(1,1);
	layout->setColumnStretch(6,1);
	layout->setRowStretch(2,1);
	layout->setRowStretch(6,1);
	layout->addWidget(pluginDocLabel,11,1,1,2);
	layout->addWidget(_pluginDoc,11,3);
	layout->setRowStretch(15,1);



	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(helloButton, SIGNAL(clicked()), this, SLOT(_showHello()));
	connect(nextButton1, SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(browse, SIGNAL(clicked()), this, SLOT(_selectOutputDir()));


	// page 2
	QVBoxLayout* background2 = new QVBoxLayout(page2);
	QWidget* page2hi = new QWidget(page2);
	QWidget* page2lo = new QWidget(page2);
	background2->addWidget(page2hi,6);
	background2->addWidget(page2lo,1);
	QGridLayout* layout2 = new QGridLayout(page2hi);
	page2->setLayout(background2);
	page2hi->setLayout(layout2);
	QHBoxLayout* buttonlayout2 = new QHBoxLayout(page2lo);
	page2lo->setLayout(buttonlayout2);
	QSpacerItem* spacer2 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QSpacerItem* spacerMid = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QPushButton* nextButton2 = new QPushButton (tr("&Weiter"));
	QPushButton* add = new QPushButton (tr("&Add"));
	QPushButton* remove = new QPushButton (tr("&Remove"));
	QPushButton* add2 = new QPushButton (tr("&Add"));
	QPushButton* remove2 = new QPushButton (tr("&Remove"));
	QPushButton* exitButton2 = new QPushButton(tr("&Exit"));
	QLabel* inputLabel = new QLabel (tr("Input Slots"));
	QLabel* outputLabel = new QLabel (tr("Output Slots"));
	QWidget* page2mid = new QWidget(page2);
	QHBoxLayout* buttonMidLayout = new QHBoxLayout();
	_table1 = new QTableWidget(0, 3, page2);
	QStringList longerList = (QStringList() << "Name"  << "Documentation" << "Typ");
	_table1->setHorizontalHeaderLabels(longerList);
	_table1->verticalHeader()->hide();
	_table1->horizontalHeader()->setStretchLastSection(true);
	_table1->resizeColumnsToContents();
	_table1->setColumnWidth(0, 200);
	_table1->setColumnWidth(2, 300);

	_editRowCount(0);
	_editRowCount(0);

	_table2 = new QTableWidget(0, 3, page2);
	_table2->setHorizontalHeaderLabels(longerList);
	_table2->verticalHeader()->hide();
	_table2->horizontalHeader()->setStretchLastSection(true);
	_table2->resizeColumnsToContents();
	_table2->setColumnWidth(0, 200);
	_table2->setColumnWidth(2, 300);

	_editRowCount(1);
	_editRowCount(1);

	layout2->addWidget(inputLabel,1,1);
	layout2->addWidget(outputLabel,4,1);
	layout2->addWidget(_table1,2,1);
	layout2->addWidget(_table2,5,1);
	layout2->addWidget(page2mid,3,1);
	page2mid->setLayout(buttonMidLayout);
	buttonMidLayout->addWidget(add);
	buttonMidLayout->addWidget(remove);
	buttonMidLayout->addItem(spacerMid);
	buttonlayout2->addWidget(add2);
	buttonlayout2->addWidget(remove2);
	buttonlayout2->addItem(spacer2);
	buttonlayout2->addWidget(exitButton2);
	buttonlayout2->addWidget(nextButton2);

	signalMapper = new QSignalMapper(this);
	connect(add, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add, 0);
	connect(signalMapper, SIGNAL(mapped(int)),
		this, SIGNAL(clicked(int)));
	connect(add2, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add2, 1);
	connect(remove, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove, 3);
	connect(remove2, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove2, 4);








	connect(exitButton2, SIGNAL(clicked()), this, SLOT(close()));
	connect(nextButton2,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(this,SIGNAL(clicked(int)), this, SLOT(_editRowCount(int)));
	connect(_table1, SIGNAL(itemChanged(QTableWidgetItem*)),
		this, SLOT(_itemChanged(QTableWidgetItem*)));


	// page 3
	QVBoxLayout* background3 = new QVBoxLayout(page3);
	QWidget* page3hi = new QWidget(page3);
	QWidget* page3lo = new QWidget(page3);
	background3->addWidget(page3hi,6);
	background3->addWidget(page3lo,1);
	QGridLayout* layout3 = new QGridLayout(page3hi);
	page3->setLayout(background3);
	page3hi->setLayout(layout3);
	QHBoxLayout* buttonlayout3 = new QHBoxLayout(page3lo);
	page3lo->setLayout(buttonlayout3);
	QSpacerItem* spacer3 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QPushButton* createButton = new QPushButton(tr("&Create"));
	QPushButton* add3 = new QPushButton (tr("&Add"));
	QPushButton* remove3 = new QPushButton (tr("&Remove"));
	QPushButton* exitButton3 = new QPushButton(tr("&Exit"));
	QLabel* parameterLabel = new QLabel (tr("Input Slots"));
	_table3 = new QTableWidget(0, 4, page3);
	QStringList paramlist = (QStringList() << "Name" << "Documentation" << "Typ" << "Default" );
	_table3->setHorizontalHeaderLabels(paramlist);
	layout3->addWidget(parameterLabel,1,1);
	layout3 -> addWidget(_table3,2,1);
	buttonlayout3->addWidget(add3);
	buttonlayout3->addWidget(remove3);
	buttonlayout3->addItem(spacer3);
	buttonlayout3->addWidget(exitButton3);
	buttonlayout3->addWidget(createButton);

	_editRowCount(2);
	_editRowCount(2);

	connect(add3, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(add3, 2);
	connect(remove3, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(remove3, 5);



	_table3->horizontalHeader()->setStretchLastSection(true);
	_table3->verticalHeader()->hide();

/*
	_table2->setCellWidget(0,1,new QTextEdit(page3));
	QTextEdit* documentation3 = qobject_cast<QTextEdit*>(_table2->cellWidget(0,1));
	Q_ASSERT(documentation3);
	documentation3->setTabChangesFocus(true);
	_table2->setCellWidget(0,0,new QLineEdit(page3));
	_table2->setCellWidget(0,2,new QLineEdit(page3));
	_table2->setCellWidget(0,3,new QLineEdit(page3));
	_table2->setCellWidget(1,1,new QTextEdit(page3));
	QTextEdit* documentation4 = qobject_cast<QTextEdit*>(_table2->cellWidget(1,1));
	Q_ASSERT(documentation4);
	documentation4->setTabChangesFocus(true);
	_table2->setCellWidget(1,0,new QLineEdit(page3));
	_table2->setCellWidget(1,2,new QLineEdit(page3));
	_table2->setCellWidget(1,3,new QLineEdit(page3));
*/
	_table2->resizeColumnsToContents();



	connect(exitButton3, SIGNAL(clicked()), this, SLOT(close()));




	// page 5
	QGridLayout* layout5 = new QGridLayout(page5);
	page5->setLayout(layout5);
	layout5->setColumnStretch(5,1);
	layout5->addWidget(helloButton,2,2);
	QSizePolicy policy(helloButton->sizePolicy());
	policy.setVerticalPolicy(QSizePolicy::Expanding);
	helloButton->setSizePolicy(policy);



	connect(createButton, SIGNAL(clicked()), this, SLOT(_save()));
}

void MainWindow::_itemChanged(QTableWidgetItem* item) {
	qDebug() << "changed row=" << item->row()
			<< "; changed column=" << item->column();
}

MainWindow::~MainWindow() {
}

void MainWindow::_showHello() {
	QMessageBox::information(this, tr("hello world box"),
				 tr("Welcome to this hello world application!"));
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
	} else if (msgBox.clickedButton() == acceptButton || overwriteButton)


	{
	///saving settings and creating the plugin


	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());


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
	str.replace("@PluginDocu@", _pluginDoc->toPlainText().replace("\n"," <br> ").trimmed());
	str.replace("@date@",date->currentDate().toString("dd.MM.yyyy"));


	for(int i = 0; i < _table1->rowCount(); i++) {

		QTableWidgetItem* IName = _table1->item(i,0);
		QTableWidgetItem* IDoc = _table1->item(i,1);
		QTableWidgetItem* ITyp = _table1->item(i,2);

		IName->text();

		if(!QString(IName->text()).isEmpty()){

		//if(IOChoose->currentIndex() == 1)
		//str.replace("@In/Out@",QString("/// @Documentation@ \n\tOutputSlot < @Typ@ > @I/O-Name@;"
		//	"\n\t@In/Out@"));

		//else
			str.replace("@In/Out@",QString("/// @Documentation@ \n\tInputSlot < @Typ@ > @I/O-Name@;"
				"\n\t@In/Out@"));


			str.replace(QString("@Documentation@"),IDoc->text().replace("\n","\n\t/// ").trimmed());
			str.replace(QString("@Typ@"),ITyp->text().trimmed());
			str.replace("@I/O-Name@",IName->text().trimmed());
		//}

		//

//		if (IOChoose->currentIndex() == 1)
//			str.replace("@add-In/Out@",QString("ParameteredObject::_addOutputSlot(@IOName@,"
//				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

//		else

			str.replace("@add-In/Out@",QString("ParameteredObject::_addInputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

			str.replace(QString("@Documentation@"),IDoc->text().replace("\n"," <br> ").trimmed());
			str.replace(QString("@Typ@"),ITyp->text().trimmed());
			str.replace(QString("@IOName@"),IName->text().trimmed());
		}

	}
	for(int k = 0; k < _table2->rowCount(); k++) {



		QTableWidgetItem* OName = _table2->item(k,0);
		QTableWidgetItem* ODoc = _table2->item(k,1);
		QTableWidgetItem* OTyp = _table2->item(k,2);

		if(!QString(OName->text()).isEmpty()){

			str.replace("@In/Out@",QString("/// @Documentation@ \n\tOutputSlot < @Typ@ > @I/O-Name@;"
			"\n\t@In/Out@"));


			str.replace(QString("@Documentation@"),ODoc->text().replace("\n","\n\t/// ").trimmed());
			str.replace(QString("@Typ@"),OTyp->text().trimmed());
			str.replace("@I/O-Name@",OName->text().trimmed());

			str.replace("@add-In/Out@",QString("ParameteredObject::_addOutputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

			str.replace(QString("@Documentation@"),ODoc->text().replace("\n"," <br> ").trimmed());
			str.replace(QString("@Typ@"),OTyp->text().trimmed());
			str.replace(QString("@IOName@"),OName->text().trimmed());
		}

	}
	str.replace("@In/Out@","");
	str.replace("@add-In/Out@","");




	for (int j = 0; j < _table3->rowCount(); j++ ) {

		QTableWidgetItem* paraName = _table3->item(j,0);
		QTableWidgetItem* paraDoc = _table3->item(j,1);
		QTableWidgetItem* paraTyp = _table3->item(j,2);
		QTableWidgetItem* paraDefault = _table3->item(j,3);

		if(!QString(paraName->text()).isEmpty()){
			str.replace("@Parameter@",QString("/// @Documentation@ \n\tParameter < @Typ@ >"
				" @ParameterName@;\n\t@Parameter@"));
			str.replace("@ParameterName@",paraName->text().trimmed());
			str.replace("@Typ@",paraTyp->text().trimmed());
			str.replace("@Documentation@",paraDoc->text()
					.replace("\n","\n\t/// ").trimmed());
			}




		if(!QString(paraName->text()).isEmpty()){

			str.replace("@addParameter@",QString("ParameteredObject::_addParameter "
				"(@ParaName@, \"@ParaName@\", \"@Documentation@\", "
				"\"@Default@\");\n\t@addParameter@"));
			str.replace("@ParaName@",paraName->text().trimmed());
			str.replace("@Documentation@",paraDoc->text()
					.replace("\n"," <br> ").trimmed());
			str.replace("@Default@",paraDefault->text().trimmed());
		}

	}
	str.replace("@Parameter@","");
	str.replace("@addParameter@","");



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
	QString path = _inputFile->text();
	if (path.isEmpty() || !QFileInfo(path).isFile() && !QFileInfo(path).isDir()){
		path = QDir::homePath();
	}
	QString dir = QFileDialog::getOpenFileName(
			this, tr("Select output directory"),
			path);
	if(!dir.isEmpty())
		_inputFile->setText(dir);
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
		if(_table1->currentRow()!= -1)
			_table1->removeRow(_table1->currentRow());
		else
			_table1->removeRow(_table1->rowCount() -1);
	}
	if(table == 4){
		if(_table2->currentRow()!= -1)
			_table2->removeRow(_table2->currentRow());
		else
			_table2->removeRow(_table2->rowCount() -1);
	}
	if(table == 5){
		if(_table3->currentRow()!= -1)
			_table3->removeRow(_table3->currentRow());
		else
			_table3->removeRow(_table3->rowCount() -1);
	}


	/*_table1->setCellWidget(row,2,new QTextEdit());
	QTextEdit* documentationx = qobject_cast<QTextEdit*>(_table1->cellWidget(row,2));
	Q_ASSERT(documentationx);
	documentationx->setTabChangesFocus(true);
	documentationx->setFrameShape(QFrame::NoFrame);
	documentationx->setFrameShadow(QFrame::Plain);

	_table1->setCellWidget(row,0,new QLineEdit());
*/
}

void MainWindow::_load() {

	QFile outFile(_inputFile->text());
	if (!outFile.exists()) {
		qWarning("Input file does not exist!");
		return;
	}

	for(; 0 < _table3->rowCount();) {
	_editRowCount(5);
	}
	for(; 0 < _table2->rowCount();) {
	_editRowCount(4);
	}
	for(; 0 < _table1->rowCount();) {
	_editRowCount(3);
	}

	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}

	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());



	QFile loadHeaderFile(_inputFile->text());
		if (!loadHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

	QTextStream loadH(&loadHeaderFile);

	QFile loadCppFile(_inputFile->text().remove(".h").append(".cpp"));
	if (!loadCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream loadCodeCpp(&loadCppFile);







	QFile loadHxxFile(_inputFile->text().append("xx"));
	if (!loadHxxFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//return;
	}

	QTextStream loadCodeHxx(&loadHxxFile);


	QString codeFiles;
	codeFiles.append(loadH.readAll());
	codeFiles.append(loadCodeCpp.readAll());
	codeFiles.append(loadCodeHxx.readAll());


	QTextStream Code(&codeFiles);



	int incount = 0;
	int outcount = 0;
	int paracount = 0;
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
				qWarning() << tempString;
			}

			tempString.replace(";","");
			tempString.remove(QRegExp("\\)\\s*$"));
			tempString.remove("\"");
			//tempString.remove(QRegExp("^.*\\("));
			tempString.remove(0,tempString.indexOf("(") + 1);
			qWarning() << tempString;
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


		if(tempString2.contains("InputSlot(", Qt::CaseInsensitive)){

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
		if(tempString2.contains("OutputSlot(", Qt::CaseInsensitive)){

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


}


void MainWindow::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::question(
			0, tr("confirm close"),
			tr("Do you really want to quit?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No) == QMessageBox::No) {
		event->ignore();
	} else {
		_writeSettings();
		QMainWindow::closeEvent(event);
	}
}

#include "MainWindow.moc"
