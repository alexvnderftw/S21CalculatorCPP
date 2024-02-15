#include "s21_deposit.h"

namespace s21 {

/* Methods to set user variables. No bound checking. */

void Deposit::setDeposit(double value) noexcept { deposit_ = value; }
void Deposit::setTerm(int days) noexcept { term_ = days; }
void Deposit::setTermType(TermType value) noexcept { term_type_ = value; }
bool Deposit::setStartDate(int day, int month, int year) noexcept {
  if (Date::isDateValid(day, month, year) == false) return false;
  start_date_.setDate(day, month, year);
  return true;
}
bool Deposit::setStartDate(Date date) noexcept {
  start_date_ = date;
  return true;
}
void Deposit::setInterest(double value) noexcept { interest_ = value; }
void Deposit::setTax(double value) noexcept { tax_ = value; }
void Deposit::setCapitalization(bool set) noexcept { capital_ = set; }
void Deposit::setPeriodicity(PayPeriod value) noexcept { periodicity_ = value; }
void Deposit::setRemainderLimit(double value) noexcept {
  remainder_limit_ = value;
}
void Deposit::addReplenish(OperPeriod freq, Date date, double value) {
  replenish_list_.push_back(Operation(freq, date, value));
}
void Deposit::addWithdrawal(OperPeriod freq, Date date, double value) {
  withdrawal_list_.push_back(Operation(freq, date, value));
}
void Deposit::removeReplenish(size_t index) {
  replenish_list_.erase(replenish_list_.begin() + index);
}
void Deposit::removeWithdrawal(size_t index) {
  withdrawal_list_.erase(withdrawal_list_.begin() + index);
}
void Deposit::popBackReplenish() {
  if (getReplenishListSize() > 0) replenish_list_.pop_back();
}
void Deposit::popBackWithdrawal() {
  if (getWithdrawalListSize() > 0) withdrawal_list_.pop_back();
}
void Deposit::clearReplenish() { replenish_list_.clear(); }
void Deposit::clearWithdrawal() { withdrawal_list_.clear(); }

/* Methods to look at some user variables. No bound checking. */

bool Deposit::isCapitalization() const noexcept { return capital_; }
double Deposit::getDeposit() const noexcept { return deposit_; }
size_t Deposit::getReplenishListSize() const noexcept {
  return replenish_list_.size();
}
size_t Deposit::getWithdrawalListSize() const noexcept {
  return withdrawal_list_.size();
}
std::vector<Deposit::Operation>::const_iterator
Deposit::getReplenishListElement(size_t index) const noexcept {
  return replenish_list_.cbegin() + index;
}
std::vector<Deposit::Operation>::const_iterator
Deposit::getWithdrawalListElement(size_t index) const noexcept {
  return withdrawal_list_.cbegin() + index;
}

/* Methods to retrieve result content. */
size_t Deposit::getEventListSize() const noexcept { return event_list_.size(); }
std::vector<Deposit::Event>::const_iterator Deposit::getEventListElement(
    size_t index) const noexcept {
  return event_list_.cbegin() + index;
}
size_t Deposit::getTaxListSize() const noexcept { return tax_list_.size(); }

std::vector<Deposit::Tax>::const_iterator Deposit::getTaxListElement(
    size_t index) const noexcept {
  return tax_list_.cbegin() + index;
}
double Deposit::getBalance() const noexcept { return balance_; }
double Deposit::getInterestTotal() const noexcept { return interest_total_; }
double Deposit::getTaxTotal() const noexcept { return tax_total_; }
double Deposit::getReplenishTotal() const noexcept { return replenish_total_; }
double Deposit::getWithdrawalTotal() const noexcept {
  return withdrawal_total_;
}

/* Main method to run. Provides bound checking. */
/* Return false if user parameters are invalid.
  Weak part: function strictly rely on events order after sorting - some
  std::sort realization doesn't ensure that equal elements won't move, in that
  case output data can have wrong order and hence be calculated incorrectly.  */
bool Deposit::calculate() {
  if (validateSettings() == false) return false;
  // throw std::invalid_argument("Validation problem. Check bounds.");
  setDefaultValues();
  calculateEndDate();
  insertDeposit();
  insertReplenishList();
  insertWithdrawalList();
  insertPaydays();
  insertNewYears();
  insertLastPayday();

  std::sort(
      event_list_.begin(), event_list_.end(),
      [](Event first, Event second) { return first.date_ < second.date_; });

  /* Splice replenishments and withdrawals of the same day. Set 'false', if full
   * representation is needed. */
  fixEventList(true);

  calculateValues();
  return true;
}

/* Misc methods */

void Deposit::setDefaultValues() noexcept {
  event_list_.clear();
  tax_list_.clear();
  balance_ = 0.0;
  year_income_ = 0.0;
  interest_total_ = 0.0;
  tax_total_ = 0.0;
  withdrawal_total_ = 0.0;
  replenish_total_ = 0.0;
}

void Deposit::calculateEndDate() {
  end_date_ = start_date_;
  if (term_type_ == T_MONTH) {
    end_date_.addMonths(term_);
    // term_ = end_date_ | start_date_;
  } else if (term_type_ == T_YEAR) {
    end_date_.addYears(term_);
    // term_ = end_date_ | start_date_;
  } else {
    end_date_ += term_;
  }
}

// void Deposit::calculateValues() {
//   double day_value =
//       calculateDayValue(start_date_.getYear(),
//       bankRoundTwoDecimal(interest_));
//   // gain = day_value * n_days;
//   // n_days = pay_event | prev_event;
//   balance_ += event_list_[0].balance_change_;
//   event_list_[0].balance_ = balance_;
//   for (size_t i = 1; i < event_list_.size(); ++i) {
//     if (event_list_[i].event_ == E_REPLENISH) {
//       event_list_[i].gain_ = bankRoundFourDecimal(
//           day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
//           balance_);
//       balance_ += event_list_[i].balance_change_;
//       event_list_[i].balance_ = balance_;
//       replenish_total_ += event_list_[i].balance_change_;
//     } else if (event_list_[i].event_ == E_PAYDAY) {
//       event_list_[i].balance_ = balance_;
//       event_list_[i].gain_ = bankRoundFourDecimal(
//           day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
//           balance_);
//       event_list_[i].payment_ = event_list_[i].gain_;
//       for (size_t j = i - 1; event_list_[j].event_ != E_PAYDAY && j > 0; --j)
//       {
//         event_list_[i].payment_ += event_list_[j].gain_;
//         event_list_[i].gain_ += event_list_[j].gain_;
//         event_list_[j].gain_ = 0.0;
//       }
//       if (capital_ == true) {
//         event_list_[i].balance_change_ = event_list_[i].payment_;
//         event_list_[i].balance_ += event_list_[i].balance_change_;
//         balance_ = event_list_[i].balance_;
//         event_list_[i].payment_ = 0.0;
//       }
//       interest_total_ += event_list_[i].gain_;
//       year_income_ += event_list_[i].gain_;
//       if (event_list_[i].date_ == end_date_) {
//         tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
//                                 year_income_ * tax_));
//         tax_total_ += year_income_ * tax_;
//       }
//     } else if (event_list_[i].event_ == E_NEWYEAR) {
//       event_list_[i].balance_ = balance_;
//       event_list_[i].gain_ = bankRoundFourDecimal(
//           day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
//           balance_);
//       day_value =
//           calculateDayValue(event_list_[i].date_.getYear() + 1, interest_);
//       tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
//                               year_income_ * tax_));
//       tax_total_ += year_income_ * tax_;
//       year_income_ = 0.0;
//     } else if (event_list_[i].event_ == E_WITHDRAWAL) {
//       event_list_[i].balance_ = balance_;
//       event_list_[i].gain_ = bankRoundFourDecimal(
//           day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
//           balance_);
//       if (balance_ + event_list_[i].balance_change_ >= remainder_limit_) {
//         event_list_[i].balance_ += event_list_[i].balance_change_;
//         balance_ = event_list_[i].balance_;
//         withdrawal_total_ -= event_list_[i].balance_change_;
//       } else {
//         event_list_[i].event_ = E_DECLINE;
//       }
//     }
//   }
// }

void Deposit::calculateValues() {
  double day_value = calculateDayValue(start_date_.getYear(), interest_);
  // gain = day_value * n_days;
  // n_days = pay_event | prev_event;
  balance_ += bankRoundTwoDecimal(event_list_[0].balance_change_);
  event_list_[0].balance_ = bankRoundTwoDecimal(balance_);
  for (size_t i = 1; i < event_list_.size(); ++i) {
    if (event_list_[i].event_ == E_REPLENISH) {
      event_list_[i].gain_ = bankRoundTwoDecimal(
          day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
          balance_);
      balance_ += bankRoundTwoDecimal(event_list_[i].balance_change_);
      event_list_[i].balance_ = bankRoundTwoDecimal(balance_);
      replenish_total_ += bankRoundTwoDecimal(event_list_[i].balance_change_);
    } else if (event_list_[i].event_ == E_PAYDAY) {
      event_list_[i].balance_ = bankRoundTwoDecimal(balance_);
      event_list_[i].gain_ = bankRoundTwoDecimal(
          day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
          balance_);
      event_list_[i].payment_ = bankRoundTwoDecimal(event_list_[i].gain_);
      for (size_t j = i - 1; event_list_[j].event_ != E_PAYDAY && j > 0; --j) {
        event_list_[i].payment_ += bankRoundTwoDecimal(event_list_[j].gain_);
        event_list_[i].gain_ += bankRoundTwoDecimal(event_list_[j].gain_);
        event_list_[j].gain_ = 0.0;
      }
      if (capital_ == true) {
        event_list_[i].balance_change_ =
            bankRoundTwoDecimal(event_list_[i].payment_);
        event_list_[i].balance_ +=
            bankRoundTwoDecimal(event_list_[i].balance_change_);
        balance_ = bankRoundTwoDecimal(event_list_[i].balance_);
        event_list_[i].payment_ = 0.0;
      }
      interest_total_ += bankRoundTwoDecimal(event_list_[i].gain_);
      year_income_ += bankRoundTwoDecimal(event_list_[i].gain_);
      if (event_list_[i].date_ == end_date_) {
        tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
                                bankRoundTwoDecimal(year_income_ * tax_)));
        tax_total_ += bankRoundTwoDecimal(year_income_ * tax_);
      }
    } else if (event_list_[i].event_ == E_NEWYEAR) {
      event_list_[i].balance_ = bankRoundTwoDecimal(balance_);
      event_list_[i].gain_ = bankRoundTwoDecimal(
          day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
          balance_);
      day_value =
          calculateDayValue(event_list_[i].date_.getYear() + 1, interest_);
      tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
                              bankRoundTwoDecimal(year_income_ * tax_)));
      tax_total_ += bankRoundTwoDecimal(year_income_ * tax_);
      year_income_ = 0.0;
    } else if (event_list_[i].event_ == E_WITHDRAWAL) {
      event_list_[i].balance_ = bankRoundTwoDecimal(balance_);
      event_list_[i].gain_ = bankRoundTwoDecimal(
          day_value * (event_list_[i].date_ | event_list_[i - 1].date_) *
          balance_);
      if (balance_ + event_list_[i].balance_change_ >= remainder_limit_) {
        event_list_[i].balance_ +=
            bankRoundTwoDecimal(event_list_[i].balance_change_);
        balance_ = bankRoundTwoDecimal(event_list_[i].balance_);
        withdrawal_total_ -=
            bankRoundTwoDecimal(event_list_[i].balance_change_);
      } else {
        event_list_[i].event_ = E_DECLINE;
      }
    }
  }
}

// void Deposit::calculateValues() {
//   double day_value = calculateDayValue(start_date_.getYear(), interest_);
//   // gain = day_value * n_days;
//   // n_days = pay_event | prev_event;
//   balance_ += bankRoundTwoDecimal(event_list_[0].balance_change_);
//   event_list_[0].balance_ = balance_;
//   for (size_t i = 1; i < event_list_.size(); ++i) {
//     if (event_list_[i].event_ == E_REPLENISH) {
//       event_list_[i].gain_ = day_value *
//                              (event_list_[i].date_ | event_list_[i -
//                              1].date_) * balance_;
//       balance_ += bankRoundTwoDecimal(event_list_[i].balance_change_);
//       event_list_[i].balance_ = balance_;
//       replenish_total_ +=
//       bankRoundTwoDecimal(event_list_[i].balance_change_);
//     } else if (event_list_[i].event_ == E_PAYDAY) {
//       event_list_[i].balance_ = bankRoundTwoDecimal(balance_);
//       event_list_[i].gain_ = day_value *
//                              (event_list_[i].date_ | event_list_[i -
//                              1].date_) * balance_;
//       event_list_[i].payment_ = event_list_[i].gain_;
//       for (size_t j = i - 1; event_list_[j].event_ != E_PAYDAY && j > 0; --j)
//       {
//         event_list_[i].payment_ += bankRoundTwoDecimal(event_list_[j].gain_);
//         event_list_[i].gain_ += bankRoundTwoDecimal(event_list_[j].gain_);
//         event_list_[j].gain_ = 0.0;
//       }
//       if (capital_ == true) {
//         event_list_[i].balance_change_ = event_list_[i].payment_;
//         event_list_[i].balance_ +=
//             bankRoundTwoDecimal(event_list_[i].balance_change_);
//         balance_ = event_list_[i].balance_;
//         event_list_[i].payment_ = 0.0;
//       }
//       interest_total_ += bankRoundTwoDecimal(event_list_[i].gain_);
//       if (event_list_[i].date_ == end_date_) {
//         year_income_ = bankRoundTwoDecimal(interest_total_) -
//                        bankRoundTwoDecimal(year_income_);
//         tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
//                                 year_income_ * tax_));
//         tax_total_ += bankRoundTwoDecimal(year_income_ * tax_);
//       }
//     } else if (event_list_[i].event_ == E_NEWYEAR) {
//       event_list_[i].balance_ = balance_;
//       event_list_[i].gain_ = day_value *
//                              (event_list_[i].date_ | event_list_[i -
//                              1].date_) * balance_;
//       day_value =
//           calculateDayValue(event_list_[i].date_.getYear() + 1, interest_);
//       year_income_ = bankRoundTwoDecimal(interest_total_) -
//                      bankRoundTwoDecimal(year_income_);
//       tax_list_.push_back(Tax(event_list_[i].date_.getYear(), year_income_,
//                               year_income_ * tax_));
//       tax_total_ += bankRoundTwoDecimal(year_income_ * tax_);
//     } else if (event_list_[i].event_ == E_WITHDRAWAL) {
//       event_list_[i].balance_ = balance_;
//       event_list_[i].gain_ = day_value *
//                              (event_list_[i].date_ | event_list_[i -
//                              1].date_) * balance_;
//       if (balance_ + event_list_[i].balance_change_ >= remainder_limit_) {
//         event_list_[i].balance_ +=
//             bankRoundTwoDecimal(event_list_[i].balance_change_);
//         balance_ = event_list_[i].balance_;
//         withdrawal_total_ -=
//             bankRoundTwoDecimal(event_list_[i].balance_change_);
//       } else {
//         event_list_[i].event_ = E_DECLINE;
//       }
//     }
//   }
// }

/* Splice replenishments and withdrawals of the same day in event_list_. */
/* Swap payday and new_year events of same day so new_year goes after payday. */
void Deposit::fixEventList(bool splice_on) {
  for (size_t i = 1; i < event_list_.size() - 1; ++i) {
    if (splice_on == true) {
      if (event_list_[i].date_ == event_list_[i + 1].date_ &&
          (event_list_[i].event_ == E_REPLENISH ||
           event_list_[i].event_ == E_WITHDRAWAL) &&
          (event_list_[i + 1].event_ == E_REPLENISH ||
           event_list_[i + 1].event_ == E_WITHDRAWAL)) {
        spliceOperations(i, i + 1);
        --i;
      }
    }
    if (event_list_[i].date_ == event_list_[i + 1].date_ &&
        event_list_[i].event_ == E_NEWYEAR &&
        event_list_[i + 1].event_ == E_PAYDAY) {
      swapEvents(i, i + 1);
    }
  }
}

void Deposit::spliceOperations(size_t first, size_t second) {
  event_list_[first].balance_change_ += event_list_[second].balance_change_;
  if (event_list_[first].balance_change_ < 0)
    event_list_[first].event_ = E_WITHDRAWAL;
  else
    event_list_[first].event_ = E_REPLENISH;
  event_list_.erase(event_list_.begin() + second);
}

void Deposit::swapEvents(size_t first, size_t second) noexcept {
  Event buffer = event_list_[first];
  event_list_[first] = event_list_[second];
  event_list_[second] = buffer;
}

void Deposit::insertDeposit() {
  event_list_.push_back(
      Event(E_REPLENISH, start_date_, 0.0, deposit_, 0.0, 0.0));
}

void Deposit::insertNewYears() {
  for (int i = 0; i < end_date_.getYear() - start_date_.getYear(); i++) {
    event_list_.push_back(Event(E_NEWYEAR,
                                Date(31, 12, start_date_.getYear() + i), 0.0,
                                0.0, 0.0, 0.0));
  }
}

void Deposit::insertReplenishList() {
  for (size_t i = 0; i < replenish_list_.size(); ++i) {
    insertReplenish(i);
  }
}

void Deposit::insertWithdrawalList() {
  for (size_t i = 0; i < withdrawal_list_.size(); ++i) {
    insertWithdrawal(i);
  }
}

void Deposit::insertPaydays() {
  if (periodicity_ == P_DAILY) {
    Date current = start_date_ + 1;
    for (; current < end_date_; current++) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  } else if (periodicity_ == P_WEEKLY) {
    Date current = start_date_ + 7;
    for (; current < end_date_; current += 7) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  } else if (periodicity_ == P_MONTHLY) {
    Date current = nextMonthDate(start_date_);
    for (; current < end_date_; current = nextMonthDate(current)) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  } else if (periodicity_ == P_QUARTERLY) {
    Date current = start_date_;
    current.addMonths(3);
    for (; current < end_date_; current.addMonths(3)) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  } else if (periodicity_ == P_BIANNUALLY) {
    Date current = start_date_;
    current.addMonths(6);
    for (; current < end_date_; current.addMonths(6)) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  } else if (periodicity_ == P_ANNUALLY) {
    Date current = start_date_;
    current.addYears(1);
    for (; current < end_date_; current.addYears(1)) {
      event_list_.push_back(Event(E_PAYDAY, current, 0.0, 0.0, 0.0, 0.0));
    }
  }
}

void Deposit::insertLastPayday() {
  event_list_.push_back(Event(E_PAYDAY, end_date_, 0.0, 0.0, 0.0, 0.0));
}

void Deposit::insertReplenish(size_t i) {
  Date start = replenish_list_[i].date_;
  Date current = start;
  if (replenish_list_[i].period_ == O_ONCE) {
    if (current > start_date_ && current < end_date_)
      event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                  replenish_list_[i].value_, 0.0, 0.0));
  } else if (replenish_list_[i].period_ == O_MONTHLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current <= end_date_; ++j) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_BIMONTHLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 2; current <= end_date_; j += 2) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_QUARTERLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 3; current <= end_date_; j += 3) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_BIANNUALLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 6; current <= end_date_; j += 6) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_ANNUALLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 12; current <= end_date_; j += 12) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  }
}

void Deposit::insertWithdrawal(size_t i) {
  Date start = withdrawal_list_[i].date_;
  Date current = start;
  if (withdrawal_list_[i].period_ == O_ONCE) {
    if (current > start_date_ && current < end_date_)
      event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                  -withdrawal_list_[i].value_, 0.0, 0.0));
  } else if (withdrawal_list_[i].period_ == O_MONTHLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current <= end_date_; ++j) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_BIMONTHLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 2; current <= end_date_; j += 2) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_QUARTERLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 3; current <= end_date_; j += 3) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_BIANNUALLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 6; current <= end_date_; j += 6) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_ANNUALLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 12; current <= end_date_; j += 12) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current = start.shiftMonths(j);
    }
  }
}

bool Deposit::validateSettings() const noexcept {
  return checkReplenishes() && checkWithdrawals() &&
         checkPositiveDouble(deposit_) && checkPositiveDouble(interest_) &&
         checkPositiveDouble(tax_) && checkPositiveDouble(remainder_limit_) &&
         deposit_ <= MAX_DEPOSIT_VALUE && interest_ <= MAX_RATE &&
         tax_ <= MAX_TAX && checkDates();
}

bool Deposit::checkReplenishes() const noexcept {
  for (size_t i = 0; i < replenish_list_.size(); ++i) {
    if (replenish_list_[i].date_.getYear() > MAX_START_YEAR ||
        replenish_list_[i].value_ > MAX_DEPOSIT_VALUE ||
        !checkPositiveDouble(replenish_list_[i].value_))
      return false;
  }
  return true;
}

bool Deposit::checkWithdrawals() const noexcept {
  for (size_t i = 0; i < withdrawal_list_.size(); ++i) {
    if (withdrawal_list_[i].date_.getYear() > MAX_START_YEAR ||
        withdrawal_list_[i].value_ > MAX_DEPOSIT_VALUE ||
        !checkPositiveDouble(withdrawal_list_[i].value_))
      return false;
  }
  return true;
}

bool Deposit::checkPositiveDouble(double value) const noexcept {
  return value >= 0.0 && !std::isnan(value) && !std::isinf(value) &&
         value < MAX_DOUBLE_VALUE;
}

bool Deposit::checkDates() const noexcept {
  return term_ > 0 &&
         ((term_type_ == T_YEAR && term_ <= MAX_TERM_Y) ||
          (term_type_ == T_MONTH && term_ <= MAX_TERM_M) ||
          (term_type_ == T_DAY && term_ <= MAX_TERM_D)) &&
         start_date_.getYear() <= MAX_START_YEAR;
}

Date Deposit::nextMonthDate(Date date) {
  Date ret;
  if (date.getDay() >= 29) {
    ret = date + 10;
    if (!Date::isDateValid(date.getDay(), ret.getMonth(), ret.getYear())) {
      ret = date + 31;
      return ret;
    } else {
      ret = date + 31;
      ret.setDate(date.getDay(), ret.getMonth(), ret.getYear());
      return ret;
    }
  } else { /* if (date.getDay() < 29) */
    ret = date + 31;
    ret.setDate(date.getDay(), ret.getMonth(), ret.getYear());
    return ret;
  }
}

double Deposit::calculateDayValue(int year, double rate) noexcept {
  if (isLeapYear(year) == false) {
    return rate / 365.0;
  } else {
    return rate / 366.0;
  }
}

bool Deposit::isLeapYear(int year) noexcept {
  if (year % 400 == 0) return true;
  if (year % 100 == 0) return false;
  if (year % 4 == 0) return true;
  return false;
}

bool Deposit::dateComparator(Event first, Event second) noexcept {
  return first.date_ > second.date_;
}

}  // namespace s21
