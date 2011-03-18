#ifndef WIZARDPAGESTART_H
#define WIZARDPAGESTART_H

#include <QWizardPage>

namespace Ui {
	class WizardPageStart;
}

/// class providing validators for start page
class WizardPageStart : public QWizardPage
{
	Q_OBJECT

public:
	/// standart constructor
	explicit WizardPageStart(QWidget *parent = 0);
	virtual ~WizardPageStart();
	/// return complete state
	virtual bool isComplete() const;
	/// initialize page
	virtual void initializePage();

public slots:
	/// check if page is complete
	void checkComplete();

private:
	/// complete flag
	bool _complete;
	/// designer GUI
	Ui::WizardPageStart* _ui;
};

#endif // WIZARDPAGESTART_H
