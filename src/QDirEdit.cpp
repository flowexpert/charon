#include "QDirEdit.h"
#include <QCompleter>
#include <QDirModel>
#include <QFileDialog>

QDirEdit::QDirEdit(QWidget *parent) :
    QLineEdit(parent)
{
	QCompleter* completer = new QCompleter(this);
	completer->setModel(new QDirModel(QStringList(), QDir::AllDirs,
			QDir::Name, completer));
	setCompleter(completer);
}

void QDirEdit::fileDialog() {
	QString newVal = QFileDialog::getExistingDirectory(
			this, "select directory",text());
	if(!newVal.isEmpty())
	setText(newVal);
}

#include "QDirEdit.moc"
