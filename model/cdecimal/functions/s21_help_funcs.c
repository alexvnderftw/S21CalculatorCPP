#include "../s21_decimal.h"

#define MAX_DECIMAL_BIT 127
#define MIN_DECIMAL_BIT 0
#define SIZE_OF_INT_IN_BITS 32

#define LOW_MIN_BIT 0
#define LOW_MAX_BIT 31
#define MID_MIN_BIT 32
#define MID_MAX_BIT 63
#define OLD_MIN_BIT 64
#define OLD_MAX_BIT 95

#define INT_ONES 0xffffffff

// Получить бит на позиции pos из decimal
// В случае если pos за пределами (x>127 || x< 0) вернется 0
int get_bit(s21_decimal val, int pos) {
  int bit = 0;
  if (pos <= MAX_DECIMAL_BIT && pos >= MIN_DECIMAL_BIT) {
    int intpos = pos / SIZE_OF_INT_IN_BITS;
    int intbias = pos % SIZE_OF_INT_IN_BITS;
    bit = val.bits[intpos] >> intbias & 1;
  }
  return bit;
}

// Установит bit в необходимую позицию pos в числе val
// Если pos за пределами, ничего не происходит
void set_bit(int bit, int pos, s21_decimal *val) {
  if (pos <= MAX_DECIMAL_BIT && pos >= MIN_DECIMAL_BIT) {
    int intpos = pos / SIZE_OF_INT_IN_BITS;
    int intbias = pos % SIZE_OF_INT_IN_BITS;
    unsigned int mask = (unsigned int)INT_ONES - ((unsigned int)1 << intbias);
    val->bits[intpos] &= mask;
    if (bit) {
      val->bits[intpos] += 1 << intbias;
    }
  }
}

void decimal_set_zeros(s21_decimal *val) {
  for (int i = 0; i < 4; i++) {
    val->bits[i] = 0;
  }
}

int decimal_is_zero(s21_decimal val) {
  int res = 1;
  for (int i = 0; i < 3; i++) {
    if (val.bits[i] != 0) {
      res = 0;
    }
  }
  return res;
}

int get_pow_from_decimal(s21_decimal val) {
  return (unsigned int)val.bits[3] >> 16 & 0xff;
}

int get_sign_from_decimal(s21_decimal val) {
  return get_bit(val, MAX_DECIMAL_BIT);
}

void set_pow(int pow, s21_decimal *dst) {
  pow = (pow & 0xff) << 16;  // Ограничить размер степени байтом и сместить для
                             // дальнейшего сложения
  dst->bits[3] = dst->bits[3] & 0x80ff0000;  // Занулить степень числа
  dst->bits[3] += pow;  // Выставить новую степень
}

s21_decimal s21_abs(s21_decimal val) {
  set_bit(0, MAX_DECIMAL_BIT, &val);
  return val;
}