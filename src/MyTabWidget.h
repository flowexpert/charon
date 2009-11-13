#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>



class MyTabWidget : public QTabWidget

{
        Q_OBJECT

public:
    MyTabWidget(QWidget* parent = 0);

public slots:
    void nextPage();


};




#endif // MYTABWIDGET_H
