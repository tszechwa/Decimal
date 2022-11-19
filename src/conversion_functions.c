#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  flag_values_t flag = FLAG_SUCCESS;
  if (src != NAN && src != s21_POS_INF && dst != NULL) {
    zeroing_and_normalize(dst);
    if (src < 0) {
      src *= -1;
      set_sign(dst, NEGATIVE);
    }
    for (int i = BITS_LOW_MIN; i <= BITS_HIGH_MAX; i++) {
      if (src % 2 != 0) {
        set_bit_by_index(dst, i);
        src -= 1;
      }
      src /= 2;
    }
  } else {
    zeroing_decimal(dst);
    if (src == s21_POS_INF) {
      dst->value_type = s21_POS_INF;
    } else if (src == NAN) {
      dst->value_type = s21_NAN;
    }
    flag = FLAG_FAILURE;
  }
  return flag;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  flag_values_t flag = FLAG_SUCCESS;
  if (dst == s21_NULL) {
    flag = FLAG_FAILURE;
  } else if (s21_is_nan(src) || s21_is_pos_inf(src) || s21_is_neg_inf(src)) {
    flag = FLAG_FAILURE;
    if (s21_is_nan(src)) {
      dst->value_type = s21_NAN;
    } else if (s21_is_pos_inf(src)) {
      dst->value_type = s21_INFINITY;
    } else {
      dst->value_type = s21_NEGATIVE_INFINITY;
    }
  } else {
    zeroing_and_normalize(dst);
    int scale = 0;
    float cln = src;
    int x = 0;
    while (cln != (int)cln) {
      if (x == 6) break;
      src *= 10.00;
      scale++;
      cln = src;
      x++;
    }
    s21_from_int_to_decimal((int)cln, dst);
    set_scale(dst, scale);
    if (src < 0) {
      set_sign(dst, NEGATIVE);
    }
  }
  return flag;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 0;
  if (s21_NULL != dst && src.value_type == s21_NORMAL_VALUE) {
    double value = 0;
    for (int i = BITS_LOW_MIN; i <= BITS_HIGH_MAX; i++) {
      int bit = get_bit_by_index(&src, i);
      value += pow(2, i) * bit;
    }
    value /= pow(10, get_scale(&src));
    if (get_sign(&src) == NEGATIVE) {
        value *= -1;
    }
    if (value >= INT_MIN && value <= INT_MAX) {
      *dst = (int)value;
    } else {
      flag = 1;
    }
  } else {
    flag = 1;
  }
  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int flag = 0, temp;
  float dst_buf = 0;
  long long stp = s21_int_pow(10, get_scale(&src));
  for (int i = 0; i != 96; ++i) {
    if (get_bit_by_index(&src, i) == 1) {
      temp = 1 << i;
      dst_buf += (temp + 0.0) / stp;
    }
  }
  *dst = (get_sign(&src) != 0) && !(flag) ? -(dst_buf) : dst_buf;
  return flag;
}
