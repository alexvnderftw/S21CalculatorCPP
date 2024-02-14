#include "depositcalculator.h"
#include "ui_depositcalculator.h"

DepositCalculator::DepositCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositCalculator)
{
    ui->setupUi(this);
    setUiParameters();
    setInitialState();
    //setValues();
    connectSignals();
}

DepositCalculator::~DepositCalculator()
{
    delete ui;
}

void DepositCalculator::setDefaultFocus() {
    ui->pushButtonCalculate->setFocus();
}

void DepositCalculator::setUiParameters() {
    ui->tableWidgetEvents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetEvents->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableWidgetReplenishes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetReplenishes->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableWidgetWithdrawals->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetWithdrawals->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableWidgetTax->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTax->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
}

void DepositCalculator::connectSignals() {
    connect(ui->pushButtonCalculate, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(ui->pushButtonAddReplenish, SIGNAL(clicked()), this, SLOT(addReplenish()));
    connect(ui->pushButtonRemoveReplenish, SIGNAL(clicked()), this, SLOT(removeReplenish()));
    connect(ui->pushButtonAddWithdrawal, SIGNAL(clicked()), this, SLOT(addWithdrawal()));
    connect(ui->pushButtonRemoveWithdrawal, SIGNAL(clicked()), this, SLOT(removeWithdrawal()));
    connect(ui->checkBoxCapitalization, SIGNAL(stateChanged(int)), this, SLOT(setCapitalization(int)));
    connect(ui->comboBoxTermUnit, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTermLimit(QString)));
}

void DepositCalculator::setTermLimit(QString text) {
    if (text == "years") ui->spinBoxTerm->setMaximum(MAX_TERM_Y);
    else if (text == "months") ui->spinBoxTerm->setMaximum(MAX_TERM_M);
    else ui->spinBoxTerm->setMaximum(MAX_TERM_D);
}

void DepositCalculator::setCurrentDate() {
    ui->dateEditStartDate->setDate(QDate::currentDate());
    ui->dateEditOperationDate->setDate(QDate::currentDate());
}

void DepositCalculator::setValues() {
    data.setDeposit(ui->doubleSpinBoxDeposit->value());
    data.setTerm(ui->spinBoxTerm->value(), getUnitType());
    data.setStartDate(ui->dateEditStartDate->date().day(), ui->dateEditStartDate->date().month(), ui->dateEditStartDate->date().year());
    data.setInterest(ui->doubleSpinBoxRate->value() / 100.0);
    data.setPeriodicity(getPayPeriod());
    data.setTax(ui->doubleSpinBoxTax->value() / 100.0);
    data.setRemainderLimit(ui->doubleSpinBoxRemainderLimit->value());
}

s21::Deposit::TermType DepositCalculator::getUnitType() {
    if (ui->comboBoxTermUnit->currentIndex() == 0) return s21::Deposit::T_DAY;
    else if (ui->comboBoxTermUnit->currentIndex() == 1) return s21::Deposit::T_MONTH;
    else return s21::Deposit::T_YEAR;
}

s21::Deposit::PayPeriod DepositCalculator::getPayPeriod() {
    if (ui->comboBoxPeriodicity->currentIndex() == 0) return s21::Deposit::P_AT_END;
    else if (ui->comboBoxPeriodicity->currentIndex() == 1) return s21::Deposit::P_DAILY;
    else if (ui->comboBoxPeriodicity->currentIndex() == 2) return s21::Deposit::P_MONTHLY;
    else if (ui->comboBoxPeriodicity->currentIndex() == 3) return s21::Deposit::P_QUARTERLY;
    else if (ui->comboBoxPeriodicity->currentIndex() == 4) return s21::Deposit::P_BIANNUALLY;
    else return s21::Deposit::P_ANNUALLY;
}

void DepositCalculator::calculate() {
    clearContent();
    setValues();
    data.calculate();
    fillTable();
    fillTaxes();
    fillSummary();
}

void DepositCalculator::fillTable() {
    ui->tableWidgetEvents->setRowCount(data.getEventListSize() + 1);
    for (size_t i = 0; i < data.getEventListSize(); ++i) {
        ui->tableWidgetEvents->setItem(
            i, 0,
            new QTableWidgetItem(getDateString(data.getEventListElement(i)->date())));
        ui->tableWidgetEvents->setItem(
            i, 1,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->gain(), 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 2,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->balance_change(), 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 3,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->payment(), 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 4,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->balance(), 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 5,
            new QTableWidgetItem(getEventString(data.getEventListElement(i)->event())));
    }
    ui->tableWidgetEvents->setItem(data.getEventListSize(), 0,
                                   new QTableWidgetItem("Total:"));
    ui->tableWidgetEvents->setItem(
        data.getEventListSize(), 1,
        new QTableWidgetItem(QString::number(data.getInterestTotal(), 'f', 2)));
    ui->tableWidgetEvents->setItem(
        data.getEventListSize(), 4,
        new QTableWidgetItem(QString::number(data.getBalance(), 'f', 2)));
    ui->tableWidgetEvents->resizeRowsToContents();
}

void DepositCalculator::fillTaxes() {
    ui->tableWidgetTax->setRowCount(data.getTaxListSize());
    for (size_t i = 0; i < data.getTaxListSize(); ++i) {
        ui->tableWidgetTax->setItem(
            i, 0,
            new QTableWidgetItem(QString::number(data.getTaxListElement(i)->year())));
        ui->tableWidgetTax->setItem(
            i, 1,
            new QTableWidgetItem(QString::number(data.getTaxListElement(i)->income(), 'f', 2)));
        ui->tableWidgetTax->setItem(
            i, 2,
            new QTableWidgetItem(QString::number(data.getTaxListElement(i)->tax(), 'f', 2)));
    }
    ui->tableWidgetTax->resizeRowsToContents();
}

void DepositCalculator::clearContent() {
    ui->tableWidgetEvents->clearContents();
    ui->tableWidgetTax->clearContents();
    ui->lineEditAccruedInterest->clear();
    ui->lineEditTotal->clear();
    ui->lineEditGain->clear();
    ui->lineEditTotalTax->clear();
//    ui->lineEditPayment->clear();
//    ui->lineEditInterest->clear();
//    ui->lineEditTotal->clear();
}

QString DepositCalculator::getDateString(s21::Date date) {
    QDate qdate(date.getYear(), date.getMonth(), date.getDay());
    return qdate.toString("dd.MM.yyyy");
    //return QString::number(date.getDay()) + "." + QString::number(date.getMonth()) + "." + QString::number(date.getYear());

    //.toString("yyyy.MM.dd")
}

QString DepositCalculator::getEventString(s21::Deposit::EventType event) {
    if (event == s21::Deposit::E_REPLENISH) return "Replenishment";
    else if (event == s21::Deposit::E_PAYDAY) return "Payment";
    else if (event == s21::Deposit::E_WITHDRAWAL) return "Withdrawal";
    else if (event == s21::Deposit::E_PAYTAX) return "Taxes";
    else if (event == s21::Deposit::E_NEWYEAR) return "New year";
    else if (event == s21::Deposit::E_DECLINE) return "Withdrawal declined";
    else return "Error";
}

QString DepositCalculator::getOperPeriodString(s21::Deposit::OperPeriod period) {
    if (period == s21::Deposit::O_ONCE) return "One-time";
    else if (period == s21::Deposit::O_MONTHLY) return "Monthly";
    else if (period == s21::Deposit::O_BIMONTHLY) return "Every two months";
    else if (period == s21::Deposit::O_QUARTERLY) return "Quarterly";
    else if (period == s21::Deposit::O_BIANNUALLY) return "Twice a year";
    else if (period == s21::Deposit::O_ANNUALLY) return "Every year";
    else return "Error";
}

s21::Deposit::OperPeriod DepositCalculator::getOperPeriod() {
    if (ui->comboBoxOperationPeriodicity->currentIndex() == 0) return s21::Deposit::O_ONCE;
    else if (ui->comboBoxOperationPeriodicity->currentIndex() == 1) return s21::Deposit::O_MONTHLY;
    else if (ui->comboBoxOperationPeriodicity->currentIndex() == 2) return s21::Deposit::O_BIMONTHLY;
    else if (ui->comboBoxOperationPeriodicity->currentIndex() == 3) return s21::Deposit::O_QUARTERLY;
    else if (ui->comboBoxOperationPeriodicity->currentIndex() == 4) return s21::Deposit::O_BIANNUALLY;
    else return s21::Deposit::O_ANNUALLY;
}

/* Slots */

void DepositCalculator::addReplenish() {
    s21::Date date(ui->dateEditOperationDate->date().day(), ui->dateEditOperationDate->date().month(), ui->dateEditOperationDate->date().year());
    data.addReplenish(getOperPeriod(), date, ui->doubleSpinBoxValue->value());
    ui->tableWidgetReplenishes->insertRow(ui->tableWidgetReplenishes->rowCount());
    ui->tableWidgetReplenishes->setItem(ui->tableWidgetReplenishes->rowCount() - 1, 0, new QTableWidgetItem(getOperPeriodString(getOperPeriod())));
    ui->tableWidgetReplenishes->setItem(ui->tableWidgetReplenishes->rowCount() - 1, 1, new QTableWidgetItem(getDateString(date)));
    ui->tableWidgetReplenishes->setItem(ui->tableWidgetReplenishes->rowCount() - 1, 2, new QTableWidgetItem(QString::number(ui->doubleSpinBoxValue->value(), 'f', 2)));
    ui->tableWidgetReplenishes->selectRow(ui->tableWidgetReplenishes->rowCount() - 1);
    //    QPushButton *removeButton = new QPushButton("Remove", this);
//    ui->tableWidgetReplenishes->setCellWidget(ui->tableWidgetReplenishes->rowCount() - 1, 3, removeButton);
//    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeReplenish()));
}

void DepositCalculator::removeReplenish() {
    if (data.getReplenishListSize() > 0) {
    int index = ui->tableWidgetReplenishes->currentRow();
    ui->tableWidgetReplenishes->removeRow(ui->tableWidgetReplenishes->currentRow());
    data.removeReplenish(index);
    if (index > 0) ui->tableWidgetReplenishes->selectRow(index - 1);
    else ui->tableWidgetReplenishes->selectRow(0);
//    if (index > 0)
//        ui->tableWidgetReplenishes->setCurrentCell(index - 1, 0);
//    else
//        ui->tableWidgetReplenishes->setCurrentCell(0, 0);
//    }
//    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
//    if(w){
//        int row = ui->tableWidgetReplenishes->indexAt(w->pos()).row();
//        ui->tableWidgetReplenishes->removeRow(row);
//        ui->tableWidgetReplenishes->setCurrentCell(0, 0);
    }
}

void DepositCalculator::addWithdrawal() {
    s21::Date date(ui->dateEditOperationDate->date().day(), ui->dateEditOperationDate->date().month(), ui->dateEditOperationDate->date().year());
    data.addWithdrawal(getOperPeriod(), date, ui->doubleSpinBoxValue->value());
    ui->tableWidgetWithdrawals->insertRow(ui->tableWidgetWithdrawals->rowCount());
    ui->tableWidgetWithdrawals->setItem(ui->tableWidgetWithdrawals->rowCount() - 1, 0, new QTableWidgetItem(getOperPeriodString(getOperPeriod())));
    ui->tableWidgetWithdrawals->setItem(ui->tableWidgetWithdrawals->rowCount() - 1, 1, new QTableWidgetItem(getDateString(date)));
    ui->tableWidgetWithdrawals->setItem(ui->tableWidgetWithdrawals->rowCount() - 1, 2, new QTableWidgetItem(QString::number(ui->doubleSpinBoxValue->value(), 'f', 2)));
    ui->tableWidgetWithdrawals->selectRow(ui->tableWidgetWithdrawals->rowCount() - 1);
}

// error!!!
void DepositCalculator::removeWithdrawal() {
    if (ui->tableWidgetWithdrawals->rowCount() > 0) {
    int index = ui->tableWidgetWithdrawals->currentRow();
    ui->tableWidgetWithdrawals->removeRow(ui->tableWidgetWithdrawals->currentRow());
    data.removeWithdrawal(index);
    if (index > 0) ui->tableWidgetWithdrawals->selectRow(index - 1);
    else ui->tableWidgetWithdrawals->selectRow(0);
    }
}

void DepositCalculator::setCapitalization(int value) {
    if (value == 0) {
        data.setCapitalization(false);
    } else {
        data.setCapitalization(true);
    }
}

void DepositCalculator::fillSummary() {
    ui->lineEditAccruedInterest->setText(QString::number(data.getInterestTotal(), 'f', 2));
    ui->lineEditTotal->setText(QString::number(data.getInterestTotal() + data.getDeposit() + data.getReplenishTotal(), 'f', 2));
    ui->lineEditGain->setText(QString::number(data.getInterestTotal() / data.getDeposit() * 100.0, 'f', 2) + " %");
    ui->lineEditTotalTax->setText(QString::number(data.getTaxTotal(), 'f', 2));
}

void DepositCalculator::setInitialState() {
    setCurrentDate();
    ui->checkBoxCapitalization->setChecked(false);
    ui->comboBoxTermUnit->setCurrentIndex(INITIAL_TERM_UNIT);
    ui->spinBoxTerm->setValue(INITIAL_TERM);
    setTermLimit(ui->comboBoxTermUnit->currentText());
    ui->comboBoxPeriodicity->setCurrentIndex(INITIAL_PERIODICITY);
    ui->comboBoxOperationPeriodicity->setCurrentIndex(INITIAL_OPERATION_PERIODICITY);
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
}
