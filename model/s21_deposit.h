#ifndef S21_DEPOSIT_H
#define S21_DEPOSIT_H

#include <vector>

#include "s21_common.h"

namespace s21 {
class Deposit {
 public:
  Deposit() = default;
  ~Deposit() = default;

  enum PayPeriod {
    P_AT_END,
    P_DAILY,
    P_WEEKLY,
    P_MONTHLY,
    P_QUARTERLY,
    P_BIANNUALLY,
    P_ANNUALLY
  };

  enum OperPeriod {
    O_ONCE,
    O_MONTHLY,
    O_BIMONTHLY,
    O_QUARTERLY,
    O_BIANNUALLY,
    O_ANNUALLY
  };

  enum EventType {
    E_PAYDAY,
    E_REPLENISH,
    E_WITHDRAWAL,
    E_DECLINE,
    E_PAYTAX,
    E_NEWYEAR
  };

  enum TermType { T_DAY, T_MONTH, T_YEAR };

  struct Operation {
    OperPeriod period_;
    Date date_;
    double value_;

    Operation(OperPeriod period, Date date, double value)
        : period_(period), date_(date), value_(value) {}
  };

  struct Event {
    EventType event_;
    Date date_;
    double gain_;
    double balance_change_;
    double payment_;
    double balance_;
    // double tax;
    Event(EventType event, Date date, double gain, double balance_change,
          double payment, double balance)
        : event_(event),
          date_(date),
          gain_(gain),
          balance_change_(balance_change),
          payment_(payment),
          balance_(balance) {}
  };

  void setDeposit(double value);
  void setTerm(int days);
  void setTermType(TermType value);
  void setStartDate(int day, int month, int year);
  void setInterest(double value);
  void setTax(double value);
  void setCapitalization(bool set);
  void setPeriodicity(PayPeriod value);
  // void setReminderLimit(double value);
  void addReplenish(OperPeriod freq, Date date, double value);
  void addWithdrawal(OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index);
  void removeWithdrawal(size_t index);
  size_t getReplenishListSize();
  size_t getWIthdrawalListSize();
  std::vector<Operation>::const_iterator getReplenishListElement(size_t index);
  std::vector<Operation>::const_iterator getWithdrawalListElement(size_t index);
  // remove

  /* Calcualate */
  void calculate();
  size_t getEventListSize();
  std::vector<Event>::const_iterator getEventListElement(size_t index);

 private:
  double deposit_ = 0.0;
  int term_ = 0;
  TermType term_type_ = T_DAY;
  Date start_date_;
  Date end_date_;
  double interest_;
  double tax_ = 0.0;
  bool capital_ = false;
  PayPeriod periodicity_ = P_MONTHLY;
  double remainder_limit_ = 0.0;
  std::vector<Operation> replenish_list_{};
  std::vector<Operation> withdrawal_list_{};

  double balance_ = 0.0;
  double end_balance_ = 0.0;
  double deposit_total_ = 0.0;
  double interest_total_ = 0.0;
  double interest_gain_ = 0.0;
  double tax_total_ = 0.0;
  double withdrawal_total_ = 0.0;
  std::vector<Event> event_list_{};

  /* Misc */
  void insertNewYears();
  void insertDeposit();
  void insertLastPayday();
  void insertPaydays();
  void insertTaxes();
  void insertReplenishList();
  void insertWithdrawalList();

  void insertReplenish(size_t i);
  void insertWithdrawal(size_t i);

  static Date nextMonthDate(Date date);
  static double calculateDayValue(int year, double rate);
  static bool isLeapYear(int year);

  void calculateTerm();
  bool dateComparator(Event first, Event second);

  void spliceOperations();
};
}  // namespace s21

#endif  // S21_DEPOSIT_H
