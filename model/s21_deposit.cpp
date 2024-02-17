#include "s21_deposit.h"

namespace s21 {

/* Methods to set user variables. No bound checking. */

void Deposit::setDeposit(double value) noexcept { deposit_ = value; }
void Deposit::setTerm(int days) noexcept { term_ = days; }
void Deposit::setTermType(TermType value) noexcept { term_type_ = value; }
bool Deposit::setStartDate() {
  Date current;
  start_date_ = current;
  return true;
}
bool Deposit::setStartDate(int day, int month, int year) {
  if (Date::isDateValid(day, month, year) == false) return false;
  start_date_.setDate(day, month, year);
  return true;
}
bool Deposit::setStartDate(Date& date) noexcept {
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
void Deposit::removeReplenish(size_t index) noexcept {
  replenish_list_.erase(replenish_list_.begin() + index);
}
void Deposit::removeWithdrawal(size_t index) noexcept {
  withdrawal_list_.erase(withdrawal_list_.begin() + index);
}
void Deposit::popBackReplenish() noexcept {
  if (getReplenishListSize() > 0) replenish_list_.pop_back();
}
void Deposit::popBackWithdrawal() noexcept {
  if (getWithdrawalListSize() > 0) withdrawal_list_.pop_back();
}
void Deposit::clearReplenish() noexcept { replenish_list_.clear(); }
void Deposit::clearWithdrawal() noexcept { withdrawal_list_.clear(); }

/* Methods to look at some user variables. No bound checking. */

bool Deposit::isCapitalization() const noexcept { return capital_; }
double Deposit::getDeposit() const noexcept { return deposit_; }
double Deposit::getRemainderLimit() const noexcept { return remainder_limit_; }
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
  setDefaultValues();
  calculateEndDate();
  insertDeposit();
  insertReplenishList();
  insertWithdrawalList();
  insertPaydays();
  insertNewYears();
  insertLastPayday();

  std::sort(event_list_.begin(), event_list_.end(), dateComparator);

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
  } else if (term_type_ == T_YEAR) {
    end_date_ = nextMonthDate(end_date_, term_ * 12);
  } else {
    end_date_ += term_;
  }
}

void Deposit::calculateValues() {
  countDayValue(start_date_.getYear());
  for (std::vector<Event>::iterator i = event_list_.begin();
       i != event_list_.end(); ++i) {
    switch (i->event_) {
      case E_REPLENISH:
        countReplenish(i);
        break;
      case E_WITHDRAWAL:
        countWithdrawal(i);
        break;
      case E_NEWYEAR:
        countNewyear(i);
        break;
      case E_PAYDAY:
        countPayday(i);
        break;

      default:
        break;
    }
  }
}

void Deposit::countReplenish(std::vector<Event>::iterator i) noexcept {
  countGain(i);
  countBalance(i);
  if (i != event_list_.begin()) replenish_total_ += i->balance_change_;
}

void Deposit::countWithdrawal(std::vector<Event>::iterator i) noexcept {
  countGain(i);
  if (balance_ + i->balance_change_ >= remainder_limit_) {
    countBalance(i);
    withdrawal_total_ -= i->balance_change_;
  } else {
    i->balance_ = balance_;
    i->event_ = E_DECLINE;
  }
}

void Deposit::countNewyear(std::vector<Event>::iterator i) {
  countGain(i);
  countBalance(i);
  countDayValue(i->date_.getYear() + 1);
  if (i->date_ != start_date_) countTax(i);
}

void Deposit::countPayday(std::vector<Event>::iterator i) {
  countGain(i);
  sumPreviousGains(i);
  if (capital_ == true)
    i->balance_change_ = i->gain_;
  else
    i->payment_ = i->gain_;
  countBalance(i);
  interest_total_ += i->gain_;
  year_income_ += i->gain_;
  if (i->date_ == end_date_) {
    countTax(i);
  }
}

void Deposit::countBalance(std::vector<Event>::iterator i) noexcept {
  balance_ += i->balance_change_;
  i->balance_ = balance_;
}

/* CAREFUL. It looks back to previous element. */
void Deposit::countGain(std::vector<Event>::iterator i) noexcept {
  if (i > event_list_.begin())
    i->gain_ = day_value_ * (i->date_ | (i - 1)->date_) * balance_;
}

void Deposit::sumPreviousGains(std::vector<Event>::iterator i) noexcept {
  for (std::vector<Event>::iterator j = i - 1;
       j->event_ != E_PAYDAY && j != event_list_.begin(); --j) {
    i->gain_ += j->gain_;
    j->gain_ = 0.0L;
  }
}

void Deposit::countTax(std::vector<Event>::iterator i) {
  long double gain = year_income_ * tax_;
  tax_total_ += gain;
  tax_list_.push_back(Tax(i->date_.getYear(), year_income_, gain));
  year_income_ = 0.0L;
}

void Deposit::countDayValue(int year) noexcept {
  day_value_ = calculateDayValue(year, interest_);
}

/* Splice replenishments and withdrawals of the same day in event_list_. */
/* Swap payday and new_year events of same day so new_year goes after payday. */
void Deposit::fixEventList(bool splice_on) noexcept {
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
    /* Purpose of the commented code below is to correct std::sort job in case
     * of inconsistencies. Turn it on if std::sort messes things up in regard to
     * Events with same date.  */
    /* if (event_list_[i].date_ == event_list_[i + 1].date_ &&
        event_list_[i].event_ == E_NEWYEAR &&
        event_list_[i + 1].event_ == E_PAYDAY)
      swapEvents(i, i + 1);
    if (event_list_[i].date_ == event_list_[i + 1].date_ &&
        event_list_[i].event_ == E_PAYDAY &&
        (event_list_[i + 1].event_ == E_PAYDAY ||
         event_list_[i + 1].event_ == E_WITHDRAWAL))
      swapEvents(i, i + 1); */
  }
}

void Deposit::spliceOperations(size_t first, size_t second) noexcept {
  event_list_[first].balance_change_ += event_list_[second].balance_change_;
  if (event_list_[first].balance_change_ < 0.0L)
    event_list_[first].event_ = E_WITHDRAWAL;
  else
    event_list_[first].event_ = E_REPLENISH;
  event_list_.erase(event_list_.begin() + second);
}

void Deposit::insertDeposit() { pushEvent(E_REPLENISH, start_date_, deposit_); }

void Deposit::insertNewYears() {
  for (int i = 0; i < end_date_.getYear() - start_date_.getYear(); i++) {
    pushEvent(E_NEWYEAR, Date(31, 12, start_date_.getYear() + i));
  }
}

void Deposit::insertReplenishList() {
  for (size_t i = 0; i < replenish_list_.size(); ++i) {
    insertOperation(replenish_list_.cbegin() + i, E_REPLENISH);
  }
}

void Deposit::insertWithdrawalList() {
  for (size_t i = 0; i < withdrawal_list_.size(); ++i) {
    insertOperation(withdrawal_list_.cbegin() + i, E_WITHDRAWAL);
  }
}

void Deposit::insertPaydays() {
  switch (periodicity_) {
    case P_DAILY:
      pushPaydaysSkipDays(1);
      break;
    case P_WEEKLY:
      pushPaydaysSkipDays(7);
      break;
    case P_MONTHLY:
      pushPaydaysSkipMonths(1);
      break;
    case P_QUARTERLY:
      pushPaydaysSkipMonths(3);
      break;
    case P_BIANNUALLY:
      pushPaydaysSkipMonths(6);
      break;
    case P_ANNUALLY:
      pushPaydaysSkipMonths(12);
      break;

    default:
      break;
  }
}

void Deposit::insertLastPayday() { pushEvent(E_PAYDAY, end_date_); }

/* Dates of operations and payments seems to be produced in unstable manner
 * date-wise on calcus.ru when day is 29-31. The method is chosen depends on
 * calcul.ru behaviour with different periodicity of operation (2 methods to
 * skip months). */
void Deposit::insertOperation(std::vector<Operation>::const_iterator element,
                              EventType event) {
  Date date = element->date_;
  switch (element->period_) {
    case O_ONCE:
      if (date > start_date_ && date <= end_date_)
        pushEvent(event, date, element->value_);
      break;
    case O_MONTHLY:
      pushOperationsShiftMonths(event, date, element->value_, 1);
      break;
    case O_BIMONTHLY:
      pushOperationsShiftMonths(event, date, element->value_, 2);
      break;
    case O_QUARTERLY:
      pushOperationsShiftMonths(event, date, element->value_, 3);
      break;
    case O_BIANNUALLY:
      pushOperationsShiftMonths(event, date, element->value_, 6);
      break;
    case O_ANNUALLY:
      pushOperationsShiftMonthsWithOvercap(event, date, element->value_, 12);
      break;

    default:
      break;
  }
}

/* Build dates relatively to start date, cut days if needed.
Example: 31.01.2024 + 1 month = 29.02.2024, 31.03.2024, 30.04.2024, etc. */
void Deposit::pushOperationsShiftMonths(EventType event, Date date,
                                        long double value, size_t step) {
  Date start = date;
  for (size_t j = step; date <= end_date_; j += step) {
    if (date > start_date_) pushEvent(event, date, value);
    date = start.shiftMonths(j);
  }
}

/* Build dates relatively to current date with possibility of overcapping by
1-3 days. Current date value is always reassigned. Example: 29.02.2024 + 12
months = 01.03.2025, ... , 01.03.2028, etc. */
void Deposit::pushOperationsShiftMonthsWithOvercap(EventType event, Date date,
                                                   long double value,
                                                   size_t step) {
  while (date <= end_date_) {
    if (date > start_date_) pushEvent(event, date, value);
    date = nextMonthDate(date, step);
  }
}

void Deposit::pushPaydaysSkipMonths(int step) {
  Date current = nextMonthDate(start_date_, step);
  for (; current < end_date_; current = nextMonthDate(current, step)) {
    pushEvent(E_PAYDAY, current);
  }
}

void Deposit::pushPaydaysSkipDays(int step) {
  Date current = start_date_ + step;
  for (; current < end_date_; current += step) {
    pushEvent(E_PAYDAY, current);
  }
}

void Deposit::pushEvent(EventType event, Date date, long double change) {
  if (event == E_WITHDRAWAL) change = -change;
  event_list_.push_back(Event(event, date, 0.0L, change, 0.0L, 0.0L));
}

void Deposit::swapEvents(size_t first, size_t second) noexcept {
  Event buffer = event_list_[first];
  event_list_[first] = event_list_[second];
  event_list_[second] = buffer;
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

Date Deposit::nextMonthDate(Date& date, int n) {
  Date ret = date;
  int days = date.getDay();
  if (date.getDay() >= 29) {
    ret.addMonths(n);
    if (!Date::isDateValid(date.getDay(), ret.getMonth(), ret.getYear())) {
      ret.setDate(1, ret.getMonth(), ret.getYear());
      ret += days - 1;
      return ret;
    } else {
      ret.setDate(date.getDay(), ret.getMonth(), ret.getYear());
      return ret;
    }
  } else { /* if (date.getDay() < 29) */
    ret.addMonths(n);
    return ret;
  }
}

long double Deposit::calculateDayValue(int year, long double rate) noexcept {
  if (isLeapYear(year) == false) {
    return rate / 365.0L;
  } else {
    return rate / 366.0L;
  }
}

bool Deposit::isLeapYear(int year) noexcept {
  if (year % 400 == 0) return true;
  if (year % 100 == 0) return false;
  if (year % 4 == 0) return true;
  return false;
}

bool Deposit::dateComparator(Event& first, Event& second) noexcept {
  return first.date_ < second.date_;
}

}  // namespace s21
