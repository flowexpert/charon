#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

/// template generator wizard widget
class Wizard : public QWizard
{
	Q_OBJECT

public:
	/// standart constructor
	explicit Wizard(QWidget *parent = 0);
	virtual ~Wizard();

protected:
	/// generate module
	virtual void done(int result);
};

#endif // WIZARD_H
