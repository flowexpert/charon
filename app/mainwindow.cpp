#include "mainwindow.h"

#include <QFile>
#include <QtGui>
#include <MyLabel.h>
#include <MyTabWidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QWidget* centralWidget = new QWidget;
	setCentralWidget(centralWidget);
        centralWidget->setMinimumSize(800,610);

        QGridLayout* layoutmain = new QGridLayout(centralWidget);
        centralWidget-> setLayout(layoutmain);
        MyTabWidget *tabWidget = new MyTabWidget(centralWidget);
        layoutmain->addWidget(tabWidget,1,1);

        QWidget* page1 = new QWidget;
        tabWidget->addTab(page1, tr("tab1"));
        QWidget* page2 = new QWidget;
        tabWidget->addTab(page2, tr("tab2"));
        QWidget* page3 = new QWidget;
        tabWidget->addTab(page3, tr("tab3"));
        QWidget* page4 = new QWidget;
        tabWidget->addTab(page4, tr("tab4"));
        QWidget* page5 = new QWidget;
        tabWidget->addTab(page5, tr("tab5"));


        QVBoxLayout* background1 = new QVBoxLayout(page1);
        QWidget* page1hi = new QWidget(page1);
        QWidget* page1lo = new QWidget(page1);
        background1->addWidget(page1hi,6);
        background1->addWidget(page1lo,1);
        QGridLayout* layout = new QGridLayout(page1hi);
        page1->setLayout(background1);
        page1hi->setLayout(layout);
        QHBoxLayout* buttonlayout1 = new QHBoxLayout(page1lo);
        page1lo->setLayout(buttonlayout1);
	QPushButton* helloButton = new QPushButton(tr("hello world"));
        QPushButton* createButton = new QPushButton(tr("&Create"));
        QPushButton* exitButton = new QPushButton(tr("&Exit"));
	MyLabel* label = new MyLabel;
        QSpacerItem* spacer1 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QPushButton* nextButton1 = new QPushButton (tr("&Weiter"));
        QPushButton* browse = new QPushButton (tr("Browse"));
        QLabel* name = new QLabel (tr("Name of plug in:"));
        QLabel* save = new QLabel (tr("Saved in:"));

        inputDir = new QLineEdit(page1);
        inputName = new QLineEdit (page1);
        inputDir -> setText(tr("D:/"));
        inputName -> setText(tr("plugin1"));

        label->setText(tr("Please choose a name and a directory to save"));
        layout->addWidget(label,1,1,1,3,Qt::AlignCenter);
        layout->addWidget(name,3,1,1,2);
        layout->addWidget(save,4,1,1,2);
        layout->addWidget(inputName,3,3);
        layout->addWidget(inputDir,4,3);
        layout->addWidget(browse,4,4);
        buttonlayout1->addItem(spacer1);
        buttonlayout1->addWidget(nextButton1);
        layout->setColumnStretch(1,1);
        layout->setColumnStretch(6,1);
        layout->setRowStretch(2,1);
        layout->setRowStretch(5,1);

        double a;
        a = (inputName-> text()).toDouble();


        connect(helloButton, SIGNAL(clicked()), this, SLOT(_showHello()));
        connect(nextButton1, SIGNAL(clicked()), tabWidget, SLOT(nextPage()));


        QVBoxLayout* background2 = new QVBoxLayout(page2);
        QWidget* page2hi = new QWidget(page2);
        QWidget* page2lo = new QWidget(page2);
        background2->addWidget(page2hi,6);
        background2->addWidget(page2lo,1);
        QGridLayout* layout2 = new QGridLayout(page2hi);
        page2->setLayout(background2);
        page2hi->setLayout(layout2);
        QHBoxLayout* buttonlayout2 = new QHBoxLayout(page2lo);
        page2lo->setLayout(buttonlayout2);
        QSpacerItem* spacer2 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QPushButton* nextButton2 = new QPushButton (tr("&Weiter"));
        check1 = new QCheckBox(tr("templated Plugin"));
        check2 = new QCheckBox(tr("Parameter1"));
        check3 = new QCheckBox(tr("Parameter2"));
        check4 = new QCheckBox(tr("Parameter3"));
        check5 = new QCheckBox(tr("Input1"));
        check6 = new QCheckBox(tr("Input2"));
        check7 = new QCheckBox(tr("Output"));
        layout2->addWidget(check1,2,1);
        layout2->setRowStretch(10,2);
        layout2->addWidget(check3,5,1);
        layout2->setRowStretch(1,1);
        layout2->setRowStretch(3,1);
        layout2->addWidget(check2,4,1);
        layout2->addWidget(check4,6,1);
        layout2->addWidget(check5,7,1);
        layout2->addWidget(check6,8,1);
        layout2->addWidget(check7,9,1);
        buttonlayout2->addItem(spacer2);
        buttonlayout2->addWidget(createButton);
        buttonlayout2->addWidget(nextButton2);


        connect(nextButton2,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));

        QVBoxLayout* background3 = new QVBoxLayout(page3);
        QWidget* page3hi = new QWidget(page3);
        QWidget* page3lo = new QWidget(page3);
        background3->addWidget(page3hi,6);
        background3->addWidget(page3lo,1);
        QGridLayout* layout3 = new QGridLayout(page3hi);
        page3->setLayout(background3);
        page3hi->setLayout(layout3);
        QHBoxLayout* buttonlayout3 = new QHBoxLayout(page3lo);
        page3lo->setLayout(buttonlayout3);
        QSpacerItem* spacer3 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QPushButton* nextButton3 = new QPushButton (tr("&Weiter"));
        buttonlayout3->addItem(spacer3);
        buttonlayout3->addWidget(exitButton);
        buttonlayout3->addWidget(nextButton3);
        layout3->setColumnStretch(5,1);

        connect(nextButton3,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));
        connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

        QVBoxLayout* background4 = new QVBoxLayout(page4);
        QWidget* page4hi = new QWidget(page4);
        QWidget* page4lo = new QWidget(page4);
        background4->addWidget(page4hi,6);
        background4->addWidget(page4lo,1);
        QGridLayout* layout4 = new QGridLayout(page4hi);
        page4->setLayout(background4);
        page4hi->setLayout(layout4);
        QHBoxLayout* buttonlayout4 = new QHBoxLayout(page4lo);
        page4lo->setLayout(buttonlayout4);
        QSpacerItem* spacer4 = new QSpacerItem(40,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QPushButton* nextButton4 = new QPushButton (tr("&Weiter"));
        buttonlayout4->addItem(spacer4);
        buttonlayout4->addWidget(nextButton4);
        layout4->setColumnStretch(5,1);

        connect(nextButton4,SIGNAL(clicked()), tabWidget, SLOT(nextPage()));


        QGridLayout* layout5 = new QGridLayout(page5);
        page5->setLayout(layout5);
        layout5->setColumnStretch(5,1);
        layout5->addWidget(helloButton,2,2);
        QSizePolicy policy(helloButton->sizePolicy());
        policy.setVerticalPolicy(QSizePolicy::Expanding);
        helloButton->setSizePolicy(policy);



        connect(createButton, SIGNAL(clicked()), this, SLOT(save()));


}





MainWindow::~MainWindow() {
}

void MainWindow::_showHello() {
	QMessageBox::information(this, tr("hello world box"),
		tr("Welcome to this hello world application!"));
}

void MainWindow::save() {

    QString a;
        a = (inputName -> text());
    QString b;
        b = (inputDir -> text());
    QString c = a;
    c.prepend(b);
    c.append(".h");

    QFile file1(c);
        if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
         return;


        QTextStream out(&file1);
        out << "The magic number is: " << 66 << "\n\n\n";
        if (check1->isChecked())
            out << "This is a templated plugin! \n\n\n\n\n";
        if (check2->isChecked())
            out << "Parameter1 \n";
        if (check3->isChecked())
            out << "Parameter2 \n";
        if (check4->isChecked())
            out << "Parameter3 \n";
        if (check5->isChecked())
            out << "Input1 \n";
        if (check6->isChecked())
            out << "Input2 \n";
        if (check6->isChecked())
            out << "Output \n";


    if (check1->isChecked())
        {c.append("xx");
      QFile file2(c);

      if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

        QTextStream out2(&file2);
        out2 << "The magic number is: " << 66 << "\n\n\n";
    }

    QString d = a;
    d.prepend(b);
    d.append(".cpp");


    QFile file3(d);
        if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
         return;


        QTextStream out3(&file3);
        out3 << "The magic number is: " << 66 << "\n\n\n";

        if (check1->isChecked())
            out3 << "This is a templated plugin! \n\n\n\n\n";
        if (check2->isChecked())
            out3 << "addParameter Parameter1 \n";
        if (check3->isChecked())
            out3 << "addParameter Parameter2 \n";
        if (check4->isChecked())
            out3 << "addParameter Parameter3 \n";
        if (check5->isChecked())
            out3 << "addInputSlot Input1 \n";
        if (check6->isChecked())
            out3 << "addInputSlot Input2 \n";
        if (check6->isChecked())
            out3 << "addOutputSlot Output \n";


    }

#include "mainwindow.moc"
