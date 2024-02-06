#ifndef S21_CONTROLLER_CREDIT_H
#define S21_CONTROLLER_CREDIT_H

#include "./../model/s21_credit.h"

namespace s21 {
class ControllerCredit {
 public:
  ControllerCredit() = default;
  ~ControllerCredit() = default;

  void setAnnuity();
  void setDifferential();
  void setData(double credit, double rate, int time);

  bool isAnnuity();
  bool isDifferential();

  bool calculate();
  bool calculate(double credit, double rate, int time);

  double getSumPaid();
  double getSumMainPart();
  double getSumRatePart();
  int getMonth(size_t pos);
  int getYear(size_t pos);
  double getPayment(size_t pos);
  double getMainPart(size_t pos);
  double getRatePart(size_t pos);
  double getLeftover(size_t pos);
  size_t getDataSize();

 private:
  Credit credit_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_CREDIT_H
