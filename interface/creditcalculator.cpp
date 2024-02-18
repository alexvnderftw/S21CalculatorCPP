#include "creditcalculator.h"

#include "ui_creditcalculator.h"

CreditCalculator::CreditCalculator(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditCalculator) {
  ui->setupUi(this);
  setUiParameters();
  setLimits();
  setData();
  connectSignals();
}

CreditCalculator::~CreditCalculator() { delete ui; }

void CreditCalculator::setDefaultFocus() {
  ui->pushButtonFillCredit->setFocus();
}

void CreditCalculator::setDefaultSizes() {
  ui->widgetMain->setMinimumHeight(160);
}

void CreditCalculator::nullDefaultSizes() {
  ui->widgetMain->setMinimumHeight(0);
}

void CreditCalculator::setUiParameters() {
  ui->tableWidgetCredit->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidgetCredit->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->tableWidgetCredit->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
  ui->tableWidgetCredit->setSelectionMode(QAbstractItemView::ExtendedSelection);

  ui->tableWidgetCredit->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 5px;}");
  ui->tableWidgetCredit->verticalHeader()->setStyleSheet(
      "QHeaderView::section:last {border-bottom-left-radius: 5px;}");
}

void CreditCalculator::connectSignals() {
  connect(ui->pushButtonFillCredit, SIGNAL(clicked()), this, SLOT(calculate()));
  connect(ui->comboBoxTimeType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(limitTime(int)));
  connect(ui->comboBoxCreditType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setType(int)));
  //    connect(ui->spinBoxCredit, SIGNAL(returnPressed()), this,
  //            SLOT(calculate()));
  //    connect(ui->spinBoxTime, SIGNAL(returnPressed()), this,
  //            SLOT(calculate()));
  //    connect(ui->doubleSpinBoxRate, SIGNAL(returnPressed()), this,
  //            SLOT(calculate()));
}

void CreditCalculator::calculate() {
  setData();
  clearContent();
  if (ctrl.calculate() == true) {
    fillTable();
    fillLines();
    ui->tableWidgetCredit->horizontalHeader()->setStyleSheet(
        "QHeaderView::section:first {border-top-left-radius: 0px;}");
  }
}

void CreditCalculator::fillTable() {
  ui->tableWidgetCredit->setRowCount(ctrl.getDataSize() + 1);
  for (size_t i = 0; i < ctrl.getDataSize(); ++i) {
    ui->tableWidgetCredit->setItem(
        i, 0,
        new QTableWidgetItem(getDateString(ctrl.getMonth(i), ctrl.getYear(i))));
    ui->tableWidgetCredit->setItem(
        i, 1,
        new QTableWidgetItem(QString::number(ctrl.getPayment(i), 'f', 2)));
    ui->tableWidgetCredit->setItem(
        i, 2,
        new QTableWidgetItem(QString::number(ctrl.getMainPart(i), 'f', 2)));
    ui->tableWidgetCredit->setItem(
        i, 3,
        new QTableWidgetItem(QString::number(ctrl.getRatePart(i), 'f', 2)));
    ui->tableWidgetCredit->setItem(
        i, 4,
        new QTableWidgetItem(QString::number(ctrl.getLeftover(i), 'f', 2)));
  }
  ui->tableWidgetCredit->setItem(ctrl.getDataSize(), 0,
                                 new QTableWidgetItem("Total:"));
  ui->tableWidgetCredit->setItem(
      ctrl.getDataSize(), 1,
      new QTableWidgetItem(QString::number(ctrl.getSumPaid(), 'f', 2) +
                           "\nTotal paid"));
  ui->tableWidgetCredit->setItem(
      ctrl.getDataSize(), 2,
      new QTableWidgetItem(QString::number(ctrl.getSumMainPart(), 'f', 2) +
                           "\nDebt paid"));
  ui->tableWidgetCredit->setItem(
      ctrl.getDataSize(), 3,
      new QTableWidgetItem(QString::number(ctrl.getSumRatePart(), 'f', 2) +
                           "\nInterest paid"));
  // ui->tableWidgetCredit->resizeColumnsToContents();
  //  background-color: rgb(243, 89, 97);
  // color: rgb(2, 1, 18);
  //  ui->tableWidgetCredit->item(ctrl.getDataSize(),
  //  0)->setBackground(QColor(243, 89, 97));
  //  ui->tableWidgetCredit->item(ctrl.getDataSize(),
  //  0)->setBackground(QColor(2, 1, 18));
  ui->tableWidgetCredit->resizeRowsToContents();
}

void CreditCalculator::fillLines() {
  if (ctrl.isAnnuity() == true)
    ui->lineEditPayment->setText(QString::number(ctrl.getPayment(0), 'f', 2));
  else if (ctrl.isDifferential() == true) {
    ui->lineEditPayment->setText(QString::number(ctrl.getPayment(0), 'f', 2));
    if (ctrl.getDataSize() > 1)
      ui->lineEditPayment->insert(
          "-" +
          QString::number(ctrl.getPayment(ctrl.getDataSize() - 1), 'f', 2));
  }
  ui->lineEditInterest->setText(QString::number(ctrl.getSumRatePart(), 'f', 2));
  ui->lineEditTotal->setText(
      QString::number(ctrl.getSumRatePart() + ctrl.getSumMainPart(), 'f', 2));
}

void CreditCalculator::clearContent() {
  ui->tableWidgetCredit->clearContents();
  ui->lineEditPayment->clear();
  ui->lineEditInterest->clear();
  ui->lineEditTotal->clear();
}

void CreditCalculator::setLimits() {
  ui->comboBoxCreditType->setCurrentIndex(0);
  ui->spinBoxCredit->setMinimum(MIN_CREDIT);
  ui->spinBoxCredit->setMaximum(MAX_CREDIT);
  ui->doubleSpinBoxRate->setMinimum(MIN_RATE);
  ui->doubleSpinBoxRate->setMaximum(MAX_RATE);
  ui->spinBoxTime->setMinimum(MIN_TIME);
  limitTime(ui->comboBoxCreditType->currentIndex());
}

void CreditCalculator::setData() {
  ctrl.setData(ui->spinBoxCredit->value(),
               ui->doubleSpinBoxRate->value() / 100.0, getTime());
  setType(ui->comboBoxCreditType->currentIndex());
}

int CreditCalculator::getTime() {
  if (ui->comboBoxTimeType->currentIndex() == 0)
    return ui->spinBoxTime->value() * 12;
  else
    return ui->spinBoxTime->value();
}

void CreditCalculator::setType(int index) {
  if (index == 0)
    ctrl.setAnnuity();
  else if (index == 1)
    ctrl.setDifferential();
}

/* Slots */

void CreditCalculator::limitTime(int index) {
  // if (index == 0 && ui->spinBoxTime->value() > MAX_TIME_Y)
  // ui->spinBoxTime->setValue(MAX_TIME_Y);
  if (index == 0)
    ui->spinBoxTime->setMaximum(MAX_TIME_Y);
  else if (index == 1)
    ui->spinBoxTime->setMaximum(MAX_TIME_M);
}

/* Misc */

QString CreditCalculator::getDateString(int month, int year) {
  return MONTHS[month - 1] + " " + QString::number(year);
}
