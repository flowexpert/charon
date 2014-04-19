#ifndef STATUSEVENTFILTER_H
#define STATUSEVENTFILTER_H

#include <QObject>

/// filter status tip events to display them on the side widget
class StatusEventFilter : public QObject
{
	Q_OBJECT
public:
	/// default constructor
	explicit StatusEventFilter(QObject* parent = 0);

signals:
	/// recieved status message
	void statusMessage(QString msg);

protected:
	/// event filter to handle status tip events
	virtual bool eventFilter(QObject* obj, QEvent* event);
};

#endif // STATUSEVENTFILTER_H
