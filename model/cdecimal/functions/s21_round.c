#include "big_decimal/big_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int trail_is_zero = 1, digit = 0;

  big_decimal bd = decimal_to_bd(value), one;
  bd_set_zeros(&one);
  one.bits[0] = 1;  // one = 1

  while (bd.e > 1) {
    digit = bd_div_10(&bd);
    if (digit) {
      trail_is_zero = 0;
    }
  }
  if (bd.e) {
    digit = bd_div_10(&bd);
  }

  // Банковское округление
  if (digit > 5 || (digit == 5 && (!trail_is_zero || bd_mod_10(bd) % 2 == 1))) {
    one.e = bd.e;  // Округлять последний знак независимо от scale
    one.sign = bd.sign;  // Если число отрицательное, то необходимо прибавить -1
                         // для округления
    bd_add(bd, one, &bd);
  }

  return bd_to_decimal(bd, result);
}