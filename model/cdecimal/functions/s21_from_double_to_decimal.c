#include "big_decimal/big_decimal.h"

// #define DECIMAL_MAX \
//   7.92281577919e+28  // Самое большое число, которое хранится в
//                      // decimal
// #define DECIMAL_NEAR_ZERO \
//   1e-28  // Самое маленькое по модулю число отличное от нуля
// #define BASE_POW 6
// #define STRING_SIZE 40

// double s21_lfabs(double val) {
//   if (val < 0) {
//     val *= -1;
//   }
//   return val;
// }

// int s21_from_double_to_decimal(double src, s21_decimal *dst) {
//   int err = 0;
//   if (src != src || (s21_lfabs(src) > 0 && s21_lfabs(src) < 1e-28) ||
//       s21_lfabs(src) > DECIMAL_MAX) {
//     err = 1;
//     decimal_set_zeros(dst);
//   } else if (src == 0) {
//     decimal_set_zeros(dst);
//   } else {
//     big_decimal val;
//     char num[STRING_SIZE];
//     sprintf(num, "%+.6e", src);  // (sign)#.######e####
//     num[2] = num[1];  // Первую цифру ставим на место десятичной точки
//     num[1] = '0';     // На месте первой цифры ставим 0
//     num[0] = '0';     // На месте знака ставим 0
//     num[9] = ' ';     // На месте "e" ставим пробел
//                       // Получим 00####### ####
//     int float_digits;
//     int exp;
//     sscanf(num, "%d %d", &float_digits, &exp);
//     bd_set_zeros(&val);
//     val.sign = src < 0;
//     val.bits[0] = float_digits;

//     exp = BASE_POW - exp;
//     while (exp < 0) {
//       bd_bits_mult_10(&val);
//       exp++;
//     }
//     val.e = exp;
//     bd_to_decimal(val, dst);
//   }
//   return err;
// }

int s21_from_double_to_decimal(double src, s21_decimal *dst) {
  return s21_from_float_to_decimal((float)src, dst);
}
