#include "big_decimal/big_decimal.h"
// #define STRING_SIZE 40

int s21_from_decimal_to_double(s21_decimal src, double *dst) {
  // *dst = 0;
  // //   float power_of_ten[] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7,
  // //                           1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
  // //                           1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
  // 1e23,
  // //                           1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 1e30};
  // char num[STRING_SIZE];
  // for (int i = 0; i < STRING_SIZE - 1; i++) {
  //   num[i] = '0';
  // }
  // num[STRING_SIZE - 1] = 0;

  // big_decimal val = decimal_to_bd(src);
  // int pow = val.e;
  // int i = STRING_SIZE - 2;
  // int printed_digits = 0;
  // while ((!bd_is_zero(val) || (printed_digits != pow)) && i >= 0) {
  //   num[i] = '0' + bd_mod_10(val);
  //   bd_div_10(&val);
  //   i--;
  //   printed_digits++;
  //   if (printed_digits == pow) {
  //     num[i] = '.';
  //     i--;
  //   }
  // }
  // sscanf(num, "%lf", dst);
  // //   for (int i = 0; i < pow; i++) {
  // //     *dst /= 10;
  // //   }
  // if (val.sign) {
  //   *dst *= -1;
  // }
  int ret = 0;
  float num = 0.0f;
  ret = s21_from_decimal_to_float(src, &num);
  *dst = (double)num;
  return ret;
}
