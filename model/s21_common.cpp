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

}  // namespace s21
