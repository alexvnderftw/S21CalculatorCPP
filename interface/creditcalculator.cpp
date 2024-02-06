#include "creditcalculator.h"
#include "ui_creditcalculator.h"

CreditCalculator::CreditCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreditCalculator)
{
    ui->setupUi(this);
    setLimits();
    setData();
    connectSignals();
}

CreditCalculator::~CreditCalculator()
{
    delete ui;
}

void CreditCalculator::calculate() {
    setData();
    ui->tableWidgetCredit->clearContents();
    if (data.Calculate() == true) {
        fillTable();
    }
}

void CreditCalculator::fillTable() {
    ui->tableWidgetCredit->setRowCount(data.GetDataSize());
    for (size_t i = 0; i < data.GetDataSize(); ++i) {
        //ui->tableWidgetCredit->insertRow(i);
        ui->tableWidgetCredit->setItem(i, 0, new QTableWidgetItem(i + 1));
        ui->tableWidgetCredit->setItem(i, 1, new QTableWidgetItem(getDateString(data[i].GetMonth(), data[i].GetYear())));
        ui->tableWidgetCredit->setItem(i, 2, new QTableWidgetItem(QString::number(data[i].GetPayment(), 'f', 2)));
        ui->tableWidgetCredit->setItem(i, 3, new QTableWidgetItem(QString::number(data[i].GetMainPart(), 'f', 2)));
        ui->tableWidgetCredit->setItem(i, 4, new QTableWidgetItem(QString::number(data[i].GetRatePart(), 'f', 2)));
        ui->tableWidgetCredit->setItem(i, 5, new QTableWidgetItem(QString::number(data[i].GetLeftover(), 'f', 2)));
    }
}

void CreditCalculator::connectSignals() {
    connect(ui->pushButtonFillCredit, SIGNAL(clicked()), this,
            SLOT(calculate()));
    connect(ui->comboBoxTimeType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(limitTime(int)));
    connect(ui->comboBoxCreditType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(setType(int)));
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
    data.SetCredit(ui->spinBoxCredit->value());
    data.SetRate(ui->doubleSpinBoxRate->value());
    setType(ui->comboBoxCreditType->currentIndex());
    setTime();
}

void CreditCalculator::setTime() {
    if (ui->comboBoxTimeType->currentIndex() == 0) data.SetTime(ui->spinBoxTime->value() * 12);
    else if (ui->comboBoxTimeType->currentIndex() == 1) data.SetTime(ui->spinBoxTime->value());
}

void CreditCalculator::setType(int index) {
    if (index == 0) data.SetAnnuity();
    else if (index == 1) data.SetDifferential();
}

/* Slots */

void CreditCalculator::limitTime(int index) {
    //if (index == 0 && ui->spinBoxTime->value() > MAX_TIME_Y) ui->spinBoxTime->setValue(MAX_TIME_Y);
    if (index == 0) ui->spinBoxTime->setMaximum(MAX_TIME_Y);
    else if (index == 1) ui->spinBoxTime->setMaximum(MAX_TIME_M);
}

/* Misc */

QString CreditCalculator::getDateString(int month, int year) {
    return MONTHS[month - 1] + " " + QString::number(year);
}

//QString CreditCalculator::getMonth(int month) {
//    switch (month) {
//        case 1:
//        return "January";
//        break;
//        case 2:
//        return "January";
//        break;
//        case 3:
//        return "January";
//        break;
//        case 4:
//        return "January";
//        break;
//        case 5:
//        return "January";
//        break;
//        case 6:
//        return "January";
//        break;
//        case 7:
//        return "January";
//        break;
//        case 8:
//        return "January";
//        break;
//        case 9:
//        return "January";
//        break;
//        case 10:
//        return "January";
//        break;
//        case 11:
//        return "January";
//        break;
//        case 12:
//        return "January";
//        break;
//        default:
//        return "";
//    }
//}
