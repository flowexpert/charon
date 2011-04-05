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

	/// warp long lines adding specified delimiters
	/** \param src     source string
	 *  \param delim   line delimiter to be used to wrap
	 *  \param ll      desired line lenght
	 *  \param nl      replacement for newlines in src (prepended to delim)
	 *  \returns       modified string
	 */
	static QString breakLines(
		QString src, QString delim = "\n", QString nl = "", uint ll=80);
protected:
	/// generate module
	virtual void done(int result);

private:
	/// write files using resource templates
	bool _writeFiles();
	/// update existing CMake file adding the new module
	void _updateCMakeFile();

	/// replace patterns using the field values
	/** \param src     source filename
	 *  \param dst     target filename
	 *  \retval true   successful write
	 */
	bool _replacePlaceholders(QString src, QString dst);

	/// type lookup in cppTypeMap
	QString _cppTypeLookup(QString type, QString prefix);
};

#endif // WIZARD_H
