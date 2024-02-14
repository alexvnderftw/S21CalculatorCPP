#ifndef S21_CONTROLLER_DEPOSIT_H
#define S21_CONTROLLER_DEPOSIT_H

#include "./../model/s21_deposit.h"

namespace s21 {
class ControllerDeposit {
 public:
  ControllerDeposit() = default;
  ~ControllerDeposit() = default;

  void setDeposit(double value) noexcept;
  void setTerm(int term, Deposit::TermType unit) noexcept;
  void setStartDate(int day, int month, int year) noexcept;
  void setInterest(double value) noexcept;
  void setTax(double value) noexcept;
  void setCapitalization(bool set) noexcept;
  void setPeriodicity(Deposit::PayPeriod value) noexcept;
  void setRemainderLimit(double value) noexcept;
  void addReplenish(Deposit::OperPeriod freq, Date date, double value);
  void addWithdrawal(Deposit::OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index);
  void removeWithdrawal(size_t index);

  bool isCapitalization() const noexcept;
  double getDeposit() const noexcept;
  size_t getReplenishListSize() const noexcept;
  size_t getWithdrawalListSize() const noexcept;
  std::vector<Deposit::Operation>::const_iterator getReplenishListElement(
      size_t index) const noexcept;
  std::vector<Deposit::Operation>::const_iterator getWithdrawalListElement(
      size_t index) const noexcept;

  size_t getEventListSize() const noexcept;
  size_t getTaxListSize() const noexcept;
  std::vector<Deposit::Event>::const_iterator getEventListElement(
      size_t index) const noexcept;
  std::vector<Deposit::Tax>::const_iterator getTaxListElement(
      size_t index) const noexcept;
  double getBalance() const noexcept;
  double getInterestTotal() const noexcept;
  double getTaxTotal() const noexcept;
  double getReplenishTotal() const noexcept;
  double getWithdrawalTotal() const noexcept;

  void calculate();

 private:
  Deposit deposit_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_DEPOSIT_H
