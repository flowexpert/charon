#include "WizardPageSlots.h"
#include "ui_WizardPageSlots.h"

WizardPageSlots::WizardPageSlots(QWidget* p) :
		QWizardPage(p),
		_ui(new Ui::WizardPageSlots) {
	_ui->setupUi(this);
}

WizardPageSlots::~WizardPageSlots() {
	delete _ui;
}

void WizardPageSlots::initializePage() {
	QWizardPage::initializePage();

	_addInputSlot();
	_addInputSlot();
	_addOutputSlot();
	_addOutputSlot();
}

void WizardPageSlots::cleanupPage() {
	while(_ui->tableInputSlots->rowCount() > 0) {
		_delInputSlot();
	}
	while(_ui->tableOutputSlots->rowCount() > 0) {
		_delOutputSlot();
	}
	QWizardPage::cleanupPage();
}

void WizardPageSlots::_addInputSlot(
		QString name,QString doc,QString type, bool optional,bool multi) {
	int cRow = _ui->tableInputSlots->rowCount();
	int cCol = 0;
	_ui->tableInputSlots->insertRow(cRow);
	_ui->tableInputSlots->setItem(cRow,cCol++,new QTableWidgetItem(name));
	_ui->tableInputSlots->setItem(cRow,cCol++,new QTableWidgetItem(doc));
	_ui->tableInputSlots->setItem(cRow,cCol++,new QTableWidgetItem(type));

	_ui->tableInputSlots->setItem(cRow,cCol,new QTableWidgetItem());
	_ui->tableInputSlots->item(cRow,cCol)->setFlags(
			Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	_ui->tableInputSlots->item(cRow,cCol++)->setCheckState(
			optional ? Qt::Checked : Qt::Unchecked);

	_ui->tableInputSlots->setItem(cRow,cCol,new QTableWidgetItem());
	_ui->tableInputSlots->item(cRow,cCol)->setFlags(
			Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	_ui->tableInputSlots->item(cRow,cCol++)->setCheckState(
			multi ? Qt::Checked : Qt::Unchecked);
}

void WizardPageSlots::_addOutputSlot(
		QString name,QString doc,QString type) {
	int cRow = _ui->tableOutputSlots->rowCount();
	int cCol = 0;
	_ui->tableOutputSlots->insertRow(cRow);
	_ui->tableOutputSlots->setItem(cRow,cCol++,new QTableWidgetItem(name));
	_ui->tableOutputSlots->setItem(cRow,cCol++,new QTableWidgetItem(doc));
	_ui->tableOutputSlots->setItem(cRow,cCol++,new QTableWidgetItem(type));
}

void WizardPageSlots::_delRow(QTableWidget* table) {
	int curRow = table->currentRow();
	if(curRow < 0)
		curRow = table->rowCount()-1;
	if(curRow >= 0)
		table->removeRow(curRow);
}

void WizardPageSlots::_delInputSlot() {
	_delRow(_ui->tableInputSlots);
}

void WizardPageSlots::_delOutputSlot() {
	_delRow(_ui->tableOutputSlots);
}

