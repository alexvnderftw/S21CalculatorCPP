#include "s21_controller_credit.h"

namespace s21 {

void ControllerCredit::setAnnuity() { credit_.SetAnnuity(); }

void ControllerCredit::setDifferential() { credit_.SetDifferential(); }

void ControllerCredit::setData(double credit, double rate, int time) {
  credit_.SetCredit(credit);
  credit_.SetRate(rate);
  credit_.SetTime(time);
}

bool ControllerCredit::calculate() { return credit_.Calculate(); }

bool ControllerCredit::calculate(double credit, double rate, int time) {
  setData(credit, rate, time);
  return calculate();
}

}  // namespace s21
