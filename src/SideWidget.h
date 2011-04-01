#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <QListWidget>

/// side widget with progress display
class SideWidget : public QListWidget
{
	Q_OBJECT

public:
	/// default constructor
	explicit SideWidget(QWidget* parent = 0);

public slots:
	/// update progress status
	void updateProgress(int id);
};

#endif // SIDEWIDGET_H
