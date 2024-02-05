#ifndef S21_CONTROLLER_CREDIT_H
#define S21_CONTROLLER_CREDIT_H

#include "./../model/s21_credit.h"

namespace s21 {
class ControllerCredit {
 public:
  ControllerCredit() = default;
  ~ControllerCredit() = default;

  void setAnnuity();
  void setDifferential();
  void setData(double credit, double rate, int time);

  bool calculate();
  bool calculate(double credit, double rate, int time);

 private:
  Credit credit_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_CREDIT_H
