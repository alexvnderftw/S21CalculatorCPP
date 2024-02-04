#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonToNext, SIGNAL(clicked()), this, SLOT(pushButtonToNext_clicked()));
    connect(ui->pushButtonToPrevious, SIGNAL(clicked()), this, SLOT(pushButtonToPrevious_clicked()));
    //connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(on_stackedWidget_currentChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButtonToPrevious_clicked()
{
    if (ui->stackedWidget->currentWidget() == ui->pageCalculator) {
        ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
        ui->pushButtonToPrevious->setText("Credit");
        ui->pushButtonToNext->setText("Calculator");
    } else if (ui->stackedWidget->currentWidget() == ui->pageCredit) {
        ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
        ui->pushButtonToPrevious->setText("Deposit");
        ui->pushButtonToNext->setText("Credit");
    } else if (ui->stackedWidget->currentWidget() == ui->pageDeposit) {
        ui->stackedWidget->setCurrentWidget(ui->pageCredit);
        ui->pushButtonToPrevious->setText("Calculator");
        ui->pushButtonToNext->setText("Deposit");
    }
}

void MainWindow::pushButtonToNext_clicked()
{
    if (ui->stackedWidget->currentWidget() == ui->pageCalculator) {
        ui->stackedWidget->setCurrentWidget(ui->pageCredit);
        ui->pushButtonToPrevious->setText("Calculator");
        ui->pushButtonToNext->setText("Deposit");
    } else if (ui->stackedWidget->currentWidget() == ui->pageCredit) {
        ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
        ui->pushButtonToPrevious->setText("Credit");
        ui->pushButtonToNext->setText("Calculator");
    } else if (ui->stackedWidget->currentWidget() == ui->pageDeposit) {
        ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
        ui->pushButtonToPrevious->setText("Deposit");
        ui->pushButtonToNext->setText("Credit");
    }
}
