#include "s21_decimal.h"

s21_decimal s21_negate(s21_decimal decimal) {
  if (decimal.value_type == s21_INFINITY) {
    decimal.value_type = s21_NEGATIVE_INFINITY;
  } else if (decimal.value_type == s21_NEGATIVE_INFINITY) {
    decimal.value_type = s21_INFINITY;
  }

  if (get_sign(&decimal) == NEGATIVE) {
    set_sign(&decimal, POSITIVE);
  } else {
    set_sign(&decimal, NEGATIVE);
  }

  return decimal;
}

s21_decimal s21_truncate(s21_decimal decimal) {
  s21_decimal ten;
  s21_from_int_to_decimal(10, &ten);
  s21_long_decimal long_decimal, long_ten;
  zeroing_and_normalize_long_decimal(&long_decimal);
  decimal_to_long_decimal(&decimal, &long_decimal);
  decimal_to_long_decimal(&ten, &long_ten);
  int scale = get_scale(&decimal);
  sign_values_t current_sign = get_sign(&decimal);
  long_decimal.bits[LONG_DECIMAL_SCALE] = 0;
  for (int i = 0; i < scale; i++) {
    long_decimal = mod_ten(long_decimal, long_ten);
  }
  long_decimal_to_decimal(&long_decimal, &decimal);
  if (current_sign == NEGATIVE) {
    set_sign(&decimal, NEGATIVE);
  }
  set_scale(&decimal, 0);
  return decimal;
}

s21_decimal s21_floor(s21_decimal decimal) {
  decimal = s21_truncate(decimal);
  if (get_sign(&decimal) == NEGATIVE) {
    s21_decimal one;
    zeroing_and_normalize(&one);
    set_bit_by_index(&one, 0);
    decimal = s21_sub(decimal, one);
  }
  return decimal;
}

s21_decimal s21_round(s21_decimal decimal) {
  s21_decimal truncated_decimal, one;
  zeroing_and_normalize(&truncated_decimal);
  zeroing_and_normalize(&one);
  set_bit_by_index(&one, 0);

  truncated_decimal = s21_truncate(decimal);

  s21_decimal fraction;
  zeroing_and_normalize(&fraction);
  fraction = s21_sub(decimal, truncated_decimal);
  set_sign(&fraction, POSITIVE);

  s21_decimal half;
  zeroing_and_normalize(&half);
  set_bit_by_index(&half, 2);
  set_bit_by_index(&half, 0);
  set_scale(&half, 1);

  s21_decimal delta;
  zeroing_and_normalize(&delta);
  delta = s21_sub(half, fraction);

  decimal = s21_truncate(decimal);

  if (is_zero_decimal(&delta)) {
    set_sign(&delta, NEGATIVE);
  }
  set_sign(&one, get_sign(&decimal));
  if (get_sign(&delta) == NEGATIVE) {
    decimal = s21_add(decimal, one);
  }
  return decimal;
}
