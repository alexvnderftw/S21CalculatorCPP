#include "./big_decimal/big_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal a = decimal_to_bd(value_1), b = decimal_to_bd(value_2), res;
  b.sign = !b.sign;
  bd_add(a, b, &res);
  return bd_to_decimal(res, result);
}