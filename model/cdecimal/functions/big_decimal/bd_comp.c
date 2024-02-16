#include "big_decimal.h"
#include "math.h"

int bd_is_less(big_decimal a, big_decimal b) {
  int res = 0;
  if (bd_is_zero(a) && bd_is_zero(b)) {  // Если оба числа нули
    res = 0;
  } else {
    int flag_less = 0, flag_more = 0;  // Флаг для сравнения битов

    // Приведем числа к единому scale
    bd_scale(&a, &b);

    // Сравним биты чисел (необходимо понять МЕНЬШЕ ли абсолютное значение числа
    // a)
    int i = BITS_SIZE - 1;
    while (i > 0 && a.bits[i] == b.bits[i]) {
      i--;
    }
    if (a.bits[i] < b.bits[i]) {
      flag_less = 1;
    } else if (a.bits[i] > b.bits[i]) {
      flag_more = 1;
    }

    // Результат сравнения положительный если,
    // a<0 и b>0
    // или
    // a>0 и b>0 и abs(a) < abs(b)
    // или
    // a<0 и b<0 и abs(a) > abs(b)
    res = (a.sign && !b.sign) || (!a.sign && !b.sign && flag_less) ||
          (a.sign && b.sign && flag_more);
  }
  return res;
}

int bd_is_less_or_eq(big_decimal a, big_decimal b) {
  return bd_is_less(a, b) || bd_is_equal(a, b);
}

int bd_is_equal(big_decimal a, big_decimal b) {
  int res = 0;
  if (bd_is_zero(a) && bd_is_zero(b)) {  // Если оба числа нули
    res = 1;
  } else {
    // Приведем числа к единому scale
    bd_scale(&a, &b);

    // Сравним биты чисел
    int i = 0;
    while (i < BITS_SIZE && a.bits[i] == b.bits[i]) {
      i++;
    }
    if (i == BITS_SIZE && a.sign == b.sign) {
      res = 1;
    }
  }
  return res;
}

int bd_is_more(big_decimal a, big_decimal b) {
  return !bd_is_less(a, b) && !bd_is_equal(a, b);
}

int bd_is_more_or_eq(big_decimal a, big_decimal b) {
  return bd_is_more(a, b) || bd_is_equal(a, b);
}