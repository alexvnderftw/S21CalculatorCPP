#include "big_decimal.h"

int bd_add(big_decimal value_1, big_decimal value_2, big_decimal *result) {
  big_decimal res;
  bd_scale(&value_1, &value_2);  // Приводим числа к единому scale
  res.e = value_1.e;  // Сразу запишим scale для результата
  if (value_1.sign == value_2.sign) {
    for (int i = 0; i < BITS_SIZE; i++) {
      res.bits[i] = value_1.bits[i] + value_2.bits[i];
    }
    bd_overflow(&res);
    res.sign = value_1.sign;
  }  // Если знаки разные
  else {
    uint32_t unit = 0;  // Займ у старшего бита
    if (bd_is_more(bd_abs(value_1),
                   bd_abs(value_2))) {  // Если абсолютное значение value_1
                                        // больше value_2, то меняем местами
      big_decimal buff = value_2;
      value_2 = value_1;
      value_1 = buff;
    }
    for (int i = 0; i < BITS_SIZE; i++) {  // Вычитаем из большего (value_2)
      res.bits[i] =
          (uint32_t)value_2.bits[i] - (uint32_t)value_1.bits[i] - unit;
      if (value_2.bits[i] - value_1.bits[i] - unit > value_2.bits[i]) {
        unit = 1;
      } else {
        unit = 0;
      }
    }
    if (bd_is_zero(res)) {
      res.sign = 0;
      res.e = 0;
    } else {
      res.sign =
          value_2.sign;  // Присваивается знак большего значения (value_2)
    }
  }
  *result = res;
  bd_div_10(&res);

  return 0;
}