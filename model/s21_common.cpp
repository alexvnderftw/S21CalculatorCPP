#include "s21_common.h"

namespace s21 {

/* Regular bank round of double number to integer value. */
double bankRound(double number) {
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

Date::Date() {
  time_t t = std::time(NULL);
  if (std::localtime(&t) == NULL)
    throw std::runtime_error(
        "Can't read local time format or it doesn't exist.");
  current_ = *std::localtime(&t);
  current_.tm_sec = 0;
  current_.tm_min = 0;
  current_.tm_hour = 12;
  base_.tm_sec = 0;
  base_.tm_min = 0;
  base_.tm_hour = 12;
  base_.tm_mday = BASE_DAY;
  base_.tm_mon = BASE_MONTH - 1;
  base_.tm_year = BASE_YEAR - 1900;
  /// diff
}

Date::Date(int day, int month, int year) {
  if (isDateValid(day, month, year) == false)
    throw std::invalid_argument("Invalid date.");
  current_.tm_sec = 0;
  current_.tm_min = 0;
  current_.tm_hour = 12;
  current_.tm_mday = day;
  current_.tm_mon = month - 1;
  current_.tm_year = year - 1900;
  base_.tm_sec = 0;
  base_.tm_min = 0;
  base_.tm_hour = 12;
  base_.tm_mday = BASE_DAY;
  base_.tm_mon = BASE_MONTH - 1;
  base_.tm_year = BASE_YEAR - 1900;
}

void Date::setDate(int day, int month, int year) {
  if (isDateValid(day, month, year) == false)
    throw std::invalid_argument("Invalid date.");
  current_.tm_mday = day;
  current_.tm_mon = month - 1;
  current_.tm_year = year - 1900;
}

// int Date::getDays(const Date& date) {}

// Date Date::getDate(int days) {}

int Date::getDay() { return current_.tm_mday; }

int Date::getMonth() { return current_.tm_mon + 1; }

int Date::getYear() { return current_.tm_year + 1900; }

int Date::subtract(const Date& sub) const {
  return getDiff(current_, sub.current_);
}

int Date::operator|(const Date& sub) const {
  // Date first_new(first), second_new(second);
  return subtract(sub);
}

Date operator+(int days, const Date& date) {
  Date date_new(date);
  date_new.addDays(days);
  return date_new;
}

Date Date::operator+(int days) {
  Date date_new(*this);
  date_new.addDays(days);
  return date_new;
}

Date Date::operator-(int days) {
  Date date_new(*this);
  date_new.addDays(-days);
  return date_new;
}

Date Date::operator+=(int days) {
  addDays(days);
  return *this;
}

Date Date::operator-=(int days) {
  addDays(-days);
  return *this;
}

Date Date::operator++(int) {
  addDays(1);
  return *this;
}

Date Date::operator--(int) {
  addDays(-1);
  return *this;
}

Date Date::operator++() {
  addDays(1);
  return *this;
}

Date Date::operator--() {
  addDays(-1);
  return *this;
}

bool Date::operator==(const Date& date) const {
  return getDaysSinceBase() == date.getDaysSinceBase();
}

bool Date::operator!=(const Date& date) const {
  return getDaysSinceBase() != date.getDaysSinceBase();
}

bool Date::operator>=(const Date& date) const {
  return getDaysSinceBase() >= date.getDaysSinceBase();
}

bool Date::operator<=(const Date& date) const {
  return getDaysSinceBase() <= date.getDaysSinceBase();
}

bool Date::operator>(const Date& date) const {
  return getDaysSinceBase() > date.getDaysSinceBase();
}

bool Date::operator<(const Date& date) const {
  return getDaysSinceBase() < date.getDaysSinceBase();
}

// bool isDateValid(int d, int m, int y);

void Date::addDays(int days) {
  std::tm new_date = current_;
  time_t tmp = std::mktime(&new_date);
  tmp += static_cast<time_t>(days) * (60 * 60 * 24);
  new_date = *std::localtime(&tmp);
  current_ = new_date;
}

void Date::addMonths(int months) {
  int new_day = getDay(), new_month = 0, new_year = 0;
  if (getMonth() + months <= 0) {
    new_month = 12 - std::abs(getMonth() + months) % 12;
    new_year = getYear() - 1 - std::abs(getMonth() + months) / 12;
  } else {
    new_month = (getMonth() + months - 1) % 12 + 1;
    new_year = getYear() + (getMonth() + months - 1) / 12;
  }

  while (!isDateValid(new_day, new_month, new_year)) {
    new_day--;
  }
  setDate(new_day, new_month, new_year);
}

void Date::addYears(int years) {
  int new_year = getYear() + years;
  if (!isDateValid(getDay(), getMonth(), new_year))
    setDate(getDay() - 1, getMonth(), new_year);
  else
    setDate(getDay(), getMonth(), new_year);
}

// need this??
int Date::getDaysSinceBase() const { return getDiff(current_, base_); }

int Date::getDiff(const struct std::tm& end,
                  const struct std::tm& start) const {
  struct std::tm end_new = end, start_new = start;
  double difference =
      std::round(std::difftime(std::mktime(&end_new), std::mktime(&start_new)) /
                 (60 * 60 * 24));
  return static_cast<int>(difference);
}

bool Date::isDateValid(int d, int m, int y) {
  if (y < BASE_YEAR) return false;
  if (m < 1 || m > 12) return false;
  if (d < 1 || d > 31) return false;
  if (d == 31 && (m == 2 || m == 4 || m == 6 || m == 9 || m == 11))
    return false;
  if (d == 30 && m == 2) return false;
  if (m == 2 && d == 29 && y % 4 != 0) return false;
  if (m == 2 && d == 29 && y % 400 == 0) return true;
  if (m == 2 && d == 29 && y % 100 == 0) return false;
  if (m == 2 && d == 29 && y % 4 == 0) return true;
  return true;
}

}  // namespace s21
