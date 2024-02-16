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

  enum EventType { E_PAYDAY, E_REPLENISH, E_WITHDRAWAL, E_DECLINE, E_NEWYEAR };

  enum TermType { T_DAY, T_MONTH, T_YEAR };

  struct Operation {
   private:
    OperPeriod period_;
    Date date_;
    Decimal64 value_;

   public:
    Operation() = default;
    ~Operation() = default;
    Operation(OperPeriod period, Date date, Decimal64 value)
        : period_(period), date_(date), value_(value) {}
    friend class Deposit;

    OperPeriod period() const noexcept { return period_; }
    Date date() const noexcept { return date_; }
    double value() const { return value_.value(); }
  };

  struct Event {
   private:
    EventType event_;
    Date date_;
    Decimal64 gain_;
    Decimal64 balance_change_;
    Decimal64 payment_;
    Decimal64 balance_;

   public:
    Event() = default;
    ~Event() = default;
    Event(EventType event, Date date, Decimal64 gain, Decimal64 balance_change,
          Decimal64 payment, Decimal64 balance)
        : event_(event),
          date_(date),
          gain_(gain),
          balance_change_(balance_change),
          payment_(payment),
          balance_(balance) {}
    friend class Deposit;

    EventType event() const noexcept { return event_; }
    Date date() const noexcept { return date_; }
    double gain() const { return gain_.value(); }
    double balance_change() const { return balance_change_.value(); }
    double payment() const { return payment_.value(); }
    double balance() const { return balance_.value(); }
  };

  struct Tax {
   private:
    int year_;
    Decimal64 income_;
    Decimal64 tax_;

   public:
    Tax() = default;
    ~Tax() = default;
    Tax(int year, Decimal64 income, Decimal64 tax)
        : year_(year), income_(income), tax_(tax) {}
    friend class Deposit;

    int year() const noexcept { return year_; }
    double income() const { return income_.value(); }
    double tax() const { return tax_.value(); }
  };

  /* Methods to set user variables. No bound checking. */
  void setDeposit(double value);
  void setTerm(int days);
  void setTermType(TermType value) noexcept;
  bool setStartDate(int day, int month, int year) noexcept;
  bool setStartDate(Date date) noexcept;
  void setInterest(double value);
  void setTax(double value);
  void setCapitalization(bool set) noexcept;
  void setPeriodicity(PayPeriod value) noexcept;
  void setRemainderLimit(double value);
  void addReplenish(OperPeriod freq, Date date, double value);
  void addWithdrawal(OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index);
  void removeWithdrawal(size_t index);
  void popBackReplenish();
  void popBackWithdrawal();
  void clearReplenish();
  void clearWithdrawal();

  /* Methods to look at some user variables. No bound checking. */
  bool isCapitalization() const noexcept;
  double getDeposit() const;
  size_t getReplenishListSize() const noexcept;
  size_t getWithdrawalListSize() const noexcept;
  std::vector<Operation>::const_iterator getReplenishListElement(
      size_t index) const noexcept;
  std::vector<Operation>::const_iterator getWithdrawalListElement(
      size_t index) const noexcept;

  /* Main method to run. Provides bound checking. */
  bool calculate();

  /* Methods to retrieve result content. */
  size_t getEventListSize() const noexcept;
  std::vector<Event>::const_iterator getEventListElement(
      size_t index) const noexcept;
  size_t getTaxListSize() const noexcept;
  std::vector<Tax>::const_iterator getTaxListElement(
      size_t index) const noexcept;
  double getBalance() const;
  double getInterestTotal() const;
  double getTaxTotal() const;
  double getReplenishTotal() const;
  double getWithdrawalTotal() const;

  /* Limit values. CAREFUL: The weak parts are values near MAX_DEPOSIT_VALUE
   * and MAX_RATE with capitalization option turned on, it can
   * potentially lead to numbers reach beyond max double limit. */
  static constexpr const double MAX_DEPOSIT_VALUE =
                                    std::numeric_limits<double>::max() /
                                    1000000.0,
                                MAX_TAX = 1.0, MAX_RATE = 1000.0,
                                MAX_DOUBLE_VALUE =
                                    std::numeric_limits<double>::max();
  static constexpr const int MAX_TERM_D = 365000, MAX_TERM_M = 12000,
                             MAX_TERM_Y = 999, MAX_START_YEAR = 5000;

 private:
  /* User settings */
  /* START: Variables with bound checking. */
  Decimal64 deposit_ = 0.0;
  Decimal64 interest_ = 0.0;
  Decimal64 tax_ = 0.0;
  Decimal64 remainder_limit_ = 0.0;
  Decimal64 term_ = 0;
  TermType term_type_ = T_DAY;
  Date start_date_;
  std::vector<Operation> replenish_list_{};
  std::vector<Operation> withdrawal_list_{};
  /* END: Variables with bound checking. */
  bool capital_ = false;
  PayPeriod periodicity_ = P_AT_END;

  /* Operational variables */
  Date end_date_;
  Decimal64 year_income_ = 0.0;

  /* Result values */
  Decimal64 balance_ = 0.0;
  Decimal64 interest_total_ = 0.0;
  Decimal64 tax_total_ = 0.0;
  Decimal64 replenish_total_ = 0.0;
  Decimal64 withdrawal_total_ = 0.0;
  std::vector<Event> event_list_{};
  std::vector<Tax> tax_list_{};

  /* Misc methods */
  void setDefaultValues();
  void calculateEndDate();
  void calculateValues();
  void fixEventList(bool splice_on);
  void spliceOperations(size_t first, size_t second);
  void swapEvents(size_t first, size_t second) noexcept;
  void insertDeposit();
  void insertNewYears();
  void insertReplenishList();
  void insertWithdrawalList();
  void insertPaydays();
  void insertLastPayday();
  void insertReplenish(size_t i);
  void insertWithdrawal(size_t i);
  bool validateSettings() const;
  bool checkReplenishes() const;
  bool checkWithdrawals() const;
  bool checkPositiveDouble(double value) const noexcept;
  bool checkDates() const;
  static Date nextMonthDate(Date date);
  static Decimal64 calculateDayValue(int year, Decimal64 rate);
  static bool isLeapYear(int year) noexcept;
  static bool dateComparator(Event first, Event second) noexcept;
};
}  // namespace s21

#endif  // S21_DEPOSIT_H
