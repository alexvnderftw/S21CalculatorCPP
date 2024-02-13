#include "depositcalculator.h"
#include "ui_depositcalculator.h"

DepositCalculator::DepositCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositCalculator)
{
    ui->setupUi(this);
    ui->tableWidgetEvents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetEvents->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    setCurrentDate();
    setValues();
    connectSignals();
}

DepositCalculator::~DepositCalculator()
{
    delete ui;
}

void DepositCalculator::setDefaultFocus() {
    ui->pushButtonCalculate->setFocus();
}

void DepositCalculator::connectSignals() {
      connect(ui->pushButtonCalculate, SIGNAL(clicked()), this, SLOT(calculate()));
}

void DepositCalculator::setCurrentDate() {
    ui->dateEditStartDate->setDate(QDate::currentDate());
}

void DepositCalculator::setValues() {
    data.setDeposit(ui->doubleSpinBoxDeposit->value());
    data.setTerm(ui->spinBoxTerm->value(), getUnitType());
    data.setStartDate(ui->dateEditStartDate->date().day(), ui->dateEditStartDate->date().month(), ui->dateEditStartDate->date().year());
    data.setInterest(ui->doubleSpinBoxRate->value() / 100.0);
    data.setPeriodicity(getPayPeriod());
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
}

void DepositCalculator::fillTable() {
    ui->tableWidgetEvents->setRowCount(data.getEventListSize() + 1);
    for (size_t i = 0; i < data.getEventListSize(); ++i) {
        ui->tableWidgetEvents->setItem(
            i, 0,
            new QTableWidgetItem(getDateString(data.getEventListElement(i)->date_)));
        ui->tableWidgetEvents->setItem(
            i, 1,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->gain_, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 2,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->balance_change_, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 3,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->payment_, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 4,
            new QTableWidgetItem(QString::number(data.getEventListElement(i)->balance_, 'f', 2)));
        ui->tableWidgetEvents->setItem(
            i, 5,
            new QTableWidgetItem(getEventString(data.getEventListElement(i)->event_)));
    }
//    ui->tableWidgetCredit->setItem(ctrl.getDataSize(), 0,
//                                   new QTableWidgetItem("Total:"));
//    ui->tableWidgetCredit->setItem(
//        ctrl.getDataSize(), 1,
//        new QTableWidgetItem(QString::number(ctrl.getSumPaid(), 'f', 2) +
//                             "\nTotal paid"));
//    ui->tableWidgetCredit->setItem(
//        ctrl.getDataSize(), 2,
//        new QTableWidgetItem(QString::number(ctrl.getSumMainPart(), 'f', 2) +
//                             "\nDebt paid"));
//    ui->tableWidgetCredit->setItem(
//        ctrl.getDataSize(), 3,
//        new QTableWidgetItem(QString::number(ctrl.getSumRatePart(), 'f', 2) +
//                             "\nInterest paid"));
//    ui->tableWidgetCredit->resizeRowsToContents();
}

void DepositCalculator::fillLines() {
//    if (ctrl.isAnnuity() == true)
//        ui->lineEditPayment->setText(QString::number(ctrl.getPayment(0), 'f', 2));
//    else if (ctrl.isDifferential() == true) {
//        ui->lineEditPayment->setText(QString::number(ctrl.getPayment(0), 'f', 2));
//        if (ctrl.getDataSize() > 1)
//            ui->lineEditPayment->insert(
//                "-" +
//                QString::number(ctrl.getPayment(ctrl.getDataSize() - 1), 'f', 2));
//    }
//    ui->lineEditInterest->setText(QString::number(ctrl.getSumRatePart(), 'f', 2));
//    ui->lineEditTotal->setText(
//        QString::number(ctrl.getSumRatePart() + ctrl.getSumMainPart(), 'f', 2));
}

void DepositCalculator::clearContent() {
    ui->tableWidgetEvents->clearContents();
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
    else return "Decline";
}
