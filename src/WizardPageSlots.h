#ifndef WIZARDPAGESLOTS_H
#define WIZARDPAGESLOTS_H

#include <QWizardPage>
#include <QAbstractTableModel>
#include "ParamSlotModel.h"

namespace Ui {
	class WizardPageSlots;
}
class QTableWidget;

/// class providing validators and initialization for wizard page
class WizardPageSlots : public QWizardPage
{
	Q_OBJECT
	/// \name properties for result fields
	// \{
	Q_PROPERTY(QStringList inputSlotNames    READ inputSlotNames)
	Q_PROPERTY(QStringList inputSlotDocs     READ inputSlotDocs)
	Q_PROPERTY(QStringList inputSlotTypes    READ inputSlotTypes)
	Q_PROPERTY(QStringList inputSlotOptional READ inputSlotOptional)
	Q_PROPERTY(QStringList inputSlotMulti    READ inputSlotMulti)
	Q_PROPERTY(QStringList outputSlotNames   READ outputSlotNames)
	Q_PROPERTY(QStringList outputSlotDocs    READ outputSlotDocs)
	Q_PROPERTY(QStringList outputSlotTypes   READ outputSlotTypes)
	// \}

public:
	/// default constructor
	explicit WizardPageSlots(QWidget *parent = 0);
	virtual ~WizardPageSlots();
	/// return complete state
	virtual bool isComplete() const;
	/// check for name collisions
	virtual bool validatePage();

	/// \name property getters
	// \{
	QStringList inputSlotNames() const;
	QStringList inputSlotDocs() const;
	QStringList inputSlotTypes() const;
	QStringList inputSlotOptional() const;
	QStringList inputSlotMulti() const;
	QStringList outputSlotNames() const;
	QStringList outputSlotDocs() const;
	QStringList outputSlotTypes() const;
	// \}

private slots:
	/// \name add and remove table rows
	// \{
	void _addInputSlot();
	void _addOutputSlot();
	void _delInputSlot();
	void _delOutputSlot();
	// \}

private:
	/// designer GUI
	Ui::WizardPageSlots* _ui;
	/// input slots
	ParamSlotModel _inputSlots;
	/// output slots
	ParamSlotModel _outputSlots;
};

#endif // WIZARDPAGESLOTS_H
