#ifndef S21_COMMON_H
#define S21_COMMON_H

#include <cmath>
#include <ctime>
#include <stdexcept>

namespace s21 {

double bankRound(double number) noexcept;

/* Class for operating with days in date format. */
class Date {
 public:
  Date();
  Date(int day, int month, int year);
  ~Date() = default;

  /* Throw exception if date doesn't exist or 'year' argument is lower than
   * BASE_YEAR. */
  void setDate(int day, int month, int year);

  int getDay() const noexcept;
  int getMonth() const noexcept;
  int getYear() const noexcept;
  int subtract(const Date& sub) const;
  int operator|(const Date& sub) const;
  friend Date operator+(int days, const Date& date);
  Date operator+(int days) const;
  Date operator-(int days) const;
  Date operator+=(int days);
  Date operator-=(int days);
  Date operator++(int);
  Date operator--(int);
  Date operator++();
  Date operator--();
  void addDays(int days);
  void addMonths(int months);
  void addYears(int years);
  Date shiftMonths(int months) const;

  bool operator==(const Date& date) const;
  bool operator!=(const Date& date) const;
  bool operator>=(const Date& date) const;
  bool operator<=(const Date& date) const;
  bool operator>(const Date& date) const;
  bool operator<(const Date& date) const;

  static bool isDateValid(int d, int m, int y) noexcept;

 private:
  static const int BASE_YEAR = 1900;
  static const int BASE_MONTH = 1;
  static const int BASE_DAY = 1;
  struct std::tm base_;
  struct std::tm current_;

  int getDaysSinceBase() const;
  int getDiff(const struct std::tm& end, const struct std::tm& start) const;
};

}  // namespace s21

#endif  // S21_COMMON_H
