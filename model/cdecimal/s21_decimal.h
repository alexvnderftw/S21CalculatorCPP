#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#include <stdio.h>

typedef struct {
  int bits[4];
} s21_decimal;

// Arithmetic operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Bool operartions
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Type cast
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_double_to_decimal(double src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_double(s21_decimal src, double *dst);

// Other
int s21_floor(s21_decimal value,
              s21_decimal *result);  // round up to smaller int num
int s21_round(s21_decimal value,
              s21_decimal *result);  // round up to nearest int num
int s21_truncate(s21_decimal value, s21_decimal *result);  // drop floating part
int s21_negate(s21_decimal value, s21_decimal *result);    // decimal*=-1

// help_func
s21_decimal s21_abs(s21_decimal val);  // Absolute value

void decimal_print(s21_decimal val);  // represent decimal in bits to console
void decimal_set_zeros(s21_decimal *val);  // decimal = 0;
int decimal_is_zero(s21_decimal val);

// s21_decimal decimal_mult_2(s21_decimal val);  // shift decimal bits <<
// s21_decimal decimal_div_2(s21_decimal val);   // shift decimal bits >>

int get_bit(s21_decimal val, int pos);  // get bit on position in decimal
void set_bit(int bit, int pos,
             s21_decimal *val);  // set bit to position in decimal

int get_pow_from_decimal(s21_decimal val);   // get pow of decimal
void set_pow(int pow, s21_decimal *dst);     // set pow to decimal
int get_sign_from_decimal(s21_decimal val);  // get sign of decimal

#endif  // S21_DECIMAL_H_