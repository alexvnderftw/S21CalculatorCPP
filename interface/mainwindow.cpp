#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
#ifdef __APPLE__
  foreach (QLineEdit *line, findChildren<QLineEdit *>()) {
    line->setAttribute(Qt::WA_MacShowFocusRect, 0);
  }
  foreach (QDoubleSpinBox *box, findChildren<QDoubleSpinBox *>()) {
    box->setAttribute(Qt::WA_MacShowFocusRect, 0);
  }
  foreach (QSpinBox *box, findChildren<QSpinBox *>()) {
    box->setAttribute(Qt::WA_MacShowFocusRect, 0);
  }
#endif
  setDefaultSizes();
  connectSignals();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::connectSignals() {
  QSignalMapper *signalMapper = new QSignalMapper(this);
  connect(ui->pushButtonMenu, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToCalculator, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  connect(ui->pushButtonToCredit, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToDeposit, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  signalMapper->setMapping(ui->pushButtonMenu, ui->pushButtonMenu->text());
  signalMapper->setMapping(ui->pushButtonToCalculator,
                           ui->pushButtonToCalculator->text());
  signalMapper->setMapping(ui->pushButtonToCredit,
                           ui->pushButtonToCredit->text());
  signalMapper->setMapping(ui->pushButtonToDeposit,
                           ui->pushButtonToDeposit->text());
  connect(signalMapper, SIGNAL(mappedString(QString)), this,
          SLOT(switchPage(QString)));
  connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(quit()));
}

void MainWindow::switchPage(QString text) {
  if (text == "Calculator") {
    setDefaultSizes();
    ui->pageCalculator->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
    ui->pageCalculator->setDefaultFocus();
    this->setMinimumSize(800, 520);
    this->resize(800, 520);
  } else if (text == "Credit") {
    setDefaultSizes();
    ui->pageCredit->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageCredit);
    ui->pageCredit->setDefaultFocus();
    this->setMinimumSize(700, 450);
    this->resize(700, 550);
  } else if (text == "Deposit") {
    setDefaultSizes();
    ui->pageDeposit->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
    ui->pageDeposit->setDefaultFocus();
    this->setMinimumSize(800, 500);
    this->resize(800, 650);
  } else if (text == "") {
    setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageMenu);
    ui->pushButtonToCalculator->setFocus();
    this->setMinimumSize(400, 426);
    this->resize(400, 426);
  }
}

void MainWindow::setDefaultSizes() {
  ui->pageCalculator->nullDefaultSizes();
  ui->pageCredit->nullDefaultSizes();
  ui->pageDeposit->nullDefaultSizes();
}

void MainWindow::changeMenuIcon() {
  ui->pushButtonMenu->setIcon(QIcon("/icons/menu_yellow.svg"));
}

void MainWindow::quit() { QApplication::quit(); }
