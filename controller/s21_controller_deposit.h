#ifndef S21_CONTROLLER_DEPOSIT_H
#define S21_CONTROLLER_DEPOSIT_H

#include "./../model/s21_deposit.h"

namespace s21 {
class ControllerDeposit {
 public:
  ControllerDeposit() = default;
  ~ControllerDeposit() = default;

  void setDeposit(double value);
  void setTerm(int term, Deposit::TermType unit);
  void setStartDate(int day, int month, int year);
  void setInterest(double value);
  void setTax(double value);
  void setCapitalization(bool set);
  void setPeriodicity(Deposit::PayPeriod value);

  void addReplenish(Deposit::OperPeriod freq, Date date, double value);
  void addWithdrawal(Deposit::OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index);
  void removeWithdrawal(size_t index);
  size_t getReplenishListSize();
  size_t getWithdrawalListSize();
  size_t getEventListSize();
  size_t getTaxListSize();
  std::vector<Deposit::Operation>::const_iterator getReplenishListElement(
      size_t index);
  std::vector<Deposit::Operation>::const_iterator getWithdrawalListElement(
      size_t index);
  std::vector<Deposit::Event>::const_iterator getEventListElement(size_t index);
  std::vector<Deposit::Tax>::const_iterator getTaxListElement(size_t index);

  void calculate();

  // void setDeposit(double value);
  // void setTerm(int days);
  // void setTermType(TermType value);
  // void setStartDate(int day, int month, int year);
  // void setInterest(double value);
  // void setTax(double value);
  // void setCapitalization(bool set);
  // void setPeriodicity(PayPeriod value);
  // // void setReminderLimit(double value);
  // void addReplenish(OperPeriod freq, Date date, double value);
  // void addWithdrawal(OperPeriod freq, Date date, double value);
  // void removeReplenish(size_t index);
  // void removeWithdrawal(size_t index);
  // size_t getReplenishListSize();
  // size_t getWIthdrawalListSize();
  // std::vector<Operation>::const_iterator getReplenishListElement(size_t
  // index); std::vector<Operation>::const_iterator
  // getWithdrawalListElement(size_t index);
  // // remove

  // /* Calcualate */
  // void calculate();
  // size_t getEventListSize();
  // std::vector<Event>::const_iterator getEventListElement(size_t index);

 private:
  Deposit deposit_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_DEPOSIT_H
