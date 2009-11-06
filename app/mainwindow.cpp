#include "mainwindow.h"

#include <QtGui>
#include <MyLabel.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QWidget* centralWidget = new QWidget;
	setCentralWidget(centralWidget);
	QGridLayout* layout = new QGridLayout(centralWidget);
	centralWidget->setLayout(layout);
	QPushButton* helloButton = new QPushButton(tr("hello world"));
	QPushButton* exitButton = new QPushButton(tr("&exit"));
	MyLabel* label = new MyLabel;
	label->setText(tr("something to display at the top"));
	layout->addWidget(label,1,1,1,4,Qt::AlignCenter);
	layout->addWidget(helloButton,2,2);
	layout->addWidget(exitButton,2,3);
	layout->setColumnStretch(1,1);
	layout->setColumnStretch(4,1);
	QSizePolicy policy(helloButton->sizePolicy());
	policy.setVerticalPolicy(QSizePolicy::Expanding);
	helloButton->setSizePolicy(policy);
	connect(helloButton, SIGNAL(clicked()), this, SLOT(_showHello()));
	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
}

MainWindow::~MainWindow() {
}

void MainWindow::_showHello() {
	QMessageBox::information(this, tr("hello world box"),
		tr("Welcome to this hello world application!"));
}

#include "mainwindow.moc"
