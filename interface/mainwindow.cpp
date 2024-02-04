#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    //connect(ui->pushButtonToCalculator, SIGNAL(clicked()), this, [this]{switchPage(ui->pageCalculator); });
    //connect(ui->pushButtonToPrevious, SIGNAL(clicked()), this, SLOT(pushButtonToPrevious_clicked()));
    connect(ui->pushButtonMenu, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonToCalculator, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonToCredit, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonToDeposit, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->pushButtonMenu, ui->pageMenu);
    signalMapper->setMapping(ui->pushButtonToCalculator, ui->pageCalculator);
    signalMapper->setMapping(ui->pushButtonToCredit, ui->pageCredit);
    signalMapper->setMapping(ui->pushButtonToDeposit, ui->pageDeposit);
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(switchPage(QWidget*)));
    connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchPage(QWidget *page) {
    ui->stackedWidget->setCurrentWidget(page);
    if (page == ui->pageCalculator) {
        ui->pageCalculator->setDefaultFocus();
        //ui->pageCalculator->setDefaultButton();
    }
}

void MainWindow::quit() {
    QApplication::quit();
}

//void MainWindow::pushButtonToPrevious_clicked()
//{
//    if (ui->stackedWidget->currentWidget() == ui->pageCalculator) {
//        ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
//        ui->pushButtonToPrevious->setText("Credit");
//        ui->pushButtonToNext->setText("Calculator");
//    } else if (ui->stackedWidget->currentWidget() == ui->pageCredit) {
//        ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
//        ui->pushButtonToPrevious->setText("Deposit");
//        ui->pushButtonToNext->setText("Credit");
//    } else if (ui->stackedWidget->currentWidget() == ui->pageDeposit) {
//        ui->stackedWidget->setCurrentWidget(ui->pageCredit);
//        ui->pushButtonToPrevious->setText("Calculator");
//        ui->pushButtonToNext->setText("Deposit");
//    }
//}

//void MainWindow::pushButtonToNext_clicked()
//{
//    if (ui->stackedWidget->currentWidget() == ui->pageCalculator) {
//        ui->stackedWidget->setCurrentWidget(ui->pageCredit);
//        ui->pushButtonToPrevious->setText("Calculator");
//        ui->pushButtonToNext->setText("Deposit");
//    } else if (ui->stackedWidget->currentWidget() == ui->pageCredit) {
//        ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
//        ui->pushButtonToPrevious->setText("Credit");
//        ui->pushButtonToNext->setText("Calculator");
//    } else if (ui->stackedWidget->currentWidget() == ui->pageDeposit) {
//        ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
//        ui->pushButtonToPrevious->setText("Deposit");
//        ui->pushButtonToNext->setText("Credit");
//    }
//}

