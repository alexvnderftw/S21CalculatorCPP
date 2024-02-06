#include "s21_controller_credit.h"

namespace s21 {

void ControllerCredit::setAnnuity() { credit_.SetAnnuity(); }

void ControllerCredit::setDifferential() { credit_.SetDifferential(); }

bool ControllerCredit::isAnnuity() {
  return credit_.GetType() == credit_.ANNUITY;
}

bool ControllerCredit::isDifferential() {
  return credit_.GetType() == credit_.DIFFERENTIAL;
}

void ControllerCredit::setData(double credit, double rate, int time) {
  credit_.SetCredit(credit);
  credit_.SetRate(rate);
  credit_.SetTime(time);
  credit_.SetStartDate();
}

bool ControllerCredit::calculate() { return credit_.Calculate(); }

bool ControllerCredit::calculate(double credit, double rate, int time) {
  setData(credit, rate, time);
  return calculate();
}

double ControllerCredit::getSumPaid() { return credit_.GetSummaryPaid(); }

double ControllerCredit::getSumMainPart() {
  return credit_.GetSummaryMainPart();
}

double ControllerCredit::getSumRatePart() {
  return credit_.GetSummaryRatePart();
}

int ControllerCredit::getMonth(size_t pos) { return credit_[pos].GetMonth(); }

int ControllerCredit::getYear(size_t pos) { return credit_[pos].GetYear(); }

double ControllerCredit::getPayment(size_t pos) {
  return credit_[pos].GetPayment();
}

double ControllerCredit::getMainPart(size_t pos) {
  return credit_[pos].GetMainPart();
}

double ControllerCredit::getRatePart(size_t pos) {
  return credit_[pos].GetRatePart();
}

double ControllerCredit::getLeftover(size_t pos) {
  return credit_[pos].GetLeftover();
}

size_t ControllerCredit::getDataSize() { return credit_.GetDataSize(); }

}  // namespace s21
