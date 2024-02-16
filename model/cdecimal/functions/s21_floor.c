#include "big_decimal/big_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int trail_is_zero = 1;

  big_decimal bd = decimal_to_bd(value), one;
  if (!bd_is_zero(bd)) {
    bd_set_zeros(&one);
    one.bits[0] = 1;  // one = 1

    while (bd.e >= 1) {
      if (bd_div_10(&bd)) {
        trail_is_zero = 0;
      }
    }

    // Если число отрицательное и дробная часть не 0, тогда прибавить -1
    if (bd.sign == 1 && !trail_is_zero) {
      one.e = bd.e;
      one.sign = 1;
      bd_add(bd, one, &bd);
    }
  }

  return bd_to_decimal(bd, result);
}