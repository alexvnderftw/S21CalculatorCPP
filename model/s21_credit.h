#ifndef S21_CREDIT_H
#define S21_CREDIT_H

#include <cmath>
#include <vector>

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

  Type GetType() { return payment_type_; }
  int GetTime() { return time_; }
  double GetRate() { return rate_; }
  double GetCredit() { return credit_; }
  double GetSummaryPaid();
  double GetSummaryMainPart();
  double GetSummaryRatePart();
  std::vector<CreditMonth>::iterator GetData();
  CreditMonth operator[](size_t pos);
  size_t GetDataSize();

  bool Calculate();

 private:
  int start_month_, start_year_;
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
  void CalculateAnnuity();
  void CalculateDifferential();
  void CalculateSummary();
  void RoundData();
  void SetStartDate();
  double BankRoundTwoDecimal(double number);
  double BankRound(double number);
};
}  // namespace s21

#endif  // S21_CREDIT_H
