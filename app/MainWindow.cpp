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

	QWidget* page1 = new QWidget;
	tabWidget->addTab(page1, tr("Directory"));
	QWidget* page2 = new QWidget;
	tabWidget->addTab(page2, tr("Slots"));
	QWidget* page3 = new QWidget;
	tabWidget->addTab(page3, tr("Parameters"));
	QWidget* page4 = new QWidget;
	tabWidget->addTab(page4, tr("tab4"));
	QWidget* page5 = new QWidget;
	tabWidget->addTab(page5, tr("tab5"));


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


	_check1 = new QCheckBox(tr(""));
	QLabel* checktemplated = new QLabel(tr("templated Plugin"));
	_inputAuthorName = new QLineEdit(page1);
	_inputDir = new QLineEdit(page1);
	_inputName = new QLineEdit (page1);
	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
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
	layout->addWidget(_check1,10,3);
	layout->addWidget(checktemplated,10,1,1,2);
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
	QPushButton* nextButton2 = new QPushButton (tr("&Weiter"));
	QPushButton* add = new QPushButton (tr("&Add"));
	QPushButton* remove = new QPushButton (tr("&Remove"));
	QPushButton* exitButton2 = new QPushButton(tr("&Exit"));
	_table1 = new QTableWidget(2, 4, page2);
	QStringList longerList = (QStringList() << "Name" << "Input/Output" << "Documentation" << "Typ");
	_table1->setHorizontalHeaderLabels(longerList);
	_table1->verticalHeader()->hide();
	_table1->horizontalHeader()->setStretchLastSection(true);

	_table1->setCellWidget(0,2,new QTextEdit(page2));
	QTextEdit* documentation1 = qobject_cast<QTextEdit*>(_table1->cellWidget(0,2));
	Q_ASSERT(documentation1);
	documentation1->setTabChangesFocus(true);
	_table1->setCellWidget(0,0,new QLineEdit(page2));
	_table1->setCellWidget(0,1,new QComboBox(page2));
	QComboBox* combobox1 = qobject_cast<QComboBox*>(_table1->cellWidget(0,1));
	Q_ASSERT(combobox1);
	combobox1 ->addItem(tr("Input"));
	combobox1 ->addItem(tr("Output"));
	_table1->setCellWidget(0,3,new QLineEdit(page2));
	_table1->setCellWidget(1,2,new QTextEdit(page2));
	QTextEdit* documentation2 = qobject_cast<QTextEdit*>(_table1->cellWidget(1,2));
	Q_ASSERT(documentation2);
	documentation2->setTabChangesFocus(true);
	_table1->setCellWidget(1,0,new QLineEdit(page2));
	_table1->setCellWidget(1,1,new QComboBox(page2));
	QComboBox* combobox2 = qobject_cast<QComboBox*>(_table1->cellWidget(1,1));
	Q_ASSERT(combobox2);
	combobox2 ->addItem(tr("Input"));
	combobox2 ->addItem(tr("Output"));
	_table1->setCellWidget(1,3,new QLineEdit(page2));

	_table1->resizeColumnsToContents();

	layout2->addWidget(_table1,3,1);
	buttonlayout2->addWidget(add);
	buttonlayout2->addWidget(remove);
	buttonlayout2->addItem(spacer2);
	buttonlayout2->addWidget(exitButton2);
	buttonlayout2->addWidget(nextButton2);




	connect(exitButton2, SIGNAL(clicked()), this, SLOT(close()));
	connect(nextButton2,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
	connect(add,SIGNAL(clicked()), this, SLOT(_addslot()));
	connect(remove,SIGNAL(clicked()), this, SLOT(_removeslot()));


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
	QPushButton* add2 = new QPushButton (tr("&Add"));
	QPushButton* remove2 = new QPushButton (tr("&Remove"));
	QPushButton* exitButton3 = new QPushButton(tr("&Exit"));
	_table2 = new QTableWidget(2, 4, page3);
	QStringList paramlist = (QStringList() << "Name" << "Documentation" << "Typ" << "Default" );
	_table2->setHorizontalHeaderLabels(paramlist);
	layout3 -> addWidget(_table2,1,1);
	buttonlayout3->addWidget(add2);
	buttonlayout3->addWidget(remove2);
	buttonlayout3->addItem(spacer3);
	buttonlayout3->addWidget(exitButton3);
	buttonlayout3->addWidget(createButton);


	_table2->horizontalHeader()->setStretchLastSection(true);
	_table2->verticalHeader()->hide();


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

	_table2->resizeColumnsToContents();



	connect(exitButton3, SIGNAL(clicked()), this, SLOT(close()));
	connect(add2,SIGNAL(clicked()), this, SLOT(_addparameter()));
	connect(remove2,SIGNAL(clicked()), this, SLOT(_removeparameter()));

	// page 4
	QVBoxLayout* background4 = new QVBoxLayout(page4);
	QWidget* page4hi = new QWidget(page4);
	QWidget* page4lo = new QWidget(page4);
	background4->addWidget(page4hi,6);
	background4->addWidget(page4lo,1);
	QGridLayout* layout4 = new QGridLayout(page4hi);
	page4->setLayout(background4);
	page4hi->setLayout(layout4);
	QHBoxLayout* buttonlayout4 = new QHBoxLayout(page4lo);
	page4lo->setLayout(buttonlayout4);
	QSpacerItem* spacer4 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QPushButton* nextButton4 = new QPushButton (tr("&Weiter"));

	buttonlayout4->addItem(spacer4);
	buttonlayout4->addWidget(nextButton4);



	connect(nextButton4,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));


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

MainWindow::~MainWindow() {
}

void MainWindow::_showHello() {
	QMessageBox::information(this, tr("hello world box"),
				 tr("Welcome to this hello world application!"));
}

void MainWindow::_save() {

	///message box zu verify if the button was clicked by purpose

	QMessageBox msgBox;
	msgBox.setText("Do you really want to create the plugin?");
	QPushButton *acceptButton = msgBox.addButton(tr("Create"), QMessageBox::AcceptRole);
	QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
	msgBox.setDefaultButton(acceptButton);
	msgBox.exec();

	if (msgBox.clickedButton() == cancelButton) {
	return;
	} else if (msgBox.clickedButton() == acceptButton) {

	///saving settings and creating the plugin

	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}
	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());


	QStringList newFiles;

	if (_check1->isChecked()) {


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
	str.replace("@PluginDoc@", _pluginDoc->toPlainText().replace("\n","\n/// ").trimmed());
	str.replace("@PluginDocu@", _pluginDoc->toPlainText().replace("\n"," <br> ").trimmed());
	str.replace("@date@",date->currentDate().toString("dd.MM.yyyy"));


	for(int i = 0; i < _table1->rowCount(); i++) {

		QLineEdit* IOName = qobject_cast<QLineEdit*>(_table1->cellWidget(i,0));
		QComboBox* IOChoose = qobject_cast<QComboBox*>(_table1->cellWidget(i,1));
		QTextEdit* IODoc = qobject_cast<QTextEdit*>(_table1->cellWidget(i,2));
		QLineEdit* IOTyp = qobject_cast<QLineEdit*>(_table1->cellWidget(i,3));

		if(!QString(IOName->text()).isEmpty()){

		if(IOChoose->currentIndex() == 1)
		str.replace("@In/Out@",QString("/// @Documentation@ \n\tOutputSlot < @Typ@ > @I/O-Name@;"
			"\n\t@In/Out@"));

		else
		str.replace("@In/Out@",QString("/// @Documentation@ \n\tInputSlot < @Typ@ > @I/O-Name@;"
			"\n\t@In/Out@"));


		str.replace(QString("@Documentation@"),IODoc->toPlainText().replace("\n","\n\t/// ").trimmed());
		str.replace(QString("@Typ@"),IOTyp->text().trimmed());
		str.replace("@I/O-Name@",IOName->text().trimmed());
		}

		if(!QString(IOName->text()).isEmpty()){

		if (IOChoose->currentIndex() == 1)
			str.replace("@add-In/Out@",QString("ParameteredObject::_addOutputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

		else
			str.replace("@add-In/Out@",QString("ParameteredObject::_addInputSlot(@IOName@,"
				" \"@IOName@\", \"@Documentation@\", \"@Typ@\"); \n\t@add-In/Out@"));

		str.replace(QString("@Documentation@"),IODoc->toPlainText().replace("\n"," <br> ").trimmed());
		str.replace(QString("@Typ@"),IOTyp->text().trimmed());
		str.replace(QString("@IOName@"),IOName->text().trimmed());
		}

	}
	str.replace("@In/Out@","");
	str.replace("@add-In/Out@","");




	for (int j = 0; j < _table2->rowCount(); j++ ) {

		QLineEdit* paraName = qobject_cast<QLineEdit*>(_table2->cellWidget(j,0));
		QTextEdit* paraDoc = qobject_cast<QTextEdit*>(_table2->cellWidget(j,1));
		QLineEdit* paraTyp = qobject_cast<QLineEdit*>(_table2->cellWidget(j,2));
		QLineEdit* paraDefault = qobject_cast<QLineEdit*>(_table2->cellWidget(j,3));

		if(!QString(paraName->text()).isEmpty()){
			str.replace("@Parameter@",QString("/// @Documentation@ \n\tParameter < @Typ@ >"
				" @ParameterName@;\n\t@Parameter@"));
			str.replace("@ParameterName@",paraName->text().trimmed());
			str.replace("@Typ@",paraTyp->text().trimmed());
			str.replace("@Documentation@",paraDoc->toPlainText().replace("\n","\n\t/// ").trimmed());
			}




	if(!QString(paraName->text()).isEmpty()){

		str.replace("@addParameter@",QString("ParameteredObject::_addParameter "
			"(@ParaName@, \"@ParaName@\", \"@Documentation@\", "
			"\"@Default@\");"));
		str.replace("@ParaName@",paraName->text().trimmed());
		str.replace("@Documentation@",paraDoc->toPlainText().replace("\n"," <br> ").trimmed());
		str.replace("@Default@",paraDefault->text().trimmed());
		}

	}
	str.replace("@Parameter@","");
	str.replace("@addParameter@","");



	QStringList editedNewFiles;
	editedNewFiles = str.split("@@@@@@@@@@@");


	if (_check1->isChecked()) {
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
	QString dir = QFileDialog::getExistingDirectory(
			this, tr("Select output directory"),
			QDir::homePath(),
			QFileDialog::ShowDirsOnly);
	if(!dir.isEmpty())
		_inputDir->setText(dir);
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

void MainWindow::_addslot() {
	int row = _table1->rowCount();
	_table1->insertRow(row);

	_table1->setCellWidget(row,2,new QTextEdit());
	QTextEdit* documentationx = qobject_cast<QTextEdit*>(_table1->cellWidget(row,2));
	Q_ASSERT(documentationx);
	documentationx->setTabChangesFocus(true);
	_table1->setCellWidget(row,0,new QLineEdit());
	_table1->setCellWidget(row,1,new QComboBox());
	QComboBox* comboboxx = qobject_cast<QComboBox*>(_table1->cellWidget(row,1));
	Q_ASSERT(comboboxx);
	comboboxx ->addItem(tr("Input"));
	comboboxx ->addItem(tr("Output"));
	_table1->setCellWidget(row,3,new QLineEdit());
	}

void MainWindow::_removeslot() {
	_table1->removeRow(_table1->rowCount() -1);
	}


void MainWindow::_addparameter() {
	int row = _table2->rowCount();
	_table2->insertRow(row);

	_table2->setCellWidget(row,1,new QTextEdit());
	QTextEdit* documentationx = qobject_cast<QTextEdit*>(_table2->cellWidget(row,1));
	Q_ASSERT(documentationx);
	documentationx->setTabChangesFocus(true);
	_table2->setCellWidget(row,0,new QLineEdit());
	_table2->setCellWidget(row,2,new QLineEdit());
	_table2->setCellWidget(row,3,new QLineEdit());
	}

void MainWindow::_removeparameter() {
	_table2->removeRow(_table2->rowCount() -1);
	}


void MainWindow::closeEvent(QCloseEvent* event)
{

	QMessageBox msgBox2;
	msgBox2.setText("Do you really want to quit?");
	QPushButton *acceptButton2 = msgBox2.addButton(tr("Exit"), QMessageBox::AcceptRole);
	QPushButton *cancelButton2 = msgBox2.addButton(tr("Cancel"), QMessageBox::RejectRole);
	msgBox2.setDefaultButton(acceptButton2);
	msgBox2.exec();

	if (msgBox2.clickedButton() == cancelButton2) {
	event->ignore();;
	} else if (msgBox2.clickedButton() == acceptButton2) {
	_writeSettings();
	QMainWindow::closeEvent(event);
	}
}

#include "MainWindow.moc"
