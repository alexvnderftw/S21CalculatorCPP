#include "s21_controller_deposit.h"

namespace s21 {

// double ControllerDeposit::calculate(const std::string expr, const std::string
// x) {
// }

void ControllerDeposit::setDeposit(double value) { deposit_.setDeposit(value); }
void ControllerDeposit::setTerm(int term, Deposit::TermType unit) {
  deposit_.setTerm(term);
  deposit_.setTermType(unit);
}
void ControllerDeposit::setStartDate(int day, int month, int year) {
  deposit_.setStartDate(day, month, year);
}
void ControllerDeposit::setInterest(double value) {
  deposit_.setInterest(value);
}
void ControllerDeposit::setTax(double value) { deposit_.setTax(value); }
void ControllerDeposit::setCapitalization(bool set) {
  deposit_.setCapitalization(set);
}
void ControllerDeposit::setPeriodicity(Deposit::PayPeriod value) {
  deposit_.setPeriodicity(value);
}
void ControllerDeposit::setRemainderLimit(double value) {
  deposit_.setRemainderLimit(value);
}

void ControllerDeposit::addReplenish(Deposit::OperPeriod freq, Date date,
                                     double value) {
  deposit_.addReplenish(freq, date, value);
}
void ControllerDeposit::addWithdrawal(Deposit::OperPeriod freq, Date date,
                                      double value) {
  deposit_.addWithdrawal(freq, date, value);
}
void ControllerDeposit::removeReplenish(size_t index) {
  deposit_.removeReplenish(index);
}
void ControllerDeposit::removeWithdrawal(size_t index) {
  deposit_.removeWithdrawal(index);
}
size_t ControllerDeposit::getReplenishListSize() {
  return deposit_.getReplenishListSize();
}
size_t ControllerDeposit::getWithdrawalListSize() {
  return deposit_.getWithdrawalListSize();
}
size_t ControllerDeposit::getEventListSize() {
  return deposit_.getEventListSize();
}
size_t ControllerDeposit::getTaxListSize() { return deposit_.getTaxListSize(); }
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getReplenishListElement(size_t index) {
  return deposit_.getReplenishListElement(index);
}
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getWithdrawalListElement(size_t index) {
  return deposit_.getWithdrawalListElement(index);
}
std::vector<Deposit::Event>::const_iterator
ControllerDeposit::getEventListElement(size_t index) {
  return deposit_.getEventListElement(index);
}
std::vector<Deposit::Tax>::const_iterator ControllerDeposit::getTaxListElement(
    size_t index) {
  return deposit_.getTaxListElement(index);
}
double ControllerDeposit::getBalance() { return deposit_.getBalance(); }
double ControllerDeposit::getDeposit() { return deposit_.getDeposit(); }
double ControllerDeposit::getInterestTotal() {
  return deposit_.getInterestTotal();
}
double ControllerDeposit::getTaxTotal() { return deposit_.getTaxTotal(); }
double ControllerDeposit::getReplenishTotal() {
  return deposit_.getReplenishTotal();
}
double ControllerDeposit::getWithdrawalTotal() {
  return deposit_.getWithdrawalTotal();
}

void ControllerDeposit::calculate() { deposit_.calculate(); }

}  // namespace s21
