#ifndef CHARONRUN_H
#define CHARONRUN_H

#include <QObject>

class CharonRun : public QObject
{
	Q_OBJECT
public:
	explicit CharonRun(QObject* parent = 0);

signals:

public slots:
	/// update plugin information
	void updatePlugins() const;

};

#endif // CHARONRUN_H
