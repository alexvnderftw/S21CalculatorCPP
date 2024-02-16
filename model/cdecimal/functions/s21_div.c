#include "big_decimal/big_decimal.h"
#define DECIMAL_SIGNS_NUM 30

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal a = decimal_to_bd(value_1), b = decimal_to_bd(value_2), res,
              val_3;
  bd_set_zeros(&res);  // Инициализация переменной с результатом
  bd_set_zeros(&val_3);  // Для прибавления
  int err = 0;           // Код возврата
  if (bd_is_zero(b)) {   // Проверка на ноль
    err = 3;
  } else if (bd_is_zero(a)) {
    decimal_set_zeros(result);
  } else {
    char sign = a.sign ^ b.sign;  // Знак результата
    a.sign = 0;
    b.sign = 0;

    int pow = 1;
    bd_scale(&a, &b);
    while (bd_is_more_or_eq(a, b)) {
      bd_bits_mult_10(&b);
      pow++;
    }
    int decimal_digits = pow - 1;
    int digits = decimal_digits;
    if (digits == 0) {  // Если a/b < 1, тогда учтем знак нуля 0. в записи
                        // вещественного числа
      digits = 1;
    }

    while (pow > 1) {
      bd_bits_mult_10(&res);
      bd_bits_mult_10(&a);
      pow--;
      if (bd_is_more_or_eq(a, b)) {
        val_3.bits[0] = bd_div_but_10(&a, b);  // Разделим a на b
        bd_add(res, val_3, &res);  // Прибавим результат деления
      }
    }

    while (digits < DECIMAL_SIGNS_NUM) {
      bd_bits_mult_10(&res);
      bd_bits_mult_10(&a);
      pow++;
      digits++;
      if (bd_is_more_or_eq(a, b)) {
        val_3.bits[0] = bd_div_but_10(&a, b);  // Разделим a на b
        bd_add(res, val_3, &res);  // Прибавим результат деления
      }
    }

    res.e = digits - decimal_digits;
    res.sign = sign;
    err = bd_to_decimal(res, result);
    if (decimal_is_zero(*result)) {
      result->bits[3] = 0;
    }
  }
  return err;
}
