#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLineEdit *inputName;
    QLineEdit *inputDir;
    QCheckBox *check1;
    QCheckBox *check2;
    QCheckBox *check3;
    QCheckBox *check4;
    QCheckBox *check5;
    QCheckBox *check6;
    QCheckBox *check7;


private slots:
	void _showHello();
        void save();
};

#endif // MAINWINDOW_H
