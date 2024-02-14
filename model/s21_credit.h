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

    int GetMonth() const noexcept { return month_; }
    int GetYear() const noexcept { return year_; }
    double GetPayment() const noexcept { return payment_; }
    double GetMainPart() const noexcept { return main_fee_; }
    double GetRatePart() const noexcept { return rate_fee_; }
    double GetLeftover() const noexcept { return leftover_; }

   private:
    int month_;
    int year_;
    double payment_;
    double main_fee_;
    double rate_fee_;
    double leftover_;
  };

  enum Type { ANNUITY, DIFFERENTIAL };

  /* Set methods */
  void SetAnnuity() noexcept;
  void SetDifferential() noexcept;
  void SetTime(int months) noexcept;
  void SetRate(double rate) noexcept;
  void SetCredit(double credit) noexcept;
  void SetStartDate();
  void SetStartDate(int month, int year);

  /* Get methods */
  Type GetType() const noexcept;
  int GetTime() const noexcept;
  double GetRate() const noexcept;
  double GetCredit() const noexcept;
  int GetStartMonth() const noexcept;
  int GetStartYear() const noexcept;
  double GetSummaryPaid() const noexcept;
  double GetSummaryMainPart() const noexcept;
  double GetSummaryRatePart() const noexcept;
  size_t GetDataSize() const noexcept;
  std::vector<CreditMonth>::const_iterator GetData() const noexcept;
  CreditMonth operator[](size_t pos) const noexcept;

  /* Main function */
  bool Calculate();

 private:
  const int MAX_TIME_ = 1000, MAX_YEAR_ = 2700, MIN_YEAR_ = 1900;
  const double MAX_RATE_ = 10.0, MAX_CREDIT_ = 100000000000.0;

  int start_month_ = 0, start_year_ = 0;
  double credit_ = 0.0;
  int time_ = 0;
  double rate_ = 0;
  Type payment_type_ = ANNUITY;
  double sum_paid_ = 0.0;
  double sum_main_paid_ = 0.0;
  double sum_rate_paid_ = 0.0;
  std::vector<CreditMonth> data_{};

  void Reset() noexcept;
  bool Validate() const noexcept;
  bool ValidateCredit() const noexcept;
  bool ValidateRate() const noexcept;
  bool ValidateTime() const noexcept;
  bool ValidateStartDate() const noexcept;
  void CalculateAnnuity() noexcept;
  void CalculateDifferential() noexcept;
  void CalculateSummary() noexcept;
  void RoundData() noexcept;
  static double BankRoundTwoDecimal(double number) noexcept;
};
}  // namespace s21

#endif  // S21_CREDIT_H
