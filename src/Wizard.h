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

private:
	/// write files using resource templates
	bool _writeFiles();
	/// replace patterns using the field values
	/** \param src     source filename
	 *  \param dst     target filename
	 *  \retval true   successful write
	 */
	bool _replacePlaceholders(QString src, QString dst);
};

#endif // WIZARD_H
