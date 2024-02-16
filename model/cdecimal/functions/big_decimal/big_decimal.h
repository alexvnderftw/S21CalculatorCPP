#ifndef BIG_DECIMAL_H_
#define BIG_DECIMAL_H_

#include <stdint.h>
#include <stdio.h>

#include "../../s21_decimal.h"

#define LO 0xffffffff
#define HI 0xffffffff00000000

#define BITS_SIZE 8

typedef struct {
  char sign;
  unsigned short e;
  unsigned long long bits[BITS_SIZE];
} big_decimal;

big_decimal decimal_to_bd(s21_decimal dec);  // Функции конвертации
int bd_to_decimal(big_decimal b_dec, s21_decimal* dest);

void bd_set_zeros(big_decimal* dest);  // Вспомогательные штуки
int bd_is_zero(big_decimal val);
void bd_bits_mult_10(big_decimal* val);
uint64_t bd_mod_10(big_decimal val);
uint64_t bd_div_10(big_decimal* val);
int bd_div_but_10(big_decimal* a, big_decimal b);
void bd_overflow(big_decimal* val);
void bd_scale(big_decimal* a, big_decimal* b);
big_decimal bd_abs(big_decimal val);

int bd_add(big_decimal value_1, big_decimal value_2,
           big_decimal* result);  // Математические операторы
int bd_sub(big_decimal value_1, big_decimal value_2, big_decimal* result);
int bd_mul(big_decimal value_1, big_decimal value_2, big_decimal* result);

int bd_is_less(big_decimal a, big_decimal b);  // Операторы сравнения
int bd_is_less_or_eq(big_decimal a, big_decimal b);
int bd_is_equal(big_decimal a, big_decimal b);
int bd_is_more(big_decimal a, big_decimal b);
int bd_is_more_or_eq(big_decimal a, big_decimal b);

#endif  // BIG_DECIMAL_H_