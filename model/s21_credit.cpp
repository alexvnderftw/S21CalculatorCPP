#include "s21_credit.h"

namespace s21 {

void Credit::SetAnnuity() { payment_type_ = ANNUITY; }
void Credit::SetDifferential() { payment_type_ = DIFFERENTIAL; }
void Credit::SetTime(int months) { time_ = months; }
void Credit::SetRate(double rate) { rate_ = rate; }
void Credit::SetCredit(double credit) { credit_ = credit; }

double Credit::GetSummaryPaid() { return sum_paid_; }
double Credit::GetSummaryMainPart() { return sum_main_paid_; }
double Credit::GetSummaryRatePart() { return sum_rate_paid_; }
Credit::CreditMonth Credit::operator[](size_t pos) { return data_[pos]; }
size_t Credit::GetDataSize() { return data_.size(); }
std::vector<Credit::CreditMonth>::iterator Credit::GetData() {
  return data_.begin();
}

/* Main function. Call it after setting all values.
If all data correct it fills data vector and summary, and returns true.
If some data incorrect returns false. */
bool Credit::Calculate() {
  if (Validate() == true) {
    Reset();
    SetStartDate();
    data_.resize(time_);
    if (payment_type_ == ANNUITY)
      CalculateAnnuity();
    else
      CalculateDifferential();
    return true;
  }
  return false;
}

/* Misc */

void Credit::Reset() {
  sum_paid_ = 0.0;
  sum_main_paid_ = 0.0;
  sum_rate_paid_ = 0.0;
  data_.clear();
}

bool Credit::Validate() {
  return ValidateCredit() && ValidateRate() && ValidateTime();
}

bool Credit::ValidateCredit() {
  return credit_ >= 0.01 && credit_ < 1000000000 && !std::isnan(credit_);
}

bool Credit::ValidateRate() {
  return rate_ >= 0.0001 && rate_ < 10.0 && !std::isnan(rate_);
}

bool Credit::ValidateTime() { return time_ >= 1 && time_ <= 600; }

void Credit::CalculateAnnuity() {
  data_[0].month_ = start_month_;
  data_[0].year_ = start_year_;
  data_[0].rate_fee_ = BankRoundTwoDecimal(credit_ * rate_ / 12.0);
  data_[0].payment_ = BankRoundTwoDecimal(
      credit_ * rate_ / 12.0 /
      (1.0 - std::pow(1.0 + rate_ / 12.0, -static_cast<double>(time_))));
  data_[0].main_fee_ =
      BankRoundTwoDecimal(data_[0].payment_ - data_[0].rate_fee_);
  data_[0].leftover_ = BankRoundTwoDecimal(credit_ - data_[0].main_fee_);

  for (int i = 1; i < time_; ++i) {
    data_[i].month_ = data_[i - 1].month_ + 1;
    data_[i].year_ = data_[i - 1].year_;
    if (data_[i].month_ > 12) {
      data_[i].month_ = 1;
      data_[i].year_++;
    }
    data_[i].payment_ = data_[0].payment_;
    data_[i].rate_fee_ =
        BankRoundTwoDecimal(data_[i - 1].leftover_ * rate_ / 12.0);
    data_[i].main_fee_ =
        BankRoundTwoDecimal(data_[i].payment_ - data_[i].rate_fee_);
    data_[i].leftover_ =
        BankRoundTwoDecimal(data_[i - 1].leftover_ - data_[i].main_fee_);
  }
  CalculateSummary();
}

void Credit::CalculateDifferential() {
  data_[0].month_ = start_month_;
  data_[0].year_ = start_year_;
  data_[0].main_fee_ = credit_ / time_;
  data_[0].payment_ = data_[0].main_fee_ + credit_ * rate_ / 12.0;
  data_[0].rate_fee_ = data_[0].payment_ - data_[0].main_fee_;
  data_[0].leftover_ = credit_ - data_[0].main_fee_;

  for (int i = 1; i < time_; ++i) {
    data_[i].month_ = data_[i - 1].month_ + 1;
    data_[i].year_ = data_[i - 1].year_;
    if (data_[i].month_ > 12) {
      data_[i].month_ = 1;
      data_[i].year_++;
    }
    data_[i].main_fee_ = data_[0].main_fee_;
    data_[i].payment_ =
        data_[0].main_fee_ + (credit_ - data_[0].main_fee_ * i) * rate_ / 12.0;
    data_[i].rate_fee_ = data_[i].payment_ - data_[i].main_fee_;
    data_[i].leftover_ = data_[i - 1].leftover_ - data_[i].main_fee_;
  }
  CalculateSummary();
  RoundData();
}

void Credit::CalculateSummary() {
  for (int i = 0; i < time_; ++i) {
    sum_paid_ += data_[i].payment_;
    sum_main_paid_ += data_[i].main_fee_;
    sum_rate_paid_ += data_[i].rate_fee_;
  }
}

void Credit::RoundData() {
  for (int i = 0; i < time_; ++i) {
    data_[i].main_fee_ = BankRoundTwoDecimal(data_[i].main_fee_);
    data_[i].payment_ = BankRoundTwoDecimal(data_[i].payment_);
    data_[i].rate_fee_ = BankRoundTwoDecimal(data_[i].rate_fee_);
    data_[i].leftover_ = BankRoundTwoDecimal(data_[i].leftover_);
  }
  sum_paid_ = BankRoundTwoDecimal(sum_paid_);
  sum_main_paid_ = BankRoundTwoDecimal(sum_main_paid_);
  sum_rate_paid_ = BankRoundTwoDecimal(sum_rate_paid_);
}

void Credit::SetStartDate() {
  time_t t = std::time(NULL);
  struct tm time_data = *localtime(&t);
  start_month_ = time_data.tm_mon + 1;
  start_year_ = time_data.tm_year + 1900;
}

/* Bank rounding of number to number with two decimal digits. */
double Credit::BankRoundTwoDecimal(double number) {
  return BankRound(number * 100.0) / 100.0;
}

/* Regular bank round of double number to integer value. */
double Credit::BankRound(double number) {
  if (number >= 0 && number - std::floor(number) == 0.5) {
    if (std::fmod(std::fmod(std::floor(number), 10.0), 2.0) == 0.0)
      number = std::floor(number);
    else
      number = std::ceil(number);
  } else if (number < 0 && number - std::floor(number) == 0.5) {
    if (std::fmod(std::fmod(std::ceil(number), 10.0), 2.0) == 0.0)
      number = std::ceil(number);
    else
      number = std::floor(number);
  } else {
    number = std::round(number);
  }
  return number;
}

}  // namespace s21