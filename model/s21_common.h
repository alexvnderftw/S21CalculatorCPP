#ifndef S21_COMMON_H
#define S21_COMMON_H

#include <cmath>
#include <ctime>
#include <stdexcept>

namespace s21 {

double bankRound(double number);

/* Class for operating with days in date format.
Throws exception when date created is invalid. */
class Date {
 public:
  Date();
  Date(int day, int month, int year);
  ~Date() = default;

  void setDate(int day, int month, int year);

  // int getDays(const Date& date);
  // Date getDate(int days);
  int getDay();
  int getMonth();
  int getYear();
  int getDiff(const Date& first, const Date& second);
  friend int operator-(const Date& first, const Date& second);
  friend Date operator+(int days, const Date& date);
  void addDays(int days);

  bool isDateValid(int d, int m, int y);

 private:
  const int BASE_YEAR = 1900;
  const int BASE_MONTH = 1;
  const int BASE_DAY = 1;
  struct std::tm base_ = {0, 0, 12, BASE_DAY, BASE_MONTH - 1, BASE_YEAR - 1900};
  struct std::tm current_;
  // std::time_t base_seconds_;
  // std::time_t current_seconds_;
  // int days_;
  // int day_;
  // int month_;
  // int year_;

  int getDaysSinceBase(int d, int m, int y);
  int getDiff(const struct std::tm& first, const struct std::tm& second);
};

}  // namespace s21
// Take into consideration 'year' argument can't be lower than 1900 (will throw
// exception).
#endif  // S21_COMMON_H
