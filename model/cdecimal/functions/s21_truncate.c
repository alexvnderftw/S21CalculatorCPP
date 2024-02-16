#include "big_decimal/big_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  big_decimal bd = decimal_to_bd(value);
  if (!bd_is_zero(bd)) {
    while (bd.e) {
      bd_div_10(&bd);
    }
  }
  return bd_to_decimal(bd, result);
}