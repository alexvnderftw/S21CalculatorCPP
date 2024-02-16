#include "big_decimal.h"

int bd_mul(big_decimal value_1, big_decimal value_2, big_decimal *result) {
  bd_set_zeros(result);
  if (bd_is_zero(value_1) || bd_is_zero(value_2)) {
    // Если одно из чисел 0, то и результат умножения 0
  } else {
    result->e = value_1.e + value_2.e;
    result->sign = value_1.sign ^ value_2.sign;
    for (int i = 0; i < BITS_SIZE; i++) {
      for (int j = 0; j < BITS_SIZE; j++) {
        if (i + j < BITS_SIZE) {
          result->bits[i + j] += value_1.bits[i] * value_2.bits[j];
          bd_overflow(result);
        }
      }
    }
  }
  return 0;
}