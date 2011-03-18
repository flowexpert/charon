#ifndef WIZARDPAGESLOTS_H
#define WIZARDPAGESLOTS_H

#include <QWizardPage>

namespace Ui {
	class WizardPageSlots;
}
class QTableWidget;

/// class providing validators and initialization for wizard page
class WizardPageSlots : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageSlots(QWidget *parent = 0);
	virtual ~WizardPageSlots();
	/// initialize page
	virtual void initializePage();
	/// page cleanup (if <em>back</em> is pressed)
	virtual void cleanupPage();

private slots:
	/// \name add and remove table rows
	// \{
	void _addInputSlot(
			QString name="",QString doc="",QString type="",
			bool optional=false, bool multi=false);
	void _addOutputSlot(
			QString name="",QString doc="",QString type="");
	void _delInputSlot();
	void _delOutputSlot();
	// \}

private:
	/// designer GUI
	Ui::WizardPageSlots* _ui;
	/// delete selected row from table widget
	/** if no row is selected, delete last row
	 *  \param table     table widget to use
	 */
	void _delRow(QTableWidget* table);
};

#endif // WIZARDPAGESLOTS_H
