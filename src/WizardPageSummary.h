#ifndef WIZARDPAGESUMMARY_H
#define WIZARDPAGESUMMARY_H

#include <QWizardPage>

namespace Ui {
	class WizardPageSummary;
}

/// class providing validators and initialization for wizard page
class WizardPageSummary : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageSummary(QWidget* parent = 0);
	virtual ~WizardPageSummary();
	/// initialize page
	virtual void initializePage();

private slots:
	/// update output file summary
	void _updateOut();

private:
	/// designer GUI
	Ui::WizardPageSummary* _ui;
};

#endif // WIZARDPAGESUMMARY_H
