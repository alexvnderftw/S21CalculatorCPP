#include "big_decimal/big_decimal.h"
#define SIGN_POS 127

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal a = decimal_to_bd(value_1), b = decimal_to_bd(value_2), res;
  bd_mul(a, b, &res);
  return bd_to_decimal(res, result);
}