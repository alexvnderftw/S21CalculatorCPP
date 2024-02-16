#include "../s21_decimal.h"
#define SIGN_POS 127

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int err = 0;
  decimal_set_zeros(dst);
  if (dst) {
    if (src >= 0) {
      dst->bits[0] = src;
    } else if (src < 0) {
      dst->bits[0] = -src;
      set_bit(1, SIGN_POS, dst);
    }
  } else {
    err = 1;
  }
  return err;
}
