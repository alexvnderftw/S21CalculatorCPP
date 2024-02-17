#ifndef CREDITCALCULATOR_H
#define CREDITCALCULATOR_H

#include <QWidget>

#include "../controller/s21_controller_credit.h"

namespace Ui {
class CreditCalculator;
}

class CreditCalculator : public QWidget {
  Q_OBJECT

 public:
  explicit CreditCalculator(QWidget *parent = nullptr);
  ~CreditCalculator();

  void setDefaultFocus();
  void setDefaultSizes();
  void nullDefaultSizes();

 private slots:
  void calculate();
  void limitTime(int index);
  void setType(int index);

 private:
  Ui::CreditCalculator *ui;
  s21::ControllerCredit ctrl;
  const int MIN_CREDIT = 1, MAX_CREDIT = 1000000000, MIN_TIME = 1,
            MAX_TIME_M = 600, MAX_TIME_Y = 50;
  const double MIN_RATE = 0.01, MAX_RATE = 1000.0;
  const std::vector<QString> MONTHS{
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};

  void clearContent();
  void fillTable();
  void fillLines();
  void connectSignals();
  void setLimits();
  void setData();
  int getTime();
  QString getDateString(int month, int year);
};

#endif  // CREDITCALCULATOR_H
