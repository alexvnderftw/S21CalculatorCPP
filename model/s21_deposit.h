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
   private:
    OperPeriod period_;
    Date date_;
    double value_;

   public:
    Operation() = default;
    ~Operation() = default;
    Operation(OperPeriod period, Date date, double value)
        : period_(period), date_(date), value_(value) {}
    friend class Deposit;
  };

  struct Event {
   private:
    EventType event_;
    Date date_;
    double gain_;
    double balance_change_;
    double payment_;
    double balance_;

   public:
    Event() = default;
    ~Event() = default;
    Event(EventType event, Date date, double gain, double balance_change,
          double payment, double balance)
        : event_(event),
          date_(date),
          gain_(gain),
          balance_change_(balance_change),
          payment_(payment),
          balance_(balance) {}
    friend class Deposit;

    EventType event() const noexcept { return event_; }
    Date date() const noexcept { return date_; }
    double gain() const noexcept { return gain_; }
    double balance_change() const noexcept { return balance_change_; }
    double payment() const noexcept { return payment_; }
    double balance() const noexcept { return balance_; }
  };

  struct Tax {
   private:
    int year_;
    double income_;
    double tax_;

   public:
    Tax() = default;
    ~Tax() = default;
    Tax(int year, double income, double tax)
        : year_(year), income_(income), tax_(tax) {}
    friend class Deposit;

    int year() const noexcept { return year_; }
    double income() const noexcept { return income_; }
    double tax() const noexcept { return tax_; }
  };

  /* Methods to set user variables. No bound checking, except setStartDate. */
  void setDeposit(double value) noexcept;
  void setTerm(int days) noexcept;
  void setTermType(TermType value) noexcept;
  void setStartDate(int day, int month, int year) noexcept;
  void setInterest(double value) noexcept;
  void setTax(double value) noexcept;
  void setCapitalization(bool set) noexcept;
  void setPeriodicity(PayPeriod value) noexcept;
  void setRemainderLimit(double value) noexcept;
  void addReplenish(OperPeriod freq, Date date, double value);
  void addWithdrawal(OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index);
  void removeWithdrawal(size_t index);

  /* Methods to look at some user variables. No bound checking. */
  double getDeposit() const noexcept;
  size_t getReplenishListSize() const noexcept;
  size_t getWithdrawalListSize() const noexcept;
  std::vector<Operation>::const_iterator getReplenishListElement(
      size_t index) const noexcept;
  std::vector<Operation>::const_iterator getWithdrawalListElement(
      size_t index) const noexcept;

  /* Main method to run. Provides bound checking. */
  void calculate();

  /* Methods to retrieve result content. */
  size_t getEventListSize() const noexcept;
  std::vector<Event>::const_iterator getEventListElement(
      size_t index) const noexcept;
  size_t getTaxListSize() const noexcept;
  std::vector<Tax>::const_iterator getTaxListElement(
      size_t index) const noexcept;
  double getBalance() const noexcept;
  double getInterestTotal() const noexcept;
  double getTaxTotal() const noexcept;
  double getReplenishTotal() const noexcept;
  double getWithdrawalTotal() const noexcept;

 private:
  /* Limit values. CAREFUL: The weak parts are values near MAX_DEPOSIT_VALUE
   * and MAX_RATE with capitalization option turned on, it can
   * potentially lead to numbers reach beyond max double limit. */
  const double MAX_DEPOSIT_VALUE =
                   std::numeric_limits<double>::max() / 1000000.0,
               MAX_TAX = 1.0, MAX_RATE = 1000.0,
               MAX_DOUBLE_VALUE = std::numeric_limits<double>::max();
  const int MAX_TERM_D = 365000, MAX_TERM_M = 12000, MAX_TERM_Y = 999,
            MAX_START_YEAR = 5000;

  /* User settings */
  /* START: Variables with bound checking. */
  double deposit_ = 0.0;
  double interest_ = 0.0;
  double tax_ = 0.0;
  double remainder_limit_ = 0.0;
  int term_ = 0;
  TermType term_type_ = T_DAY;
  Date start_date_;
  /* END: Variables with bound checking. */
  bool capital_ = false;
  PayPeriod periodicity_ = P_AT_END;
  std::vector<Operation> replenish_list_{};
  std::vector<Operation> withdrawal_list_{};

  /* Operational variables */
  Date end_date_;
  double year_income_ = 0.0;

  /* Result values */
  double balance_ = 0.0;
  double interest_total_ = 0.0;
  double tax_total_ = 0.0;
  double replenish_total_ = 0.0;
  double withdrawal_total_ = 0.0;
  std::vector<Event> event_list_{};
  std::vector<Tax> tax_list_{};

  /* Misc methods */
  void setDefaultValues() noexcept;
  void calculateTerm();
  void calculateValues();
  void spliceOperations();
  void insertDeposit();
  void insertNewYears();
  void insertReplenishList();
  void insertWithdrawalList();
  void insertPaydays();
  void insertLastPayday();
  void insertReplenish(size_t i);
  void insertWithdrawal(size_t i);
  bool validateSettings() const noexcept;
  bool checkPositiveDouble(double value) const noexcept;
  bool checkDates() const noexcept;
  static Date nextMonthDate(Date date);
  static double calculateDayValue(int year, double rate) noexcept;
  static bool isLeapYear(int year) noexcept;
  static bool dateComparator(Event first, Event second) noexcept;
};
}  // namespace s21

#endif  // S21_DEPOSIT_H
