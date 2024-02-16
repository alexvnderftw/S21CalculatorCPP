#include <stdint.h>

#include "big_decimal.h"
#define MAX_DECIMAL_BIT 127

big_decimal decimal_to_bd(s21_decimal dec) {
  big_decimal b_dec;
  bd_set_zeros(&b_dec);
  b_dec.sign = get_sign_from_decimal(dec);
  b_dec.e = get_pow_from_decimal(dec);
  b_dec.bits[0] = (uint32_t)dec.bits[0];
  b_dec.bits[1] = (uint32_t)dec.bits[1];
  b_dec.bits[2] = (uint32_t)dec.bits[2];
  return b_dec;
}

// Перевод big_decimal в decimal
// Вернет 0 в случае успешной конвертации
// Вернет 1 в случае переполнения
int bd_to_decimal(big_decimal b_dec, s21_decimal* dest) {
  int fail = 0;
  int trail_is_zero = 1, digit = 0;

  big_decimal dec_max, one;
  bd_set_zeros(&dec_max);
  bd_set_zeros(&one);
  one.bits[0] = 1;               // one = 1
  dec_max.bits[0] = 0xffffffff;  // dec_max = DECIMAL_MAX
  dec_max.bits[1] = 0xffffffff;
  dec_max.bits[2] = 0xffffffff;

  // Если наше число не вмещается в decimal и scale позволяет уменьшить число,
  // то делим на 10 В digit будем сохранять последнюю цифру, которая ушла при
  // делении

  // в trail_is_zero сохраняется информация о том встретилась ли цифра отличная
  // от нуля при делении на 10 (важно при банковском округлении)

  while (b_dec.e > 0 && bd_mod_10(b_dec) == 0) {  // Отбрасываем незначащие нули
    digit = bd_div_10(&b_dec);
  }
  dec_max.e =
      b_dec.e;  // Выставим ту же экспоненту, чтобы сравнивать только биты

  while ((b_dec.e > 1 && bd_is_more(bd_abs(b_dec), dec_max)) || b_dec.e > 28) {
    digit = bd_div_10(&b_dec);
    dec_max.e =
        b_dec.e;  // Выставим ту же экспоненту, чтобы сравнивать только биты

    if (digit) {
      trail_is_zero = 0;
    }
  }
  if (b_dec.e && bd_is_more(bd_abs(b_dec), dec_max)) {
    digit = bd_div_10(&b_dec);
    dec_max.e =
        b_dec.e;  // Выставим ту же экспоненту, чтобы сравнивать только биты
  }

  // Банковское округление
  if (digit > 5 ||
      (digit == 5 && (!trail_is_zero || bd_mod_10(b_dec) % 2 == 1))) {
    one.e = b_dec.e;  // Округлять последний знак независимо от scale
    one.sign = b_dec.sign;  // Если число отрицательное, то необходимо прибавить
                            // -1 для округления
    bd_add(b_dec, one, &b_dec);
    bd_overflow(&b_dec);
  }

  while (b_dec.e > 0 &&
         bd_mod_10(b_dec) ==
             0) {  // Отбрасываем незначащие нули, снова после округления
    digit = bd_div_10(&b_dec);
    dec_max.e =
        b_dec.e;  // Выставим ту же экспоненту, чтобы сравнивать только биты
  }

  if (bd_is_more(bd_abs(b_dec), dec_max)) {
    if (b_dec.sign) {
      fail = 2;  // Если число слишком мало вернется 2
    } else {
      fail = 1;  // Если число слишком большое вернется 1
    }
  } else {
    decimal_set_zeros(dest);
    dest->bits[0] = b_dec.bits[0];
    dest->bits[1] = b_dec.bits[1];
    dest->bits[2] = b_dec.bits[2];
    set_bit(b_dec.sign, MAX_DECIMAL_BIT, dest);
    set_pow(b_dec.e, dest);
    if (decimal_is_zero(*dest)) {
      decimal_set_zeros(dest);
    }
  }
  return fail;
}