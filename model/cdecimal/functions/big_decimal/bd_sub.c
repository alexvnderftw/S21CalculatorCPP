#include "big_decimal.h"

int bd_sub(big_decimal value_1, big_decimal value_2, big_decimal *result) {
  value_2.sign = 1;
  return bd_add(value_1, value_2, result);
}