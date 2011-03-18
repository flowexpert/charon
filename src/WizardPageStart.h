#ifndef WIZARDPAGESTART_H
#define WIZARDPAGESTART_H

#include <QWizardPage>

/// class providing validators for start page
class WizardPageStart : public QWizardPage
{
	Q_OBJECT

public:
	/// standart constructor
	explicit WizardPageStart(QWidget *parent = 0);
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
};

#endif // WIZARDPAGESTART_H
