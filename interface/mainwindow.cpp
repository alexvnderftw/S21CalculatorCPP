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
  foreach (QDateEdit *line, findChildren<QDateEdit *>()) {
    line->setAttribute(Qt::WA_MacShowFocusRect, 0);
  }
#endif
  ui->pushButtonMenu->setStyleSheet(
      "QPushButton {image: url(:/icons/menu_yellow.svg);} QPushButton:hover "
      "{image: url(:/icons/menu_yellow.svg);} QPushButton:pressed {image: "
      "url(:/icons/menu_black.svg);}");
  setDefaultSizes();
  connectSignals();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::connectSignals() {
  QSignalMapper *signalMapper = new QSignalMapper(this);
  connect(ui->pushButtonMenu, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonCalculator, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  connect(ui->pushButtonCredit, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonDeposit, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToCalculator, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  connect(ui->pushButtonToCredit, SIGNAL(clicked()), signalMapper, SLOT(map()));
  connect(ui->pushButtonToDeposit, SIGNAL(clicked()), signalMapper,
          SLOT(map()));
  signalMapper->setMapping(ui->pushButtonMenu,
                           ui->pushButtonMenu->objectName());
  signalMapper->setMapping(ui->pushButtonCalculator,
                           ui->pushButtonCalculator->objectName());
  signalMapper->setMapping(ui->pushButtonCredit,
                           ui->pushButtonCredit->objectName());
  signalMapper->setMapping(ui->pushButtonDeposit,
                           ui->pushButtonDeposit->objectName());
  signalMapper->setMapping(ui->pushButtonToCalculator,
                           ui->pushButtonToCalculator->objectName());
  signalMapper->setMapping(ui->pushButtonToCredit,
                           ui->pushButtonToCredit->objectName());
  signalMapper->setMapping(ui->pushButtonToDeposit,
                           ui->pushButtonToDeposit->objectName());
  connect(signalMapper, SIGNAL(mappedString(QString)), this,
          SLOT(switchPage(QString)));
  connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(quit()));
}

void MainWindow::switchPage(QString text) {
  if (text == "pushButtonToCalculator" || text == "pushButtonCalculator") {
    resetIcons();
    ui->pushButtonCalculator->setStyleSheet(
        "QPushButton {image: url(:/icons/calculate_yellow.svg);} "
        "QPushButton:hover {image: url(:/icons/calculate_yellow.svg);} "
        "QPushButton:pressed {image: url(:/icons/calculate_black.svg);}");
    setDefaultSizes();
    ui->pageCalculator->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageCalculator);
    ui->pageCalculator->setDefaultFocus();
    this->setMinimumSize(800, 520);
    this->resize(800, 520);
  } else if (text == "pushButtonToCredit" || text == "pushButtonCredit") {
    resetIcons();
    setDefaultSizes();
    ui->pushButtonCredit->setStyleSheet(
        "QPushButton {image: url(:/icons/payments_yellow.svg);} "
        "QPushButton:hover {image: url(:/icons/payments_yellow.svg);} "
        "QPushButton:pressed {image: url(:/icons/payments_black.svg);}");
    ui->pageCredit->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageCredit);
    ui->pageCredit->setDefaultFocus();
    this->setMinimumSize(700, 450);
    this->resize(700, 550);
  } else if (text == "pushButtonToDeposit" || text == "pushButtonDeposit") {
    resetIcons();
    ui->pushButtonDeposit->setStyleSheet(
        "QPushButton {image: url(:/icons/savings_yellow.svg);} "
        "QPushButton:hover {image: url(:/icons/savings_yellow.svg);} "
        "QPushButton:pressed {image: url(:/icons/savings_black.svg);}");
    setDefaultSizes();
    ui->pageDeposit->setDefaultSizes();
    ui->stackedWidget->setCurrentWidget(ui->pageDeposit);
    ui->pageDeposit->setDefaultFocus();
    this->setMinimumSize(800, 700);
    this->resize(800, 700);
  } else if (text == "pushButtonMenu") {
    resetIcons();
    ui->pushButtonMenu->setStyleSheet(
        "QPushButton {image: url(:/icons/menu_yellow.svg);} QPushButton:hover "
        "{image: url(:/icons/menu_yellow.svg);} QPushButton:pressed {image: "
        "url(:/icons/menu_black.svg);}");
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

void MainWindow::resetIcons() {
  ui->pushButtonMenu->setStyleSheet(
      "QPushButton {image: url(:/icons/menu_black.svg);} QPushButton:hover "
      "{image: url(:/icons/menu_yellow.svg);} QPushButton:pressed {image: "
      "url(:/icons/menu_black.svg);}");
  ui->pushButtonCalculator->setStyleSheet(
      "QPushButton {image: url(:/icons/calculate_black.svg);} "
      "QPushButton:hover {image: url(:/icons/calculate_yellow.svg);} "
      "QPushButton:pressed {image: url(:/icons/calculate_black.svg);}");
  ui->pushButtonCredit->setStyleSheet(
      "QPushButton {image: url(:/icons/payments_black.svg);} QPushButton:hover "
      "{image: url(:/icons/payments_yellow.svg);} QPushButton:pressed {image: "
      "url(:/icons/payments_black.svg);}");
  ui->pushButtonDeposit->setStyleSheet(
      "QPushButton {image: url(:/icons/savings_black.svg);} QPushButton:hover "
      "{image: url(:/icons/savings_yellow.svg);} QPushButton:pressed {image: "
      "url(:/icons/savings_black.svg);}");
}

void MainWindow::quit() { QApplication::quit(); }
