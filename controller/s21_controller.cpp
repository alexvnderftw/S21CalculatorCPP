#include "s21_controller.h"

namespace s21 {

double Controller::calculate(const std::string expr, const std::string x) {
  return calculator_.GetResult(expr, x);
}

double Controller::calculate(double x) { return calculator_.GetResult(x); }

void Controller::setRadian() { calculator_.SetRadian(); }

void Controller::setDegree() { calculator_.SetDegree(); }

/* For valid response call it after at least one call of calculate */
bool Controller::isSuccessful() {
  return calculator_.GetStatus() != calculator_.COMPLETED ? false : true;
}

bool Controller::isEmpty() {
  return calculator_.GetStatus() == calculator_.EMPTY ? true : false;
}

}  // namespace s21
