#include "big_decimal.h"

// вернет 1 если big decimal == 0
// -0 == 0
int bd_is_zero(big_decimal val) {
  int res = 1;
  for (int i = 0; i < BITS_SIZE; i++) {
    if (val.bits[i] != 0) {
      res = 0;
    }
  }
  return res;
}

// Исправит полученное переполнение
void bd_overflow(big_decimal* val) {
  for (int i = 1; i < BITS_SIZE; i++) {
    val->bits[i] += (val->bits[i - 1] & HI) >> 32;
    val->bits[i - 1] &= LO;
  }
}

// Умножит value big decimal на 10
// SCALE НЕ ИЗМЕНИТСЯ!!!
void bd_bits_mult_10(big_decimal* val) {
  for (int i = 0; i < BITS_SIZE; i++) {
    val->bits[i] *= 10;
  }
  bd_overflow(val);
}

// Сколько раз b есть в a
// а изменится после деления
int bd_div_but_10(big_decimal* a, big_decimal b) {
  int res = 0;
  a->sign = 0;
  b.sign = 0;
  while (bd_is_more_or_eq(*a, b)) {
    bd_sub(*a, b, a);
    bd_overflow(a);
    res++;
  }
  return res;
}

// Разделит big_decimal на 10 и вернет остаток
// Работает не совсем как деление, например:
// 154,53 -> 154,5 и вернет 3
uint64_t bd_div_10(big_decimal* val) {
  uint64_t reminder = 0;
  if (bd_is_zero(*val)) {
    // Если число 0, тогда ничего не делаем
    val->e = 0;
  } else {
    val->e--;
    for (int i = BITS_SIZE - 1; i >= 0; i--) {
      val->bits[i] += reminder << 32;
      reminder = val->bits[i] % 10;
      val->bits[i] /= 10;
    }
  }
  return reminder;
}

// Вернет остаток от деления big_decimal на 10
uint64_t bd_mod_10(big_decimal val) {
  uint64_t reminder = 0;
  for (int i = BITS_SIZE - 1; i >= 0; i--) {
    val.bits[i] += reminder << 32;
    reminder = val.bits[i] % 10;
    val.bits[i] /= 10;
  }
  return reminder;
}

// Приведет два числа к одному scale
void bd_scale(big_decimal* a, big_decimal* b) {
  if (a->e < b->e) {
    while (a->e < b->e) {
      bd_bits_mult_10(a);
      a->e++;
    }
  } else if (a->e > b->e) {
    while (a->e > b->e) {
      bd_bits_mult_10(b);
      b->e++;
    }
  }
}

// Абсолютное значение big decimal
big_decimal bd_abs(big_decimal val) {
  val.sign = 0;
  return val;
}

// Заполнит big decimal нулями
void bd_set_zeros(big_decimal* dest) {
  dest->e = 0;
  dest->sign = 0;
  for (int i = 0; i < BITS_SIZE; i++) {
    dest->bits[i] = 0;
  }
}