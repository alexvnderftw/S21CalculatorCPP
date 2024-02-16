#include "big_decimal/big_decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  big_decimal ba = decimal_to_bd(a), bb = decimal_to_bd(b);
  return bd_is_less(ba, bb);
}
int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  big_decimal ba = decimal_to_bd(a), bb = decimal_to_bd(b);
  return bd_is_less_or_eq(ba, bb);
}
int s21_is_greater(s21_decimal a, s21_decimal b) {
  big_decimal ba = decimal_to_bd(a), bb = decimal_to_bd(b);
  return bd_is_more(ba, bb);
}
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  big_decimal ba = decimal_to_bd(a), bb = decimal_to_bd(b);
  return bd_is_more_or_eq(ba, bb);
}
int s21_is_equal(s21_decimal a, s21_decimal b) {
  big_decimal ba = decimal_to_bd(a), bb = decimal_to_bd(b);
  return bd_is_equal(ba, bb);
}
int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}