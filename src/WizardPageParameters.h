#ifndef WIZARDPAGEPARAMETERS_H
#define WIZARDPAGEPARAMETERS_H

#include <QWizardPage>
class QTableWidget;

namespace Ui {
	class WizardPageParameters;
}

/// class providing validators and initialization for wizard page
class WizardPageParameters : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageParameters(QWidget *parent = 0);
	~WizardPageParameters();
	/// initialize page
	virtual void initializePage();
	/// page cleanup (if <em>back</em> is pressed)
	virtual void cleanupPage();

private slots:
	/// \name add and remove table rows
	// \{
	void _addParameter(
			QString name="",QString doc="",QString type="",
			QString defVal="", bool list=false);
	void _delParameter();
	// \}

private:
	/// designer GUI
	Ui::WizardPageParameters* _ui;
	/// delete selected row from table widget
	/** if no row is selected, delete last row
	 *  \param table     table widget to use
	 */
	void _delRow(QTableWidget* table);
};

#endif // WIZARDPAGEPARAMETERS_H
