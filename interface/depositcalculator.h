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
    void addReplenish();
    void removeReplenish();
    void addWithdrawal();
    void removeWithdrawal();
    void setCapitalization(int value);
    void setTermLimit(QString text);

private:
    Ui::DepositCalculator *ui;
    s21::ControllerDeposit data;

    const int MIN_TERM = 1,
        MAX_TERM_D = 36500, MAX_TERM_M = 1200, MAX_TERM_Y = 100, MAX_START_YEAR = 4000, MIN_START_YEAR = 1900;
    const double MIN_DEPOSIT = 1.0, MAX_DEPOSIT = 1000000000000.0, MIN_RATE = 0.01, MAX_RATE = 1000.0, MIN_TAX = 0.0, MAX_TAX = 100.0,
        MIN_OPERATION_VALUE = 1.0, MAX_OPERATION_VALUE = 1000000000.0, MIN_REMAINDER_LIMIT = 0.0, MAX_REMAINDER_LIMIT = 1000000000.0;
    const int INITIAL_TERM_UNIT = 2, INITIAL_PERIODICITY = 2, INITIAL_OPERATION_PERIODICITY = 0;
    const int INITIAL_TERM = 1;
    const double INITIAL_DEPOSIT = 100000.0, INITIAL_RATE = 10.0, INITIAL_TAX = 0.0, INITIAL_REMAINDER_LIMIT = 0.0, INITIAL_OPERATION_VALUE = 10000.0;
    /* Initial date parameters are set to current local date. */

    void connectSignals();

    void setUiParameters();
    void setCurrentDate();
    void setValues();
    void setInitialState();
    s21::Deposit::TermType getUnitType();
    s21::Deposit::PayPeriod getPayPeriod();
    s21::Deposit::OperPeriod getOperPeriod();

    void fillTable();
    void fillTaxes();
    void fillSummary();
    void clearContent();

    QString getDateString(s21::Date date);
    QString getEventString(s21::Deposit::EventType event);
    QString getOperPeriodString(s21::Deposit::OperPeriod period);
    static QString outputNumber(double number, bool force_plus = false, char format = 'f', int decimal_digits = '2');
    static QString removeTrailingZeros(QString number, bool scientific = false);
    static bool isAllZeros(QString number);

};

#endif // DEPOSITCALCULATOR_H
