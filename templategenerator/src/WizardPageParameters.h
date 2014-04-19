#ifndef WIZARDPAGEPARAMETERS_H
#define WIZARDPAGEPARAMETERS_H

#include <QWizardPage>
#include <QAbstractTableModel>
#include "ParamSlotModel.h"
class QTableWidget;

namespace Ui {
	class WizardPageParameters;
}

/// class providing validators and initialization for wizard page
class WizardPageParameters : public QWizardPage
{
	Q_OBJECT
	/// \name properties for result fields
	// \{
	Q_PROPERTY(QStringList paramNames    READ paramNames)
	Q_PROPERTY(QStringList paramDocs     READ paramDocs)
	Q_PROPERTY(QStringList paramTypes    READ paramTypes)
	Q_PROPERTY(QStringList paramDefaults READ paramDefaults)
	Q_PROPERTY(QStringList paramLists    READ paramLists)
	// \}

public:
	/// default constructor
	explicit WizardPageParameters(QWidget *parent = 0);
	~WizardPageParameters();
	/// return complete state
	virtual bool isComplete() const;
	/// check for name collisions
	virtual bool validatePage();

	/// \name property getters
	// \{
	QStringList paramNames() const;
	QStringList paramDocs() const;
	QStringList paramTypes() const;
	QStringList paramDefaults() const;
	QStringList paramLists() const;
	// \}

private slots:
	/// \name add and remove table rows
	// \{
	void _addParameter();
	void _delParameter();
	// \}

private:
	/// designer GUI
	Ui::WizardPageParameters* _ui;
	/// parameter table model
	ParamSlotModel _params;
};

#endif // WIZARDPAGEPARAMETERS_H
