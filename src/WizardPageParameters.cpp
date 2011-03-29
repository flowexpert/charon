#include "WizardPageParameters.h"
#include "ui_WizardPageParameters.h"
#include <QMessageBox>

WizardPageParameters::WizardPageParameters(QWidget* p) :
			QWizardPage(p),
			_ui(new Ui::WizardPageParameters) {
	_ui->setupUi(this);
	_ui->tableParameters->setModel(&_params);
	_ui->tableParameters->hideColumn(3);
	_ui->tableParameters->hideColumn(4);

	registerField("paramNames", this, "paramNames",
			SIGNAL(completeChanged()));
	registerField("paramDocs", this, "paramDocs",
			SIGNAL(completeChanged()));
	registerField("paramTypes", this, "paramTypes",
			SIGNAL(completeChanged()));
	registerField("paramLists", this, "paramLists",
			SIGNAL(completeChanged()));

	connect(&_params, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
}

bool WizardPageParameters::isComplete() const {
	return QWizardPage::isComplete()
			&& (_params.names.indexOf("") < 0)
			&& (_params.docs.indexOf("") < 0);
}

bool WizardPageParameters::validatePage() {
	// check for name collisions
	QStringList s;
	s.append(field("inputSlotNames").toStringList());
	s.append(field("outputSlotNames").toStringList());
	for(int ii=0; ii < _params.names.size(); ii++) {
		if(s.indexOf(_params.names[ii]) >= 0) {
			QMessageBox::warning(
					this,tr("name collision"),
					tr(
							"It is not possible to use <b>%1</b> as slot "
							"<em>and</em> as parameter.<br>"
							"Please change the name of one of them.")
					.arg(_params.names[ii]));
			return false;
		}
	}
	return QWizardPage::validatePage();
}

WizardPageParameters::~WizardPageParameters() {
	delete _ui;
}

void WizardPageParameters::_addParameter() {
	_params.insertRow(_params.rowCount());
	emit completeChanged();
}

void WizardPageParameters::_delParameter() {
	QModelIndex cur = _ui->tableParameters->currentIndex();
	_params.removeRow(cur.isValid() ? cur.row() : (_params.rowCount()-1));
	emit completeChanged();
}

QStringList WizardPageParameters::paramNames() const {
	return _params.names;
}

QStringList WizardPageParameters::paramDocs() const {
	return _params.docs;
}

QStringList WizardPageParameters::paramTypes() const {
	return _params.types;
}

QStringList WizardPageParameters::paramLists() const {
	QStringList res;
	for (int ii=0; ii< _params.list.size(); ii++) {
		res.append(QVariant(_params.list[ii]).toString());
	}
	return res;
}
