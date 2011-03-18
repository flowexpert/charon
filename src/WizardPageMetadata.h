#ifndef WIZARDPAGEMETADATA_H
#define WIZARDPAGEMETADATA_H

#include <QWizardPage>

namespace Ui {
	class WizardPageMetadata;
}

/// class providing validators and initialization for wizard page
class WizardPageMetadata : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageMetadata(QWidget* parent = 0);
	/// initialize page
	virtual void initializePage();

private:
	/// designer GUI
	Ui::WizardPageMetadata* _ui;
};

#endif // WIZARDPAGEMETADATA_H
