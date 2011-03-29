#include "WizardPageSlots.h"
#include "ui_WizardPageSlots.h"
#include <QMessageBox>

WizardPageSlots::WizardPageSlots(QWidget* p) :
		QWizardPage(p),
		_ui(new Ui::WizardPageSlots) {
	_ui->setupUi(this);
	_ui->tableInputSlots->setModel(&_inputSlots);
	_ui->tableInputSlots->hideColumn(5);
	_ui->tableOutputSlots->setModel(&_outputSlots);
	_ui->tableOutputSlots->hideColumn(3);
	_ui->tableOutputSlots->hideColumn(4);
	_ui->tableOutputSlots->hideColumn(5);

	registerField("inputSlotNames", this, "inputSlotNames",
			SIGNAL(completeChanged()));
	registerField("inputSlotDocs", this, "inputSlotDocs",
			SIGNAL(completeChanged()));
	registerField("inputSlotTypes", this, "inputSlotTypes",
			SIGNAL(completeChanged()));
	registerField("inputSlotOptional", this, "inputSlotOptional",
			SIGNAL(completeChanged()));
	registerField("inputSlotMulti", this, "inputSlotMulti",
			SIGNAL(completeChanged()));
	registerField("outputSlotNames", this, "outputSlotNames",
			SIGNAL(completeChanged()));
	registerField("outputSlotDocs", this, "outputSlotDocs",
			SIGNAL(completeChanged()));
	registerField("outputSlotTypes", this, "outputSlotTypes",
			SIGNAL(completeChanged()));

	connect(&_inputSlots, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
	connect(&_outputSlots, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
}

WizardPageSlots::~WizardPageSlots() {
	delete _ui;
}

bool WizardPageSlots::isComplete() const {
	return QWizardPage::isComplete()
			&& (_inputSlots.names.indexOf("") < 0)
			&& (_inputSlots.docs.indexOf("") < 0)
			&& (_outputSlots.names.indexOf("") < 0)
			&& (_outputSlots.docs.indexOf("") < 0);
}

bool WizardPageSlots::validatePage() {
	// check for name collisions
	for(int ii=0; ii < _inputSlots.names.size(); ii++) {
		if(_outputSlots.names.indexOf(_inputSlots.names[ii]) >= 0) {
			QMessageBox::warning(
					this,tr("name collision"),
					tr(
							"It is not possible to use <b>%1</b> as input "
							"<em>and</em> output slot.<br>"
							"Please change the name of one of them.")
					.arg(_inputSlots.names[ii]));
			return false;
		}
	}
	return QWizardPage::validatePage();
}

void WizardPageSlots::_addInputSlot() {
	int nr = _inputSlots.rowCount();
	_inputSlots.insertRow(nr);
	emit completeChanged();
}

void WizardPageSlots::_addOutputSlot() {
	int nr = _outputSlots.rowCount();
	_outputSlots.insertRow(nr);
	emit completeChanged();
}

void WizardPageSlots::_delInputSlot() {
	QModelIndex cur = _ui->tableInputSlots->currentIndex();
	_inputSlots.removeRow(
			cur.isValid() ? cur.row() : (_inputSlots.rowCount()-1));
	emit completeChanged();
}

void WizardPageSlots::_delOutputSlot() {
	QModelIndex cur = _ui->tableOutputSlots->currentIndex();
	_outputSlots.removeRow(
			cur.isValid() ? cur.row() : (_outputSlots.rowCount()-1));
	emit completeChanged();
}

QStringList WizardPageSlots::inputSlotNames() const {
	return _inputSlots.names;
}

QStringList WizardPageSlots::inputSlotDocs() const {
	return _inputSlots.docs;
}

QStringList WizardPageSlots::inputSlotTypes() const {
	return _inputSlots.types;
}

QStringList WizardPageSlots::inputSlotOptional() const {
	QStringList res;
	for (int ii=0; ii< _inputSlots.optional.size(); ii++) {
		res.append(QVariant(_inputSlots.optional[ii]).toString());
	}
	return res;
}

QStringList WizardPageSlots::inputSlotMulti() const {
	QStringList res;
	for (int ii=0; ii< _inputSlots.multi.size(); ii++) {
		res.append(QVariant(_inputSlots.multi[ii]).toString());
	}
	return res;
}

QStringList WizardPageSlots::outputSlotNames() const {
	return _outputSlots.names;
}

QStringList WizardPageSlots::outputSlotDocs() const {
	return _outputSlots.docs;
}

QStringList WizardPageSlots::outputSlotTypes() const {
	return _outputSlots.types;
}
