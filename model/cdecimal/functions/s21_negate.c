#include "../s21_decimal.h"
#define SIGN_POS 127

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  if (decimal_is_zero(value)) {
    set_bit(0, SIGN_POS, result);
  } else {
    set_bit(!get_bit(value, SIGN_POS), SIGN_POS, result);
  }
  return 0;
}