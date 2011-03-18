#include "WizardPageParameters.h"
#include "ui_WizardPageParameters.h"

WizardPageParameters::WizardPageParameters(QWidget* p) :
			QWizardPage(p),
			_ui(new Ui::WizardPageParameters) {
	_ui->setupUi(this);
}

WizardPageParameters::~WizardPageParameters() {
	delete _ui;
}

void WizardPageParameters::initializePage() {
	QWizardPage::initializePage();

	_addParameter();
	_addParameter();
}

void WizardPageParameters::cleanupPage() {
	while(_ui->tableParameters->rowCount() > 0) {
		_delParameter();
	}
	QWizardPage::cleanupPage();
}

void WizardPageParameters::_addParameter(
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

void WizardPageParameters::_delRow(QTableWidget* table) {
	int curRow = table->currentRow();
	if(curRow < 0)
		curRow = table->rowCount()-1;
	if(curRow >= 0)
		table->removeRow(curRow);
}

void WizardPageParameters::_delParameter() {
	_delRow(_ui->tableParameters);
}

