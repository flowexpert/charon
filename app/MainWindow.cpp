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
	connect(browse, SIGNAL(clicked()), this, SLOT(_selectOutputDir()));
	QLabel* name = new QLabel (tr("Name of plug in:"));
	QLabel* save = new QLabel (tr("saved in:"));
	QLabel* author = new QLabel (tr("Author:"));


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

	label->setText(tr("Please choose a name and a directory to save"));
	layout->addWidget(label,1,1,1,3,Qt::AlignCenter);
	layout->addWidget(author,3,1,1,2);
	layout->addWidget(_inputAuthorName,3,3);
	layout->addWidget(name,4,1,1,2);
	layout->addWidget(save,5,1,1,2);
	layout->addWidget(_inputName,4,3);
	layout->addWidget(_inputDir,5,3);
	layout->addWidget(browse,5,4);
	buttonlayout1->addItem(spacer1);
	buttonlayout1->addWidget(nextButton1);
	layout->setColumnStretch(1,1);
	layout->setColumnStretch(6,1);
	layout->setRowStretch(2,1);
	layout->setRowStretch(6,1);



	connect(helloButton, SIGNAL(clicked()), this, SLOT(_showHello()));
	connect(nextButton1, SIGNAL(clicked()), tabWidget, SLOT(nextPage()));


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
	QTableWidget* table2 = new QTableWidget(5, 4, page2);
	_check1 = new QCheckBox(tr("templated Plugin"));
	QSpacerItem* spacertable1 = new QSpacerItem(20,20, QSizePolicy::Minimum, QSizePolicy::Minimum);
	QStringList longerList = (QStringList() << "Name" << "Input/Output" << "Documentation" << "Typ");
	table2->setHorizontalHeaderLabels(longerList);
	_inputIOName1 = new QLineEdit(page2);
	_inputIOName2 = new QLineEdit(page2);
	_inputIOName3 = new QLineEdit(page2);
	_inputIOName4 = new QLineEdit(page2);
	_inputIOName5 = new QLineEdit(page2);
	_inputIODocumentation = new QTextEdit(page2);
	_inputIOTyp = new QLineEdit(page2);
	/*_check2 = new QCheckBox(page2);
	_check3 = new QCheckBox(tr("Parameter2"));
	_check4 = new QCheckBox(tr("Parameter3"));
	_check5 = new QCheckBox(tr("Input1"));
	_check6 = new QCheckBox(tr("Input2"));
	_check7 = new QCheckBox(tr("Output"));
	layout2->addWidget(_check3,5,1);
	layout2->setRowStretch(1,1);
	layout2->setRowStretch(3,1);
	layout2->addWidget(_check2,4,1);
	layout2->addWidget(_check4,6,1);
	layout2->addWidget(_check5,7,1);
	layout2->addWidget(_check6,8,1);
	layout2->addWidget(_check7,9,1); */
	_combo1 = new QComboBox(page3);
	_combo1 ->addItem(tr("Input"));
	_combo1 ->addItem(tr("Output"));
	QComboBox* combo2 = new QComboBox(page3);
	combo2 ->addItem(tr("Input"));
	combo2 ->addItem(tr("Output"));
	table2->setCellWidget(0,0,_inputIOName1);
	table2->setCellWidget(1,0,_inputIOName2);
	table2->setCellWidget(2,0,_inputIOName3);
	table2->setCellWidget(3,0,_inputIOName4);
	table2->setCellWidget(4,0,_inputIOName5);
	table2->setCellWidget(0,3,_inputIOTyp);
	table2->setCellWidget(0,2,_inputIODocumentation);
	table2->setCellWidget(0,1,_combo1);
	table2->setCellWidget(1,1,combo2);
	layout2->addWidget(_check1,1,1);
	layout2->addWidget(table2,3,1);
	layout2->addItem(spacertable1,3,2);
	//layout2->setColumnStretch(4,1);
	buttonlayout2->addItem(spacer2);
	buttonlayout2->addWidget(nextButton2);


	connect(nextButton2,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));

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
	QPushButton* nextButton3 = new QPushButton (tr("&Weiter"));
	QPushButton* createButton = new QPushButton(tr("&Create"));
	table1 = new QTableWidget(12, 4, page3);
	QStringList paramlist = (QStringList() << "Name" << "Documentation" << "Typ" << "Default" );
	table1->setHorizontalHeaderLabels(paramlist);
	layout3 -> addWidget(table1,1,1);
	buttonlayout3->addItem(spacer3);
	buttonlayout3->addWidget(createButton);
	buttonlayout3->addWidget(nextButton3);
	_inputParaName = new QLineEdit(page3);
	_inputParaTyp = new QLineEdit(page3);
	_inputParaDocumentation = new QTextEdit(page3);
	_inputParaDefault = new QLineEdit(page3);
	table1->setCellWidget(0,0,_inputParaName);
	table1->setCellWidget(0,1,_inputParaDocumentation);
	table1->setCellWidget(0,2,_inputParaTyp);
	table1->setCellWidget(0,3,_inputParaDefault);


	//layout3->setColumnStretch(5,1);

	connect(nextButton3,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));

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
	QPushButton* exitButton = new QPushButton(tr("&Exit"));
	buttonlayout4->addItem(spacer4);
	buttonlayout4->addWidget(exitButton);
	buttonlayout4->addWidget(nextButton4);


	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
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

	QDir outDir(_inputDir->text());
	if (!outDir.exists()) {
		qWarning("Output directory does not exist!");
		return;
	}
	QSettings settings("Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	settings.setValue("recentOutputDir", outDir.absolutePath());

	if (_check1->isChecked()) {


		QFile templatedPrototypHeaderFile(":/templates/res/TempH.h");
		if (!templatedPrototypHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempHText(&templatedPrototypHeaderFile);

		QString newTempHeader;

		newTempHeader = TempHText.readAll();
		newTempHeader.replace(QString("@Author@"),_inputAuthorName->text());
		newTempHeader.replace(QString("@pluginName@"),_inputName->text());

		if(!QString(_inputIOName1->text()).isEmpty()){

		if(_combo1->currentIndex() == 1){
		newTempHeader.replace("@In/Out@",QString("@Documentation@ \n OutputSlot<cimg_library::CImgList<@Typ@> > ").append(QString("%1;")
			.arg(_inputIOName1->text())));}
		else
		newTempHeader.replace("@In/Out@",QString("@Documentation@ \n InputSlot<cimg_library::CImgList<@Typ@> > ").append(QString("%1;")
			.arg(_inputIOName1->text())));

		newTempHeader.replace(QString("@Documentation@"),_inputIODocumentation->toPlainText());
		newTempHeader.replace(QString("@Typ@"),_inputIOTyp->text());
		}
		else
			newTempHeader.replace("@In/Out@","");

		if(!QString(_inputParaName->text()).isEmpty()){
			newTempHeader.replace("@Parameter@",QString("@Documentation@ \n Parameter<@Typ@> @ParameterName@;"));
			newTempHeader.replace("@ParameterName@",_inputParaName->text());
			newTempHeader.replace("@Typ@",_inputParaTyp->text());
			newTempHeader.replace("@Documentation@",_inputParaDocumentation->toPlainText());
			}
		else
			newTempHeader.replace("@Parameter@","");


		QFile templatedHeaderFile(outDir.absoluteFilePath(QString("%1.h")
				.arg(_inputName->text())));
		if (!templatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;



		QTextStream out(&templatedHeaderFile);
		out  << newTempHeader << "\n\n\n" ;

		/*if (_check1->isChecked())
			out << "This is a templated plugin! \n\n\n\n\n";
		if (_check2->isChecked())
			out << "Parameter1 \n";
		if (_check3->isChecked())
			out << "Parameter2 \n";
		if (_check4->isChecked())
			out << "Parameter3 \n";
		if (_check5->isChecked())
			out << "Input1 \n";
		if (_check6->isChecked())
			out << "Input2 \n";
		if (_check6->isChecked())
			out << "Output \n";*/





		QFile templatedPrototypCppFile(":/templates/res/TempCpp.cpp");
		if (!templatedPrototypCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempCppText(&templatedPrototypCppFile);

		QString newTempCpp;

		newTempCpp = TempCppText.readAll();
		newTempCpp.replace(QString("@Author@"),_inputAuthorName->text());
		newTempCpp.replace(QString("@pluginName@"),_inputName->text());



		QFile templatedCppFile(outDir.absoluteFilePath(QString("%1.cpp")
			.arg(_inputName->text())));

		if (!templatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out2(&templatedCppFile);
		out2 << newTempCpp << "\n\n\n";


		QFile templatedPrototypHxxFile(":/templates/res/TempHxx.hxx");
		if (!templatedPrototypHxxFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream TempHxxText(&templatedPrototypHxxFile);

		QString newTempHxx;

		newTempHxx = TempHxxText.readAll();
		newTempHxx.replace(QString("@Author@"),_inputAuthorName->text());
		newTempHxx.replace(QString("@pluginName@"),_inputName->text());

		if(!QString(_inputParaName->text()).isEmpty()){

		newTempHxx.replace("@Parameter@",QString("this->_addParameter (@ParaName@, \"@ParaName@\", \"@Documentation@\", \"@Default@\");"));
		newTempHxx.replace("@ParaName@",_inputParaName->text());
		newTempHxx.replace("@Documentation@",_inputParaDocumentation->toPlainText());
		newTempHxx.replace("@Default@",_inputParaDefault->text());
		}
		else
			newTempHxx.replace("@Parameter@","");

		if(!QString(_inputIOName1->text()).isEmpty()){

		if(_combo1->currentIndex() == 1){
		newTempHxx.replace("@In/Out@",QString("this->_addOutputSlot(@IOName@, \"@IOName@\", \"@Documentation@\", \"CImgList<@Typ@>\");"));}
		else
		newTempHxx.replace("@In/Out@",QString("this->_addInputSlot(@IOName@, \"@IOName@\", \"@Documentation@\", \"CImgList<@Typ@>\");"));

		newTempHxx.replace(QString("@Documentation@"),_inputIODocumentation->toPlainText());
		newTempHxx.replace(QString("@Typ@"),_inputIOTyp->text());
		newTempHxx.replace(QString("@IOName@"),_inputIOName1->text());
		}
		else
			newTempHxx.replace("@In/Out@","");


		QFile templatedHxxFile(outDir.absoluteFilePath(QString("%1.hxx")
				.arg(_inputName->text())));
		if (!templatedHxxFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out3(&templatedHxxFile);
		out3  << newTempHxx << "\n\n\n";

		/*if (_check1->isChecked())
			out3 << "This is a templated plugin! \n\n\n\n\n";
		if (_check2->isChecked())
			out3 << "addParameter Parameter1 \n";
		if (_check3->isChecked())
			out3 << "addParameter Parameter2 \n";
		if (_check4->isChecked())
			out3 << "addParameter Parameter3 \n";
		if (_check5->isChecked())
			out3 << "addInputSlot Input1 \n";
		if (_check6->isChecked())
			out3 << "addInputSlot Input2 \n";
		if (_check6->isChecked())
		out3 << "addOutputSlot Output \n";*/
	}
	else{
		QFile nonTemplatedPrototypHeaderFile(":/templates/res/NonTempH.h");
		if (!nonTemplatedPrototypHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream nonTempHText(&nonTemplatedPrototypHeaderFile);

		QString newNonTempHeader;

		newNonTempHeader = nonTempHText.readAll();
		newNonTempHeader.replace(QString("@Author@"),_inputAuthorName->text());
		newNonTempHeader.replace(QString("@pluginName@"),_inputName->text());

		if(!QString(_inputParaName->text()).isEmpty()){

		newNonTempHeader.replace("@Parameter@",QString("Parameter<@Typ@>   @ParaName@"));
		newNonTempHeader.replace("@addParameter@",QString("_addParameter (@ParaName@, \"@ParaName@\", \"@Documentation@\", \"@Default@\");"));
		newNonTempHeader.replace("@ParaName@",_inputParaName->text());
		newNonTempHeader.replace("@Documentation@",_inputParaDocumentation->toPlainText());
		newNonTempHeader.replace("@Default@",_inputParaDefault->text());
		newNonTempHeader.replace("@Typ@",_inputParaTyp->text());
		}
		else{
			newNonTempHeader.replace("@Parameter@","");
			newNonTempHeader.replace("@addParameter@","");
			}

		if(!QString(_inputIOName1->text()).isEmpty()){

		if(_combo1->currentIndex() == 1){
		newNonTempHeader.replace("@In/Out@",QString("OutputSlot<@Typ@>   @IOName@;"));
		newNonTempHeader.replace("@addSlot@","_addOutputSlot (@IOName@,  \"@IOName@\",  \"@Documentation@\"");
		}
		else{
		newNonTempHeader.replace("@In/Out@",QString("InputSlot<@Typ@>   @IOName@;"));
		newNonTempHeader.replace("@addSlot@","_addInputSlot (@IOName@,  \"@IOName@\",  \"@Documentation@\"");
		}
		newNonTempHeader.replace(QString("@Documentation@"),_inputIODocumentation->toPlainText());
		newNonTempHeader.replace(QString("@Typ@"),_inputIOTyp->text());
		newNonTempHeader.replace(QString("@IOName@"),_inputIOName1->text());
		}
		else{
			newNonTempHeader.replace("@In/Out@","");
			newNonTempHeader.replace("@addSlot@","");
			}


		QFile nontemplatedHeaderFile(outDir.absoluteFilePath(QString("%1.h")
			.arg(_inputName->text())));

		if (!nontemplatedHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

		QTextStream out4(&nontemplatedHeaderFile);
		out4 << newNonTempHeader << "\n\n\n";


		QFile nonTemplatedPrototypCppFile(":/templates/res/NonTempCpp.cpp");
		if (!nonTemplatedPrototypCppFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream nonTempCppText(&nonTemplatedPrototypCppFile);

		QString newNonTempCpp;

		newNonTempCpp = nonTempCppText.readAll();
		newNonTempCpp.replace(QString("@Author@"),_inputAuthorName->text());
		newNonTempCpp.replace(QString("@pluginName@"),_inputName->text());


		QFile nontemplatedCppFile(outDir.absoluteFilePath(QString("%1.cpp")
				.arg(_inputName->text())));
		if (!nontemplatedCppFile.open(QIODevice::WriteOnly | QIODevice::Text))
			return;


		QTextStream out5(&nontemplatedCppFile);
		out5 << newNonTempCpp << "\n\n\n";
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

void MainWindow::closeEvent(QCloseEvent* event)
{
	_writeSettings();
	QMainWindow::closeEvent(event);
}

#include "MainWindow.moc"
