#ifndef DEPOSITCALCULATOR_H
#define DEPOSITCALCULATOR_H

#include <QWidget>
#include "../controller/s21_controller_deposit.h"

namespace Ui {
class DepositCalculator;
}

class DepositCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit DepositCalculator(QWidget *parent = nullptr);
    ~DepositCalculator();

    void setDefaultFocus();

private slots:
    void calculate();

private:
    Ui::DepositCalculator *ui;
    s21::ControllerDeposit data;

    void connectSignals();

    void setCurrentDate();
    void setValues();
    s21::Deposit::TermType getUnitType();
    s21::Deposit::PayPeriod getPayPeriod();

    void fillTable();
    void fillLines();
    void clearContent();

    QString getDateString(s21::Date date);
    QString getEventString(s21::Deposit::EventType event);

};

#endif // DEPOSITCALCULATOR_H
