#include "big_decimal/big_decimal.h"

#define DECIMAL_SIGN 127

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int err = 0;
  *dst = 0;
  s21_truncate(src, &src);
  s21_decimal int_max, int_min;
  decimal_set_zeros(&int_max);
  decimal_set_zeros(&int_min);
  int_max.bits[0] = __INT32_MAX__;
  int_min.bits[0] = 0x80000000;
  set_bit(1, DECIMAL_SIGN, &int_min);

  if ((s21_is_greater(s21_abs(src), int_max) &&
       get_bit(src, DECIMAL_SIGN) == 0) ||
      (s21_is_greater(s21_abs(src), s21_abs(int_min)) &&
       get_bit(src, DECIMAL_SIGN) == 1)) {
    err = 1;
  } else {
    if (get_bit(src, DECIMAL_SIGN)) {
      if (s21_is_equal(src, int_min)) {
        *dst = -__INT32_MAX__ - 1;
      } else {
        *dst = -src.bits[0];
      }
    } else {
      *dst = src.bits[0];
    }
  }
  return err;
}