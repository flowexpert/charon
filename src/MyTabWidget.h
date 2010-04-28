#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>

/// Tab widget with additional slots for switching to previous/next tab.
class MyTabWidget : public QTabWidget {
	Q_OBJECT

public:
	/// constructor setting parent widget
	/** \param parent    parent widget
	 */
	MyTabWidget(QWidget* parent = 0);

public slots:
	/// switch tab to next page
	void nextPage();
	/// switch tab to previous page
	void previousPage();
};




#endif // MYTABWIDGET_H
