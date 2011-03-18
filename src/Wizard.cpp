#include "Wizard.h"
#include "ui_Wizard.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include "WizardPageStart.h"
#include "WizardPageMetadata.h"
#include "WizardPageSummary.h"

Wizard::Wizard(QWidget* p) :
		QWizard(p),
		_ui(new Ui::Wizard) {
	addPage(new WizardPageStart(this));
	addPage(new WizardPageMetadata(this));
	_ui->setupUi(this);
	addPage(new WizardPageSummary(this));

	_addInputSlot();
	_addInputSlot();
	_addOutputSlot();
	_addOutputSlot();
	_addParameter();
	_addParameter();
}

Wizard::~Wizard() {
	delete _ui;
}

void Wizard::_addInputSlot(
		QString name,QString doc,QString type, bool optional,bool multi) {
	int curRow = _ui->tableInputSlots->rowCount();
	_ui->tableInputSlots->insertRow(curRow);
	_ui->tableInputSlots->setItem(curRow,0u,new QTableWidgetItem(name));
	_ui->tableInputSlots->setItem(curRow,1u,new QTableWidgetItem(doc));
	_ui->tableInputSlots->setItem(curRow,2u,new QTableWidgetItem(type));
	_ui->tableInputSlots->setItem(curRow,3u,new QTableWidgetItem());
	_ui->tableInputSlots->item(curRow,3u)->setFlags(
			Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	_ui->tableInputSlots->item(curRow,3u)->setCheckState(
			optional ? Qt::Checked : Qt::Unchecked);
	_ui->tableInputSlots->setItem(curRow,4u,new QTableWidgetItem());
	_ui->tableInputSlots->item(curRow,4u)->setFlags(
			Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	_ui->tableInputSlots->item(curRow,4u)->setCheckState(
			multi ? Qt::Checked : Qt::Unchecked);
}

void Wizard::_addOutputSlot(
		QString name,QString doc,QString type) {
	int curRow = _ui->tableOutputSlots->rowCount();
	_ui->tableOutputSlots->insertRow(curRow);
	_ui->tableOutputSlots->setItem(curRow,0u,new QTableWidgetItem(name));
	_ui->tableOutputSlots->setItem(curRow,1u,new QTableWidgetItem(doc));
	_ui->tableOutputSlots->setItem(curRow,2u,new QTableWidgetItem(type));
}

void Wizard::_addParameter(
		QString name,QString doc,QString type, QString defVal, bool list) {
	int curRow = _ui->tableParameters->rowCount();
	_ui->tableParameters->insertRow(curRow);
	_ui->tableParameters->setItem(curRow,0u,new QTableWidgetItem(name));
	_ui->tableParameters->setItem(curRow,1u,new QTableWidgetItem(doc));
	_ui->tableParameters->setItem(curRow,2u,new QTableWidgetItem(type));
	_ui->tableParameters->setItem(curRow,3u,new QTableWidgetItem(type));
	_ui->tableParameters->setItem(curRow,4u,new QTableWidgetItem(defVal));
	_ui->tableParameters->item(curRow,4u)->setFlags(
			Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	_ui->tableParameters->item(curRow,4u)->setCheckState(
			list ? Qt::Checked : Qt::Unchecked);
}

void Wizard::_delRow(QTableWidget* table) {
	int curRow = table->currentRow();
	if(curRow < 0)
		curRow = table->rowCount()-1;
	if(curRow >= 0)
		table->removeRow(curRow);
}

void Wizard::_delInputSlot() {
	_delRow(_ui->tableInputSlots);
}

void Wizard::_delOutputSlot() {
	_delRow(_ui->tableOutputSlots);
}

void Wizard::_delParameter() {
	_delRow(_ui->tableParameters);
}

void Wizard::done(int res) {
	// save settings
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");
	if(field("loadExisting").toBool()) {
		settings.setValue("recentInput", field("loadPath"));
	}
	settings.setValue("author", field("author"));
	settings.setValue("email", field("email"));

	settings.beginGroup("Paths");
	settings.setValue("headerSeparate", field("headerSeparate"));
	settings.setValue("commonOut", field("commonOut"));
	settings.setValue("headerOut", field("headerOut"));
	settings.setValue("sourceOut", field("sourceOut"));
	settings.endGroup();

	QWizard::done(res);
}
