#ifndef S21_CONTROLLER_CREDIT_H
#define S21_CONTROLLER_CREDIT_H

#include "./../model/s21_credit.h"

namespace s21 {
class ControllerCredit {
 public:
  ControllerCredit() = default;
  ~ControllerCredit() = default;

  void setAnnuity() noexcept;
  void setDifferential() noexcept;
  void setData(double credit, double rate, int time);

  bool isAnnuity() const noexcept;
  bool isDifferential() const noexcept;

  bool calculate();
  bool calculate(double credit, double rate, int time);

  double getSumPaid() const noexcept;
  double getSumMainPart() const noexcept;
  double getSumRatePart() const noexcept;
  int getMonth(size_t pos) const noexcept;
  int getYear(size_t pos) const noexcept;
  double getPayment(size_t pos) const noexcept;
  double getMainPart(size_t pos) const noexcept;
  double getRatePart(size_t pos) const noexcept;
  double getLeftover(size_t pos) const noexcept;
  size_t getDataSize() const noexcept;

 private:
  Credit credit_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_CREDIT_H
