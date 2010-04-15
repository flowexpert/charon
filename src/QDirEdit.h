#ifndef QDIREDIT_H
#define QDIREDIT_H

#include <QLineEdit>

class QDirEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit QDirEdit(QWidget* parent = 0);

public slots:
	void fileDialog();
};

#endif // QDIREDIT_H
