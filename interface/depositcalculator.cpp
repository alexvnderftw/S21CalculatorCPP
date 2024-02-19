#include "depositcalculator.h"

#include "ui_depositcalculator.h"

DepositCalculator::DepositCalculator(QWidget *parent)
    : QWidget(parent), ui(new Ui::DepositCalculator) {
  ui->setupUi(this);
  setUiParameters();
  setInitialState();
  connectSignals();
  hideOperationWidget();
}

DepositCalculator::~DepositCalculator() { delete ui; }

void DepositCalculator::setDefaultFocus() {
  ui->pushButtonCalculate->setFocus();
}

void DepositCalculator::setDefaultSizes() {
  ui->widgetMain->setMinimumWidth(360);
  ui->widgetMain->setMinimumHeight(220);
  // this->setMinimumHeight(700);
}

void DepositCalculator::nullDefaultSizes() {
  ui->widgetMain->setMinimumWidth(0);
  ui->widgetMain->setMinimumHeight(0);
}

void DepositCalculator::setUiParameters() {
  ui->tableWidgetEvents->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidgetEvents->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->tableWidgetReplenishes->setEditTriggers(
      QAbstractItemView::NoEditTriggers);
  ui->tableWidgetReplenishes->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->tableWidgetWithdrawals->setEditTriggers(
      QAbstractItemView::NoEditTriggers);
  ui->tableWidgetWithdrawals->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->tableWidgetTax->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidgetTax->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->tableWidgetEvents->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
  ui->tableWidgetReplenishes->verticalHeader()->setDefaultAlignment(
      Qt::AlignCenter);
  ui->tableWidgetWithdrawals->verticalHeader()->setDefaultAlignment(
      Qt::AlignCenter);
  ui->tableWidgetTax->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

  ui->tableWidgetEvents->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 5px;}");
  ui->tableWidgetEvents->verticalHeader()->setStyleSheet(
      "QHeaderView::section:last {border-bottom-left-radius: 5px;}");
  ui->tableWidgetTax->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 5px;}");
  ui->tableWidgetTax->verticalHeader()->setStyleSheet(
      "QHeaderView::section:last {border-bottom-left-radius: 5px;}");
  ui->tableWidgetWithdrawals->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 5px;}");
  ui->tableWidgetWithdrawals->verticalHeader()->setStyleSheet(
      "QHeaderView::section:last {border-bottom-left-radius: 5px;}");
  ui->tableWidgetReplenishes->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 5px;}");
  ui->tableWidgetReplenishes->verticalHeader()->setStyleSheet(
      "QHeaderView::section:last {border-bottom-left-radius: 5px;}");
}

void DepositCalculator::connectSignals() {
  connect(ui->pushButtonCalculate, SIGNAL(clicked()), this, SLOT(calculate()));
  connect(ui->pushButtonAddReplenish, SIGNAL(clicked()), this,
          SLOT(addReplenish()));
  connect(ui->pushButtonRemoveReplenish, SIGNAL(clicked()), this,
          SLOT(removeReplenish()));
  connect(ui->pushButtonAddWithdrawal, SIGNAL(clicked()), this,
          SLOT(addWithdrawal()));
  connect(ui->pushButtonRemoveWithdrawal, SIGNAL(clicked()), this,
          SLOT(removeWithdrawal()));
  connect(ui->pushButtonHideOperations, SIGNAL(clicked()), this,
          SLOT(hideOperationWidget()));
  connect(ui->pushButtonShowOperations, SIGNAL(clicked()), this,
          SLOT(showOperationWidget()));
  connect(ui->checkBoxCapitalization, SIGNAL(stateChanged(int)), this,
          SLOT(setCapitalization(int)));
  connect(ui->comboBoxTermUnit, SIGNAL(currentTextChanged(QString)), this,
          SLOT(setTermLimit(QString)));
}

void DepositCalculator::hideOperationWidget() {
  ui->widgetHidder->hide();
  ui->widgetShower->show();
  // this->setMinimumHeight(700);
}

void DepositCalculator::showOperationWidget() {
  ui->widgetHidder->show();
  ui->widgetShower->hide();
  // this->setMinimumHeight(800);
}

void DepositCalculator::setTermLimit(QString text) {
  if (text == "years")
    ui->spinBoxTerm->setMaximum(MAX_TERM_Y);
  else if (text == "months")
    ui->spinBoxTerm->setMaximum(MAX_TERM_M);
  else
    ui->spinBoxTerm->setMaximum(MAX_TERM_D);
}

void DepositCalculator::setCurrentDate() {
  ui->dateEditStartDate->setDate(QDate::currentDate());
  ui->dateEditOperationDate->setDate(QDate::currentDate());
}

void DepositCalculator::setValues() {
  data.setDeposit(ui->doubleSpinBoxDeposit->value());
  data.setTerm(ui->spinBoxTerm->value(), getUnitType());
  data.setStartDate(ui->dateEditStartDate->date().day(),
                    ui->dateEditStartDate->date().month(),
                    ui->dateEditStartDate->date().year());
  data.setInterest(ui->doubleSpinBoxRate->value() / 100.0);
  data.setPeriodicity(getPayPeriod());
  data.setTax(ui->doubleSpinBoxTax->value() / 100.0);
  data.setRemainderLimit(ui->doubleSpinBoxRemainderLimit->value());
}

s21::Deposit::TermType DepositCalculator::getUnitType() {
  if (ui->comboBoxTermUnit->currentIndex() == 0)
    return s21::Deposit::T_DAY;
  else if (ui->comboBoxTermUnit->currentIndex() == 1)
    return s21::Deposit::T_MONTH;
  else
    return s21::Deposit::T_YEAR;
}

s21::Deposit::PayPeriod DepositCalculator::getPayPeriod() {
  if (ui->comboBoxPeriodicity->currentIndex() == 0)
    return s21::Deposit::P_AT_END;
  else if (ui->comboBoxPeriodicity->currentIndex() == 1)
    return s21::Deposit::P_DAILY;
  else if (ui->comboBoxPeriodicity->currentIndex() == 2)
    return s21::Deposit::P_WEEKLY;
  else if (ui->comboBoxPeriodicity->currentIndex() == 3)
    return s21::Deposit::P_MONTHLY;
  else if (ui->comboBoxPeriodicity->currentIndex() == 4)
    return s21::Deposit::P_QUARTERLY;
  else if (ui->comboBoxPeriodicity->currentIndex() == 5)
    return s21::Deposit::P_BIANNUALLY;
  else
    return s21::Deposit::P_ANNUALLY;
}

void DepositCalculator::calculate() {
  clearContent();
  setValues();
  if (data.calculate() == true) {
    fillTable();
    fillTaxes();
    fillSummary();
    ui->tableWidgetEvents->horizontalHeader()->setStyleSheet(
        "QHeaderView::section:first {border-top-left-radius: 0px;}");
    ui->tableWidgetTax->horizontalHeader()->setStyleSheet(
        "QHeaderView::section:first {border-top-left-radius: 0px;}");
  }
}

void DepositCalculator::fillTable() {
  ui->tableWidgetEvents->insertRow(ui->tableWidgetEvents->rowCount());
  for (size_t i = 0; i < data.getEventListSize(); ++i) {
    if (data.getEventListElement(i)->event() != s21::Deposit::E_NEWYEAR) {
      ui->tableWidgetEvents->setItem(ui->tableWidgetEvents->rowCount() - 1, 0,
                                     new QTableWidgetItem(getDateString(
                                         data.getEventListElement(i)->date())));
      if (data.getEventListElement(i)->event() != s21::Deposit::E_DECLINE) {
        if (data.getEventListElement(i)->event() == s21::Deposit::E_PAYDAY)
          ui->tableWidgetEvents->setItem(
              ui->tableWidgetEvents->rowCount() - 1, 1,
              new QTableWidgetItem(outputNumber(
                  data.getEventListElement(i)->gain(), false, 'f', 2)));
        if (data.getEventListElement(i)->event() != s21::Deposit::E_PAYDAY ||
            (data.getEventListElement(i)->event() == s21::Deposit::E_PAYDAY &&
             data.isCapitalization() == true))
          ui->tableWidgetEvents->setItem(
              ui->tableWidgetEvents->rowCount() - 1, 2,
              new QTableWidgetItem(
                  outputNumber(data.getEventListElement(i)->balance_change(),
                               true, 'f', 2)));
        if (data.getEventListElement(i)->event() == s21::Deposit::E_PAYDAY &&
            data.isCapitalization() == false)
          ui->tableWidgetEvents->setItem(
              ui->tableWidgetEvents->rowCount() - 1, 3,
              new QTableWidgetItem(outputNumber(
                  data.getEventListElement(i)->payment(), false, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            ui->tableWidgetEvents->rowCount() - 1, 4,
            new QTableWidgetItem(outputNumber(
                data.getEventListElement(i)->balance(), false, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            ui->tableWidgetEvents->rowCount() - 1, 5,
            new QTableWidgetItem(
                getEventString(data.getEventListElement(i)->event())));
      } else {
        ui->tableWidgetEvents->setItem(
            ui->tableWidgetEvents->rowCount() - 1, 1,
            new QTableWidgetItem(
                "Withdrawal (" +
                outputNumber(data.getEventListElement(i)->balance_change(),
                             false, 'f', 2) +
                ") was declined due to minimum balance limit (" +
                outputNumber(data.getRemainderLimit(), false, 'f', 2) + ")"));
        ui->tableWidgetEvents->setSpan(ui->tableWidgetEvents->rowCount() - 1, 1,
                                       1,
                                       5);
      }
      ui->tableWidgetEvents->insertRow(ui->tableWidgetEvents->rowCount());
    }
  }
  ui->tableWidgetEvents->setItem(ui->tableWidgetEvents->rowCount() - 1, 0,
                                 new QTableWidgetItem("Total:"));
  ui->tableWidgetEvents->setItem(ui->tableWidgetEvents->rowCount() - 1, 1,
                                 new QTableWidgetItem(outputNumber(
                                     data.getInterestTotal(), false, 'f', 2)));
  ui->tableWidgetEvents->setItem(
      ui->tableWidgetEvents->rowCount() - 1, 4,
      new QTableWidgetItem(outputNumber(data.getBalance(), false, 'f', 2)));
  ui->tableWidgetEvents->resizeRowsToContents();
}

void DepositCalculator::fillTaxes() {
  ui->tableWidgetTax->setRowCount(data.getTaxListSize());
  for (size_t i = 0; i < data.getTaxListSize(); ++i) {
    ui->tableWidgetTax->setItem(i, 0,
                                new QTableWidgetItem(QString::number(
                                    data.getTaxListElement(i)->year())));
    ui->tableWidgetTax->setItem(
        i, 1,
        new QTableWidgetItem(
            outputNumber(data.getTaxListElement(i)->income(), false, 'f', 2)));
    ui->tableWidgetTax->setItem(
        i, 2,
        new QTableWidgetItem(
            outputNumber(data.getTaxListElement(i)->tax(), false, 'f', 2)));
  }
  ui->tableWidgetTax->resizeRowsToContents();
}

void DepositCalculator::clearContent() {
  ui->tableWidgetEvents->clearContents();
  ui->tableWidgetTax->clearContents();
  ui->tableWidgetEvents->setRowCount(0);
  ui->tableWidgetTax->setRowCount(0);
  ui->lineEditAccruedInterest->clear();
  ui->lineEditTotal->clear();
  ui->lineEditGain->clear();
  ui->lineEditTotalTax->clear();
}

QString DepositCalculator::getDateString(s21::Date date) {
  QDate qdate(date.getYear(), date.getMonth(), date.getDay());
  return qdate.toString("dd.MM.yyyy");
}

QString DepositCalculator::getEventString(s21::Deposit::EventType event) {
  if (event == s21::Deposit::E_REPLENISH)
    return "Replenishment";
  else if (event == s21::Deposit::E_PAYDAY)
    return "Payment";
  else if (event == s21::Deposit::E_WITHDRAWAL)
    return "Withdrawal";
  else if (event == s21::Deposit::E_NEWYEAR)
    return "New year";
  else if (event == s21::Deposit::E_DECLINE)
    return "Withdrawal declined";
  else
    return "Error";
}

QString DepositCalculator::getOperPeriodString(
    s21::Deposit::OperPeriod period) {
  if (period == s21::Deposit::O_ONCE)
    return "One-time";
  else if (period == s21::Deposit::O_MONTHLY)
    return "Monthly";
  else if (period == s21::Deposit::O_BIMONTHLY)
    return "Every two months";
  else if (period == s21::Deposit::O_QUARTERLY)
    return "Quarterly";
  else if (period == s21::Deposit::O_BIANNUALLY)
    return "Twice a year";
  else if (period == s21::Deposit::O_ANNUALLY)
    return "Every year";
  else
    return "Error";
}

s21::Deposit::OperPeriod DepositCalculator::getOperPeriod() {
  if (ui->comboBoxOperationPeriodicity->currentIndex() == 0)
    return s21::Deposit::O_ONCE;
  else if (ui->comboBoxOperationPeriodicity->currentIndex() == 1)
    return s21::Deposit::O_MONTHLY;
  else if (ui->comboBoxOperationPeriodicity->currentIndex() == 2)
    return s21::Deposit::O_BIMONTHLY;
  else if (ui->comboBoxOperationPeriodicity->currentIndex() == 3)
    return s21::Deposit::O_QUARTERLY;
  else if (ui->comboBoxOperationPeriodicity->currentIndex() == 4)
    return s21::Deposit::O_BIANNUALLY;
  else
    return s21::Deposit::O_ANNUALLY;
}

/* Slots */

void DepositCalculator::addReplenish() {
  s21::Date date(ui->dateEditOperationDate->date().day(),
                 ui->dateEditOperationDate->date().month(),
                 ui->dateEditOperationDate->date().year());
  data.addReplenish(getOperPeriod(), date, ui->doubleSpinBoxValue->value());
  ui->tableWidgetReplenishes->insertRow(ui->tableWidgetReplenishes->rowCount());
  ui->tableWidgetReplenishes->setItem(
      ui->tableWidgetReplenishes->rowCount() - 1, 0,
      new QTableWidgetItem(getOperPeriodString(getOperPeriod())));
  ui->tableWidgetReplenishes->setItem(
      ui->tableWidgetReplenishes->rowCount() - 1, 1,
      new QTableWidgetItem(getDateString(date)));
  ui->tableWidgetReplenishes->setItem(
      ui->tableWidgetReplenishes->rowCount() - 1, 2,
      new QTableWidgetItem(
          outputNumber(ui->doubleSpinBoxValue->value(), false, 'f', 2)));
  ui->tableWidgetReplenishes->selectRow(ui->tableWidgetReplenishes->rowCount() -
                                        1);
  ui->tableWidgetReplenishes->resizeRowsToContents();
  ui->tableWidgetReplenishes->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 0px;}");
}

void DepositCalculator::removeReplenish() {
  if (data.getReplenishListSize() > 0) {
    int index = ui->tableWidgetReplenishes->currentRow();
    ui->tableWidgetReplenishes->removeRow(
        ui->tableWidgetReplenishes->currentRow());
    data.removeReplenish(index);
    if (index > 0)
      ui->tableWidgetReplenishes->selectRow(index - 1);
    else
      ui->tableWidgetReplenishes->selectRow(0);
  }
  if (ui->tableWidgetReplenishes->rowCount() == 0) {
    ui->tableWidgetReplenishes->horizontalHeader()->setStyleSheet(
        "QHeaderView::section:first {border-top-left-radius: 5px;}");
  }
}

void DepositCalculator::addWithdrawal() {
  s21::Date date(ui->dateEditOperationDate->date().day(),
                 ui->dateEditOperationDate->date().month(),
                 ui->dateEditOperationDate->date().year());
  data.addWithdrawal(getOperPeriod(), date, ui->doubleSpinBoxValue->value());
  ui->tableWidgetWithdrawals->insertRow(ui->tableWidgetWithdrawals->rowCount());
  ui->tableWidgetWithdrawals->setItem(
      ui->tableWidgetWithdrawals->rowCount() - 1, 0,
      new QTableWidgetItem(getOperPeriodString(getOperPeriod())));
  ui->tableWidgetWithdrawals->setItem(
      ui->tableWidgetWithdrawals->rowCount() - 1, 1,
      new QTableWidgetItem(getDateString(date)));
  ui->tableWidgetWithdrawals->setItem(
      ui->tableWidgetWithdrawals->rowCount() - 1, 2,
      new QTableWidgetItem(
          outputNumber(ui->doubleSpinBoxValue->value(), false, 'f', 2)));
  ui->tableWidgetWithdrawals->selectRow(ui->tableWidgetWithdrawals->rowCount() -
                                        1);
  ui->tableWidgetWithdrawals->resizeRowsToContents();
  ui->tableWidgetWithdrawals->horizontalHeader()->setStyleSheet(
      "QHeaderView::section:first {border-top-left-radius: 0px;}");
}

void DepositCalculator::removeWithdrawal() {
  if (data.getWithdrawalListSize() > 0) {
    int index = ui->tableWidgetWithdrawals->currentRow();
    ui->tableWidgetWithdrawals->removeRow(
        ui->tableWidgetWithdrawals->currentRow());
    data.removeWithdrawal(index);
    if (index > 0)
      ui->tableWidgetWithdrawals->selectRow(index - 1);
    else
      ui->tableWidgetWithdrawals->selectRow(0);
  }
  if (ui->tableWidgetWithdrawals->rowCount() == 0) {
    ui->tableWidgetWithdrawals->horizontalHeader()->setStyleSheet(
        "QHeaderView::section:first {border-top-left-radius: 5px;}");
  }
}

void DepositCalculator::setCapitalization(int value) {
  if (value == 0) {
    data.setCapitalization(false);
    qobject_cast<QListView *>(ui->comboBoxPeriodicity->view())
        ->setRowHidden(0, false);
  } else {
    data.setCapitalization(true);
    qobject_cast<QListView *>(ui->comboBoxPeriodicity->view())
        ->setRowHidden(0, true);
    if (ui->comboBoxPeriodicity->currentIndex() == 0) {
      ui->comboBoxPeriodicity->setCurrentIndex(1);
    }
  }
}

void DepositCalculator::fillSummary() {
  ui->lineEditAccruedInterest->setText(
      outputNumber(data.getInterestTotal(), false, 'f', 2));
  ui->lineEditTotal->setText(outputNumber(
      data.getInterestTotal() + data.getDeposit() + data.getReplenishTotal(),
      false, 'f', 2));
  ui->lineEditGain->setText(
      outputNumber(data.getInterestTotal() / data.getDeposit() * 100.0, false,
                   'f', 2) +
      " %");
  ui->lineEditTotalTax->setText(
      outputNumber(data.getTaxTotal(), false, 'f', 2));
}

void DepositCalculator::setInitialState() {
  setCurrentDate();
  ui->checkBoxCapitalization->setChecked(false);
  ui->comboBoxTermUnit->setCurrentIndex(INITIAL_TERM_UNIT);
  ui->spinBoxTerm->setValue(INITIAL_TERM);
  setTermLimit(ui->comboBoxTermUnit->currentText());
  ui->comboBoxPeriodicity->setCurrentIndex(INITIAL_PERIODICITY);
  ui->comboBoxOperationPeriodicity->setCurrentIndex(
      INITIAL_OPERATION_PERIODICITY);
  ui->doubleSpinBoxDeposit->setMinimum(MIN_DEPOSIT);
  ui->doubleSpinBoxDeposit->setMaximum(MAX_DEPOSIT);
  ui->doubleSpinBoxDeposit->setValue(INITIAL_DEPOSIT);
  ui->doubleSpinBoxRate->setMinimum(MIN_RATE);
  ui->doubleSpinBoxRate->setMaximum(MAX_RATE);
  ui->doubleSpinBoxRate->setValue(INITIAL_RATE);
  ui->doubleSpinBoxTax->setMinimum(MIN_TAX);
  ui->doubleSpinBoxTax->setMaximum(MAX_TAX);
  ui->doubleSpinBoxTax->setValue(INITIAL_TAX);
  ui->doubleSpinBoxRemainderLimit->setMinimum(MIN_REMAINDER_LIMIT);
  ui->doubleSpinBoxRemainderLimit->setMaximum(MAX_REMAINDER_LIMIT);
  ui->doubleSpinBoxRemainderLimit->setValue(INITIAL_REMAINDER_LIMIT);
  ui->doubleSpinBoxValue->setMinimum(MIN_OPERATION_VALUE);
  ui->doubleSpinBoxValue->setMaximum(MAX_OPERATION_VALUE);
  ui->doubleSpinBoxValue->setValue(INITIAL_OPERATION_VALUE);
  ui->dateEditStartDate->setMaximumDate(QDate(MAX_START_YEAR, 1, 1));
  ui->dateEditOperationDate->setMaximumDate(QDate(MAX_START_YEAR, 1, 1));
  ui->dateEditStartDate->setMinimumDate(QDate(MIN_START_YEAR, 1, 1));
  ui->dateEditOperationDate->setMinimumDate(QDate(MIN_START_YEAR, 1, 1));
}

QString DepositCalculator::outputNumber(double number, bool force_plus,
                                        char format, int decimal_digits) {
  QString ret =
      removeTrailingZeros(QString::number(number, format, decimal_digits));
  if (isAllZeros(ret) == true) {
    if (force_plus == true)
      return QString();
    else
      return QString("0");
  }
  if (force_plus == true && number > 0.0) {
    ret.insert(0, '+');
  }
  return ret;
}

QString DepositCalculator::removeTrailingZeros(QString number,
                                               bool scientific) {
  QString ret;
  bool flag = false;
  if (scientific == true) {
    for (QString::const_reverse_iterator i = number.crbegin();
         i != number.crend(); ++i) {
      if (flag == true && *i == QChar('0'))
        continue;
      else if (*i == QChar('e') || *i == QChar('E'))
        flag = true;
      else if (flag == true && *i != QChar('0'))
        flag = false;
      ret.insert(0, *i);
    }
  } else {
    if (number.contains(QChar(',')) == true ||
        number.contains(QChar('.')) == true) {
      for (QString::const_reverse_iterator i = number.crbegin();
           i != number.crend(); ++i) {
        if (flag == false && *i == QChar('0'))
          continue;
        else if (flag == false && (*i == QChar(',') || *i == QChar('.'))) {
          flag = true;
          continue;
        } else if (flag == false && *i != QChar('0')) {
          flag = true;
        }
        ret.insert(0, *i);
      }
    }
  }
  return ret;
}

bool DepositCalculator::isAllZeros(QString number) {
  for (QString::const_reverse_iterator i = number.crbegin();
       i != number.crend(); ++i) {
    if (*i != QChar(',') && *i != QChar('0')) return false;
  }
  return true;
}

/* void DepositCalculator::mousePressEvent(QMouseEvent *event) {
  QModelIndex item = ui->tableWidgetEvents->indexAt(event->pos());
  if (!item.isValid()) ui->tableWidgetEvents->clearSelection();
} */
