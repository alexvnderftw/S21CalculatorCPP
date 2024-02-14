#include "s21_controller_deposit.h"

namespace s21 {

/* Set methods */

void ControllerDeposit::setDeposit(double value) noexcept {
  deposit_.setDeposit(value);
}
void ControllerDeposit::setTerm(int term, Deposit::TermType unit) noexcept {
  deposit_.setTerm(term);
  deposit_.setTermType(unit);
}
void ControllerDeposit::setStartDate(int day, int month, int year) noexcept {
  deposit_.setStartDate(day, month, year);
}
void ControllerDeposit::setInterest(double value) noexcept {
  deposit_.setInterest(value);
}
void ControllerDeposit::setTax(double value) noexcept {
  deposit_.setTax(value);
}
void ControllerDeposit::setCapitalization(bool set) noexcept {
  deposit_.setCapitalization(set);
}
void ControllerDeposit::setPeriodicity(Deposit::PayPeriod value) noexcept {
  deposit_.setPeriodicity(value);
}
void ControllerDeposit::setRemainderLimit(double value) noexcept {
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

/* Get methods */

double ControllerDeposit::getDeposit() const noexcept {
  return deposit_.getDeposit();
}
size_t ControllerDeposit::getReplenishListSize() const noexcept {
  return deposit_.getReplenishListSize();
}
size_t ControllerDeposit::getWithdrawalListSize() const noexcept {
  return deposit_.getWithdrawalListSize();
}
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getReplenishListElement(size_t index) const noexcept {
  return deposit_.getReplenishListElement(index);
}
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getWithdrawalListElement(size_t index) const noexcept {
  return deposit_.getWithdrawalListElement(index);
}

/* Get result methods */

size_t ControllerDeposit::getEventListSize() const noexcept {
  return deposit_.getEventListSize();
}
size_t ControllerDeposit::getTaxListSize() const noexcept {
  return deposit_.getTaxListSize();
}
std::vector<Deposit::Event>::const_iterator
ControllerDeposit::getEventListElement(size_t index) const noexcept {
  return deposit_.getEventListElement(index);
}
std::vector<Deposit::Tax>::const_iterator ControllerDeposit::getTaxListElement(
    size_t index) const noexcept {
  return deposit_.getTaxListElement(index);
}
double ControllerDeposit::getBalance() const noexcept {
  return deposit_.getBalance();
}
double ControllerDeposit::getInterestTotal() const noexcept {
  return deposit_.getInterestTotal();
}
double ControllerDeposit::getTaxTotal() const noexcept {
  return deposit_.getTaxTotal();
}
double ControllerDeposit::getReplenishTotal() const noexcept {
  return deposit_.getReplenishTotal();
}
double ControllerDeposit::getWithdrawalTotal() const noexcept {
  return deposit_.getWithdrawalTotal();
}

/* Main method */
void ControllerDeposit::calculate() { deposit_.calculate(); }

}  // namespace s21
