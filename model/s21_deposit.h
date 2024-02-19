#ifndef S21_DEPOSIT_H
#define S21_DEPOSIT_H

#include <algorithm>
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

  /* WARNING: the order of enum is essential and is used to compare Event
   * objects. */
  enum EventType { E_REPLENISH, E_WITHDRAWAL, E_DECLINE, E_PAYDAY, E_NEWYEAR };

  enum TermType { T_DAY, T_MONTH, T_YEAR };

  struct Operation {
   private:
    OperPeriod period_;
    Date date_;
    long double value_;

   public:
    Operation() = default;
    ~Operation() = default;
    Operation(OperPeriod period, Date date, long double value) noexcept
        : period_(period), date_(date), value_(value) {}
    friend class Deposit;

    OperPeriod period() const noexcept { return period_; }
    Date date() const noexcept { return date_; }
    long double value() const noexcept { return value_; }
  };

  struct Event {
   private:
    EventType event_;
    Date date_;
    long double gain_;
    long double balance_change_;
    long double payment_;
    long double balance_;

   public:
    Event() = default;
    ~Event() = default;
    Event(EventType event, Date date, long double gain,
          long double balance_change, long double payment,
          long double balance) noexcept
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
    long double income_;
    long double tax_;

   public:
    Tax() = default;
    ~Tax() = default;
    Tax(int year, long double income, long double tax) noexcept
        : year_(year), income_(income), tax_(tax) {}
    friend class Deposit;

    int year() const noexcept { return year_; }
    double income() const noexcept { return income_; }
    double tax() const noexcept { return tax_; }
  };

  /* Methods to set user variables. No bound checking. */
  void setDeposit(double value) noexcept;
  void setTerm(int days) noexcept;
  void setTermType(TermType value) noexcept;
  bool setStartDate();
  bool setStartDate(int day, int month, int year);
  bool setStartDate(Date& date) noexcept;
  void setInterest(double value) noexcept;
  void setTax(double value) noexcept;
  void setCapitalization(bool set) noexcept;
  void setPeriodicity(PayPeriod value) noexcept;
  void setRemainderLimit(double value) noexcept;
  void addReplenish(OperPeriod freq, Date date, double value);
  void addWithdrawal(OperPeriod freq, Date date, double value);
  void removeReplenish(size_t index) noexcept;
  void removeWithdrawal(size_t index) noexcept;
  void popBackReplenish() noexcept;
  void popBackWithdrawal() noexcept;
  void clearReplenish() noexcept;
  void clearWithdrawal() noexcept;

  /* Methods to look at some user variables. No bound checking. */
  bool isCapitalization() const noexcept;
  double getDeposit() const noexcept;
  double getRemainderLimit() const noexcept;
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
  double getBalance() const noexcept;
  double getInterestTotal() const noexcept;
  double getTaxTotal() const noexcept;
  double getReplenishTotal() const noexcept;
  double getWithdrawalTotal() const noexcept;

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
  long double deposit_ = 0.0L;
  long double interest_ = 0.0L;
  long double tax_ = 0.0L;
  long double remainder_limit_ = 0.0L;
  int term_ = 0;
  TermType term_type_ = T_DAY;
  Date start_date_;
  std::vector<Operation> replenish_list_{};
  std::vector<Operation> withdrawal_list_{};
  /* END: Variables with bound checking. */
  bool capital_ = false;
  PayPeriod periodicity_ = P_AT_END;

  /* Operational variables */
  Date end_date_;
  long double year_income_ = 0.0L;
  long double day_value_ = 0.0L;

  /* Result values */
  long double balance_ = 0.0L;
  long double interest_total_ = 0.0L;
  long double tax_total_ = 0.0L;
  long double replenish_total_ = 0.0L;
  long double withdrawal_total_ = 0.0L;
  std::vector<Event> event_list_{};
  std::vector<Tax> tax_list_{};

  /* Misc methods */
  void setDefaultValues() noexcept;
  void calculateEndDate();
  void calculateValues();
  void countReplenish(std::vector<Event>::iterator i) noexcept;
  void countWithdrawal(std::vector<Event>::iterator i) noexcept;
  void countNewyear(std::vector<Event>::iterator i);
  void countPayday(std::vector<Event>::iterator i);
  void countBalance(std::vector<Event>::iterator i) noexcept;
  void countGain(std::vector<Event>::iterator i) noexcept;
  void sumPreviousGains(std::vector<Event>::iterator i) noexcept;
  void countTax(std::vector<Event>::iterator i);
  void countDayValue(int year) noexcept;
  void fixEventList(bool splice_on) noexcept;
  void spliceOperations(size_t first, size_t second) noexcept;
  void insertDeposit();
  void insertNewYears();
  void insertReplenishList();
  void insertWithdrawalList();
  void insertPaydays();
  void insertLastPayday();
  void insertOperation(std::vector<Operation>::const_iterator element,
                       EventType event);
  void pushOperationsShiftMonths(EventType event, Date date, long double value,
                                 size_t step);
  void pushOperationsShiftMonthsWithOvercap(EventType event, Date date,
                                            long double value, size_t step);
  void pushPaydaysSkipMonths(int step);
  void pushPaydaysSkipDays(int step);
  void pushEvent(EventType event, Date date, long double change = 0.0L);
  bool validateSettings() const noexcept;
  bool checkReplenishes() const noexcept;
  bool checkWithdrawals() const noexcept;
  bool checkPositiveDouble(double value) const noexcept;
  bool checkDates() const noexcept;
  static Date nextMonthDate(const Date& date, int n);
  static long double calculateDayValue(int year, long double rate) noexcept;
  static bool isLeapYear(int year) noexcept;
  static bool dateComparator(const Event& first, const Event& second) noexcept;
};
}  // namespace s21

#endif  // S21_DEPOSIT_H
