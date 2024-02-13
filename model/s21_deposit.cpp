#include "s21_deposit.h"

namespace s21 {

void Deposit::setDeposit(double value) { deposit_ = value; }
void Deposit::setTerm(int days) { term_ = days; }
void Deposit::setTermType(TermType value) { term_type_ = value; }
void Deposit::setStartDate(int day, int month, int year) {
  start_date_.setDate(day, month, year);
}
void Deposit::setInterest(double value) { interest_ = value; }
void Deposit::setTax(double value) { tax_ = value; }
void Deposit::setCapitalization(bool set) { capital_ = set; }
void Deposit::setPeriodicity(PayPeriod value) { periodicity_ = value; }

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
size_t Deposit::getReplenishListSize() { return replenish_list_.size(); }
size_t Deposit::getWIthdrawalListSize() { return withdrawal_list_.size(); }
std::vector<Deposit::Operation>::const_iterator
Deposit::getReplenishListElement(size_t index) {
  return replenish_list_.cbegin() + index;
}
std::vector<Deposit::Operation>::const_iterator
Deposit::getWithdrawalListElement(size_t index) {
  return withdrawal_list_.cbegin() + index;
}

size_t Deposit::getEventListSize() { return event_list_.size(); }
std::vector<Deposit::Event>::const_iterator Deposit::getEventListElement(
    size_t index) {
  return event_list_.cbegin() + index;
}
// // remove

// /* Calcualate */
// void calculate();
// size_t getEventListSize();
// std::vector<Operation>::const_iterator getEventListElement(size_t index);
void Deposit::calculate() {
  event_list_.clear();
  calculateTerm();
  insertDeposit();
  insertPaydays();
  insertReplenishList();
  insertWithdrawalList();
  // insertTaxes();
  insertLastPayday();

  std::sort(
      event_list_.begin(), event_list_.end(),
      [](Event first, Event second) { return first.date_ < second.date_; });

  // interest_total_
  // balance_
  balance_ = deposit_;
  for (size_t i = 0; i < event_list_.size(); ++i) {
    // double income = calculatePeriodIncome();
  }
}

bool Deposit::dateComparator(Event first, Event second) {
  return first.date_ > second.date_;
}

/* Misc */
void Deposit::insertDeposit() {
  event_list_.push_back(
      Event(E_REPLENISH, start_date_, 0.0, deposit_, 0.0, deposit_));
}

void Deposit::insertLastPayday() {
  event_list_.push_back(
      Event(E_PAYDAY, start_date_ + term_, 0.0, 0.0, 0.0, 0.0));
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

void Deposit::insertTaxes() {}

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

void Deposit::insertReplenish(size_t i) {
  Date current = replenish_list_[i].date_;
  if (replenish_list_[i].period_ == O_ONCE) {
    if (current > start_date_ && current < end_date_)
      event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                  replenish_list_[i].value_, 0.0, 0.0));
  } else if (replenish_list_[i].period_ == O_MONTHLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current < end_date_; ++j) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_BIMONTHLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 2) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_QUARTERLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 3) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_BIANNUALLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 6) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (replenish_list_[i].period_ == O_ANNUALLY) {
    current = replenish_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 12) {
      if (current > start_date_)
        event_list_.push_back(Event(E_REPLENISH, current, 0.0,
                                    replenish_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  }
}

void Deposit::insertWithdrawal(size_t i) {
  Date current = withdrawal_list_[i].date_;
  if (withdrawal_list_[i].period_ == O_ONCE) {
    if (current > start_date_ && current < end_date_)
      event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                  -withdrawal_list_[i].value_, 0.0, 0.0));
  } else if (withdrawal_list_[i].period_ == O_MONTHLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current < end_date_; ++j) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_BIMONTHLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 2) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_QUARTERLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 3) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_BIANNUALLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 6) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  } else if (withdrawal_list_[i].period_ == O_ANNUALLY) {
    current = withdrawal_list_[i].date_;
    for (size_t j = 1; current < end_date_; j += 12) {
      if (current > start_date_)
        event_list_.push_back(Event(E_WITHDRAWAL, current, 0.0,
                                    -withdrawal_list_[i].value_, 0.0, 0.0));
      current.addMonths(j);
    }
  }
}  // including DECLINEs

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
  } else {  // if (date.getDay() < 29)
    ret = date + 31;
    ret.setDate(date.getDay(), ret.getMonth(), ret.getYear());
    return ret;
  }
}

double Deposit::calculateDayValue(int year, double rate) {
  if (isLeapYear(year) == false) {
    return rate / 365.0;
  } else {
    return rate / 366.0;
  }
}

bool Deposit::isLeapYear(int year) {
  if (year % 400 == 0) return true;
  if (year % 100 == 0) return false;
  if (year % 4 == 0) return true;
  return false;
}

void Deposit::calculateTerm() {
  end_date_ = start_date_;
  if (term_type_ == T_MONTH) {
    end_date_.addMonths(term_);
    term_ = end_date_ | start_date_;
  } else if (term_type_ == T_YEAR) {
    end_date_.addYears(term_);
    term_ = end_date_ | start_date_;
  }
}

}  // namespace s21