#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <QWidget>

namespace Ui {
	class SideWidget;
}

/// side widget with progress display
class SideWidget : public QWidget
{
	Q_OBJECT

public:
	/// default constructor
	explicit SideWidget(QWidget *parent = 0);
	~SideWidget();

public slots:
	/// update progress status
	void updateProgress(int id);

private:
	/// designer GUI
	Ui::SideWidget* _ui;
};

#endif // SIDEWIDGET_H
