#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <QWidget>
class QListWidget;
class QStatusBar;

/// side widget with progress display
class SideWidget : public QWidget
{
	Q_OBJECT

public:
	/// default constructor
	explicit SideWidget(QWidget* parent = 0);
	/// add entry to side widget
	void addItem(QString text);

public slots:
	/// update progress status
	void updateProgress(int id);
	/// show message
	void showMessage(QString msg);

private:
	/// progress table with page ids
	QListWidget* _list;
	/// StatusBar widget
	QStatusBar* _status;
};

#endif // SIDEWIDGET_H
