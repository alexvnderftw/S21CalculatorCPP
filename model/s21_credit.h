#ifndef S21_CREDIT_H
#define S21_CREDIT_H

#include <vector>

#include "s21_common.h"

namespace s21 {
class Credit {
 public:
  Credit() = default;
  ~Credit() = default;

  class CreditMonth {
    friend class Credit;

   public:
    CreditMonth() = default;
    ~CreditMonth() = default;

    int GetMonth() { return month_; }
    int GetYear() { return year_; }
    double GetPayment() { return payment_; }
    double GetMainPart() { return main_fee_; }
    double GetRatePart() { return rate_fee_; }
    double GetLeftover() { return leftover_; }

   private:
    int month_;
    int year_;
    double payment_;
    double main_fee_;
    double rate_fee_;
    double leftover_;
  };

  enum Type { ANNUITY, DIFFERENTIAL };

  void SetAnnuity();
  void SetDifferential();
  void SetTime(int months);
  void SetRate(double rate);
  void SetCredit(double credit);
  void SetStartDate();
  void SetStartDate(int month, int year);

  Type GetType();
  int GetTime();
  double GetRate();
  double GetCredit();
  int GetStartMonth();
  int GetStartYear();
  double GetSummaryPaid();
  double GetSummaryMainPart();
  double GetSummaryRatePart();
  std::vector<CreditMonth>::iterator GetData();
  CreditMonth operator[](size_t pos);
  size_t GetDataSize();

  bool Calculate();

 private:
  const int MAX_TIME_ = 1000;
  const double MAX_RATE_ = 10.0;
  const double MAX_CREDIT_ = 100000000000.0;
  const int MAX_YEAR_ = 2700;
  const int MIN_YEAR_ = 1900;

  int start_month_ = 0, start_year_ = 0;
  double credit_ = 0.0;
  int time_ = 0;
  double rate_ = 0;
  Type payment_type_ = ANNUITY;
  double sum_paid_ = 0.0;
  double sum_main_paid_ = 0.0;
  double sum_rate_paid_ = 0.0;
  std::vector<CreditMonth> data_{};

  void Reset();
  bool Validate();
  bool ValidateCredit();
  bool ValidateRate();
  bool ValidateTime();
  bool ValidateStartDate();
  void CalculateAnnuity();
  void CalculateDifferential();
  void CalculateSummary();
  void RoundData();
  double BankRoundTwoDecimal(double number);
  double BankRound(double number);
};
}  // namespace s21

#endif  // S21_CREDIT_H
