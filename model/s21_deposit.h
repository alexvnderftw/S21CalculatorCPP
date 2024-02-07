#ifndef S21_DEPOSIT_H
#define S21_DEPOSIT_H

// #include <vector>

// #include "s21_common.h"

namespace s21 {
class Deposit {
 public:
  Deposit() = default;
  ~Deposit() = default;

 private:
  double deposit_ = 0.0;
  int term_ = 0;

  bool capital_ = false;
};
}  // namespace s21

#endif  // S21_DEPOSIT_H
