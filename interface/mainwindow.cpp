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
#endif

  QSignalMapper *signalMapper = new QSignalMapper(this);
  connect(ui->pushButtonMenu, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToCalculator, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  connect(ui->pushButtonToCredit, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToDeposit, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  signalMapper->setMapping(ui->pushButtonMenu, ui->pageMenu);
  signalMapper->setMapping(ui->pushButtonToCalculator, ui->pageCalculator);
  signalMapper->setMapping(ui->pushButtonToCredit, ui->pageCredit);
  signalMapper->setMapping(ui->pushButtonToDeposit, ui->pageDeposit);
  connect(signalMapper, SIGNAL(mapped(QWidget *)), this,
          SLOT(switchPage(QWidget *)));
  connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(quit()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::switchPage(QWidget *page) {
  ui->stackedWidget->setCurrentWidget(page);
  if (page == ui->pageCalculator) {
    ui->pageCalculator->setDefaultFocus();
  }
}

void MainWindow::quit() { QApplication::quit(); }
