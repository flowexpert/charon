#include "Wizard.h"
#include "ui_Wizard.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>

Wizard::Wizard(QWidget* p) :
		QWizard(p),
		_ui(new Ui::Wizard) {
	_ui->setupUi(this);
}

Wizard::~Wizard() {
	delete _ui;
}

void Wizard::initializePage(int id) {
	QWizard::initializePage(id);

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");

	switch(id) {
	case 1:
		// metadata page
		_ui->editAuthor->setText(
				settings.value("author", tr("Unknown")).toString());
		_ui->editEmail->setText(
				settings.value("email", "").toString());
		break;

	case 2:
		// slot page
		_addInputSlot();
		_addInputSlot();
		_addOutputSlot();
		_addOutputSlot();
		break;

	case 3:
		// parameter page
		_addParameter();
		_addParameter();
		break;

	case 4:
		// summary page
		_ui->checkHeaderSep->setChecked(Qt::Checked);
		_ui->checkHeaderSep->setChecked(Qt::Unchecked);

		settings.beginGroup("Paths");
		_ui->checkHeaderSep->setChecked(
				settings.value("headerSeparate", false).toBool() ?
				Qt::Checked : Qt::Unchecked);
		_ui->editOutPath->setText(
				settings.value("commonOut", QDir::homePath()).toString());
		_ui->editHeaderOut->setText(
				settings.value("headerOut", QDir::homePath()).toString());
		_ui->editSourceOut->setText(
				settings.value("sourceOut", QDir::homePath()).toString());
		settings.endGroup();
		break;

	default:
		break;
	}
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
		_ui->tableInputSlots->removeRow(curRow);
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
	if(_ui->selLoadMod->isChecked()) {
		settings.setValue("recentInput", _ui->editLoadMod->text());
	}
	settings.setValue("author", _ui->editAuthor->text());
	settings.setValue("email", _ui->editEmail->text());

	settings.beginGroup("Paths");
	settings.setValue("headerSeparate", _ui->checkHeaderSep->isChecked());
	settings.setValue("commonOut", _ui->editOutPath->text());
	settings.setValue("headerOut",_ui->editHeaderOut->text());
	settings.setValue("sourceOut", _ui->editSourceOut->text());
	settings.endGroup();

	QWizard::done(res);
}
