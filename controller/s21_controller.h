#ifndef S21_CONTROLLER_H
#define S21_CONTROLLER_H

#include "./../model/s21_calculation.h"

namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  double calculate(const std::string expr, const std::string x);
  double calculate(double x);
  void setRadian() noexcept;
  void setDegree() noexcept;
  bool isSuccessful() const noexcept;
  bool isEmpty() const noexcept;

 private:
  Calculation calculator_;
};
}  // namespace s21

#endif  // S21_CONTROLLER_H
