#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

void init_max_decimal(s21_decimal *decimal) {
  for (int i = BITS_LOW_MIN; i <= BITS_HIGH_MAX; i++) {
    set_bit_by_index(decimal, i);
  }
}

START_TEST(add) {
  s21_decimal d1, d2, r;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  zeroing_and_normalize(&r);
  int x1 = 100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 + x2);

  x1 = -100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 + x2);

  x1 = 100, x2 = -500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 400);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = -100, x2 = -500;
  s21_from_int_to_decimal(x2, &d2);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 400);

  init_max_decimal(&d1);
  s21_from_int_to_decimal(1, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 0);
  ck_assert_int_eq(r.bits[MIDDLE], 0);
  ck_assert_int_eq(r.bits[HIGH], 0);
  ck_assert_int_eq(r.value_type, 1);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 0);
  ck_assert_int_eq(r.bits[MIDDLE], 0);
  ck_assert_int_eq(r.bits[HIGH], 0);
  ck_assert_int_eq(r.value_type, 1);

  s21_from_float_to_decimal(123.5, &d1);
  s21_from_float_to_decimal(55.6, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 1791);
  ck_assert_int_eq(get_scale(&r), 1);

  s21_from_float_to_decimal(17.45, &d1);
  s21_from_float_to_decimal(5.91, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 2336);
  ck_assert_int_eq(get_scale(&r), 2);

  s21_from_float_to_decimal(10.2, &d1);
  s21_from_float_to_decimal(50.66, &d2);
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 6086);
  ck_assert_int_eq(get_scale(&r), 2);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.value_type, s21_INFINITY);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  r = s21_add(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NEGATIVE_INFINITY);
}
END_TEST

START_TEST(sub) {
  s21_decimal d1, d2, r;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  zeroing_and_normalize(&r);

  int x1 = 100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 100 * 500);
  ck_assert_int_eq(get_sign(&r), 0);

  x1 = -100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 600);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = 100, x2 = -500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 600);
  ck_assert_int_eq(get_sign(&r), 0);

  x1 = -100, x2 = -500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 400);
  ck_assert_int_eq(get_sign(&r), 0);

  s21_from_float_to_decimal(123.55, &d1);
  s21_from_float_to_decimal(55.6, &d2);
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 6795);
  ck_assert_int_eq(get_scale(&r), 2);

  s21_from_float_to_decimal(5.91, &d1);
  s21_from_float_to_decimal(17.45, &d2);
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 1154);
  ck_assert_int_eq(get_scale(&r), 2);
  ck_assert_int_eq(get_sign(&r), 1);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.value_type, s21_INFINITY);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  r = s21_sub(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);
}
END_TEST

START_TEST(mul) {
  s21_decimal d1, d2, r;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  zeroing_and_normalize(&r);
  int x1 = 100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 * x2);

  x1 = -100, x2 = 500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 * x2 * -1);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = 100, x2 = -500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 * x2 * -1);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = -100, x2 = -500;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 * x2);
  ck_assert_int_eq(get_sign(&r), 0);

  s21_from_float_to_decimal(123.5, &d1);
  s21_from_float_to_decimal(55.6, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 686660);
  ck_assert_int_eq(get_scale(&r), 2);

  s21_from_float_to_decimal(17.45, &d1);
  s21_from_float_to_decimal(5.91, &d2);
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 1031295);
  ck_assert_int_eq(get_scale(&r), 4);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.value_type, s21_INFINITY);


  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  r = s21_mul(d1, d2);
  ck_assert_int_eq(r.value_type, s21_INFINITY);
}
END_TEST

START_TEST(div_) {
  s21_decimal d1, d2, r;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  zeroing_and_normalize(&r);
  int x1 = 500, x2 = 100;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 / x2);

  x1 = -123, x2 = 3;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 * -1 / x2);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = 33333, x2 = -3;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 / x2 * -1);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = -1025, x2 = -25;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 / x2);
  ck_assert_int_eq(get_sign(&r), 0);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 1);

  s21_from_float_to_decimal(12.25, &d1);
  s21_from_float_to_decimal(2.5, &d2);
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 49);
  ck_assert_int_eq(get_scale(&r), 1);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NORMAL_VALUE;
  d2.value_type = s21_NORMAL_VALUE;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NORMAL_VALUE;
  r = s21_div(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NORMAL_VALUE);
}
END_TEST

START_TEST(mod) {
  s21_decimal d1, d2, r;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  zeroing_and_normalize(&r);
  int x1 = 500, x2 = 100;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 500 % 100);

  x1 = 121, x2 = 3;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 % x2);
  ck_assert_int_eq(get_sign(&r), 0);

  x1 = 33333, x2 = -3;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 % x2 * -1);
  ck_assert_int_eq(get_sign(&r), 1);

  x1 = -1025, x2 = 25;
  s21_from_int_to_decimal(x1, &d1);
  s21_from_int_to_decimal(x2, &d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], x1 % x2 * -1);
  ck_assert_int_eq(get_sign(&r), 1);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 0);

  s21_from_float_to_decimal(12.25, &d1);
  s21_from_float_to_decimal(2.5, &d2);
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.bits[LOW], 225);
  ck_assert_int_eq(get_scale(&r), 2);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NAN);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NORMAL_VALUE;
  r = s21_mod(d1, d2);
  ck_assert_int_eq(r.value_type, s21_NORMAL_VALUE);
}
END_TEST

START_TEST(is_less) {  // 1
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_less(d1, d2), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(100, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(1.02, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 123;
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(-150, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 0);

  s21_from_int_to_decimal(-150, &d1);
  s21_from_int_to_decimal(-100, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 0);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
  ck_assert_int_eq(s21_is_less(d2, d1), 0);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);
}
END_TEST

START_TEST(is_less_or_equal) {  // 2
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(-100, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(12.55, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 25;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  s21_from_float_to_decimal(-150, &d1);
  s21_from_float_to_decimal(-100, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);


  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 0);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(d2, d1), 1);
}
END_TEST

START_TEST(is_greather) {  // 3
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(100, &d2);
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(1.02, &d2);
  ck_assert_int_eq(s21_is_greater(d1, d2), 0);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 123;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_greater(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
  ck_assert_int_eq(s21_is_less(d2, d1), 1);
}
END_TEST

START_TEST(is_greather_or_equal) {  // 4
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(100, &d2);
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(1.02, &d2);
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 123;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_greater_or_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(d2, d1), 1);
}
END_TEST

START_TEST(is_equal) {  // 4
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  ck_assert_int_eq(s21_is_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_equal(d2, d1), 0);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_equal(d2, d1), 0);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(100, &d2);
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(1.02, &d2);
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 123;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);
}
END_TEST

START_TEST(is_not_equal) {  // 4
  s21_decimal d1, d2;
  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(24, &d2);

  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(12, &d2);

  ck_assert_int_eq(s21_is_not_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 1);

  init_max_decimal(&d1);
  init_max_decimal(&d2);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 1);

  set_scale(&d1, 27);  // 79.228162514264337593543950335
  set_scale(&d2, 24);  // 79228.162514264337593543950335
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_int_to_decimal(-100, &d1);
  s21_from_int_to_decimal(100, &d2);
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_int_to_decimal(-__INT_MAX__, &d1);
  s21_from_int_to_decimal(__INT_MAX__, &d2);
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_float_to_decimal(4.5, &d1);
  s21_from_float_to_decimal(5.6, &d2);
  ck_assert_int_eq(s21_is_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_equal(d2, d1), 1);

  s21_from_float_to_decimal(12.55, &d1);
  s21_from_float_to_decimal(1.02, &d2);
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 5;
  d2.bits[MIDDLE] = 100;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  s21_from_float_to_decimal(100, &d1);
  s21_from_float_to_decimal(100, &d2);
  d1.bits[MIDDLE] = 10;
  d1.bits[HIGH] = 25;
  d2.bits[MIDDLE] = 10;
  d2.bits[HIGH] = 123;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  zeroing_and_normalize(&d1);
  zeroing_and_normalize(&d2);
  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NEGATIVE_INFINITY;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 1);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 1);

  d1.value_type = s21_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  d1.value_type = s21_NEGATIVE_INFINITY;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  d1.value_type = s21_NAN;
  d2.value_type = s21_INFINITY;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);

  d1.value_type = s21_NAN;
  d2.value_type = s21_NAN;
  ck_assert_int_eq(s21_is_not_equal(d1, d2), 0);
  ck_assert_int_eq(s21_is_not_equal(d2, d1), 0);
}
END_TEST

START_TEST(from_int_to_decimal) {
  s21_decimal d;
  zeroing_and_normalize(&d);
  int test = 12;
  s21_from_int_to_decimal(12, &d);
  ck_assert_int_eq(d.bits[LOW], test);
  ck_assert_int_eq(d.bits[1], 0);
  ck_assert_int_eq(d.bits[2], 0);
  ck_assert_int_eq(d.bits[3], 0);

  test = __INT_MAX__;
  s21_from_int_to_decimal(__INT_MAX__, &d);
  ck_assert_int_eq(d.bits[LOW], test);
  ck_assert_int_eq(d.bits[MIDDLE], 0);
  ck_assert_int_eq(d.bits[HIGH], 0);
  ck_assert_int_eq(d.bits[SCALE], 0);

  test = 0;
  s21_from_int_to_decimal(0, &d);
  ck_assert_int_eq(d.bits[LOW], test);
  ck_assert_int_eq(d.bits[MIDDLE], 0);
  ck_assert_int_eq(d.bits[HIGH], 0);
  ck_assert_int_eq(d.bits[SCALE], 0);

  test = -5;
  s21_from_int_to_decimal(-5, &d);
  ck_assert_int_eq(d.bits[LOW], test * -1);
  ck_assert_int_eq(get_sign(&d), 1);
  ck_assert_int_eq(d.bits[MIDDLE], 0);
  ck_assert_int_eq(d.bits[HIGH], 0);

  test = -__INT_MAX__;
  s21_from_int_to_decimal(-__INT_MAX__, &d);
  ck_assert_int_eq(d.bits[LOW], test * -1);
  ck_assert_int_eq(get_sign(&d), 1);
  ck_assert_int_eq(d.bits[MIDDLE], 0);
  ck_assert_int_eq(d.bits[HIGH], 0);
}
END_TEST

START_TEST(from_float_to_decimal) {
  s21_decimal d;
  zeroing_and_normalize(&d);
  float test = 123.4;
  s21_from_float_to_decimal(test, &d);
  ck_assert_int_eq(d.bits[LOW], 1234);
  ck_assert_int_eq(get_scale(&d), 1);

  test = -123.4;
  s21_from_float_to_decimal(test, &d);
  ck_assert_int_eq(d.bits[LOW], 1234);
  ck_assert_int_eq(get_scale(&d), 1);
  ck_assert_int_eq(get_sign(&d), 1);

  test = 0;
  s21_from_float_to_decimal(test, &d);
  ck_assert_int_eq(d.bits[LOW], 0);
  ck_assert_int_eq(get_scale(&d), 0);
  ck_assert_int_eq(get_sign(&d), 0);

  test = 55.005;
  s21_from_float_to_decimal(test, &d);
  ck_assert_int_eq(d.bits[LOW], 55005);
  ck_assert_int_eq(get_scale(&d), 3);

  test = 55.005;
  int a = s21_from_float_to_decimal(test, NULL);
  ck_assert_int_eq(a, 1);

  // a = s21_from_float_to_decimal(INFINITY, &d);
  // ck_assert_int_eq(a, 1);
}
END_TEST

START_TEST(from_decimal_to_int) {
  s21_decimal d;
  int test = 0;
  zeroing_and_normalize(&d);
  d.bits[LOW] = 0b00000000000000000000000000000001;
  s21_from_decimal_to_int(d, &test);
  ck_assert_int_eq(test, 1);

  d.bits[LOW] = 0b0000000000000011110001001000000;
  s21_from_decimal_to_int(d, &test);
  ck_assert_int_eq(test, 123456);

  d.bits[LOW] = 0b0000000000000000000000000000011;
  set_sign(&d, NEGATIVE);
  s21_from_decimal_to_int(d, &test);
  ck_assert_int_eq(test, -3);

  d.bits[LOW] = 0b0000000000000000000000000000011;
  set_sign(&d, NEGATIVE);
  int x = s21_from_decimal_to_int(d, NULL);
  ck_assert_int_eq(x, 1);

  d.bits[LOW] = 0b0000000000000000000000000100001;
  set_sign(&d, NEGATIVE);
  set_scale(&d, 1);
  s21_from_decimal_to_int(d, &test);
  ck_assert_int_eq(test, -3);

  d.bits[LOW] = 0b0000000000000000000000000100001;
  set_sign(&d, POSITIVE);
  set_scale(&d, 1);
  s21_from_decimal_to_int(d, &test);
  ck_assert_int_eq(test, 3);
}
END_TEST

START_TEST(from_decimal_to_float) {
  s21_decimal d;
  float test = 0;
  zeroing_and_normalize(&d);

  d.bits[LOW] = 0b000000000000000000000000000010000;
  set_scale(&d, 6);
  s21_from_decimal_to_float(d, &test);
  ck_assert_int_eq(0.000016f == test, 1);

  d.bits[LOW] = 0b000000000000001110010000100010000;
  set_scale(&d, 3);
  s21_from_decimal_to_float(d, &test);
  ck_assert_int_eq(467.216f == test, 1);

  d.bits[LOW] = 0b00000000000000000000000000000001;
  set_scale(&d, 1);
  s21_from_decimal_to_float(d, &test);
  ck_assert_int_eq(0.1f == test, 1);
}
END_TEST

START_TEST(round_) {
  s21_decimal d, r;
  zeroing_and_normalize(&d);
  zeroing_and_normalize(&r);

  init_max_decimal(&d);
  set_scale(&d, 27);  // 79.228162514264337593543950335
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 79);

  s21_from_int_to_decimal(11199, &d);
  set_scale(&d, 2);
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 112);

  s21_from_int_to_decimal(1235, &d);
  set_scale(&d, 1);
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 124);

  s21_from_int_to_decimal(555499, &d);
  set_scale(&d, 3);
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 555);

  s21_from_int_to_decimal(-1235, &d);
  set_scale(&d, 1);
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 124);
  ck_assert_int_eq(get_sign(&r), 1);

  s21_from_int_to_decimal(-1234, &d);
  set_scale(&d, 1);
  r = s21_round(d);
  ck_assert_int_eq(r.bits[LOW], 123);
  ck_assert_int_eq(get_sign(&r), 1);
}
END_TEST

START_TEST(floor_) {
  s21_decimal d, r;
  zeroing_and_normalize(&d);
  zeroing_and_normalize(&r);

  init_max_decimal(&d);
  set_scale(&d, 27);  // 79.228162514264337593543950335
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 79);

  s21_from_int_to_decimal(11199, &d);
  set_scale(&d, 2);
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 111);

  s21_from_int_to_decimal(1235, &d);
  set_scale(&d, 1);
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 123);

  s21_from_int_to_decimal(555499, &d);
  set_scale(&d, 3);
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 555);

  s21_from_int_to_decimal(-1235, &d);
  set_scale(&d, 1);
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 124);
  ck_assert_int_eq(get_sign(&r), 1);

  s21_from_int_to_decimal(-1234, &d);
  set_scale(&d, 1);
  r = s21_floor(d);
  ck_assert_int_eq(r.bits[LOW], 124);
  ck_assert_int_eq(get_sign(&r), 1);
}
END_TEST

START_TEST(truncate) {
  s21_decimal d, r;
  zeroing_and_normalize(&d);
  zeroing_and_normalize(&r);
  init_max_decimal(&d);
  set_scale(&d, 27);  // 79.228162514264337593543950335
  r = s21_truncate(d);
  ck_assert_int_eq(r.bits[LOW], 79);

  s21_from_int_to_decimal(250, &d);
  r = s21_truncate(d);
  ck_assert_int_eq(r.bits[LOW], 250);

  s21_from_int_to_decimal(-1234, &d);
  set_scale(&d, 1);
  r = s21_truncate(d);
  ck_assert_int_eq(r.bits[LOW], 123);
  ck_assert_int_eq(get_sign(&r), 1);
}
END_TEST

START_TEST(negate) {
  int x = 100;
  s21_decimal d, r;
  zeroing_and_normalize(&r);
  s21_from_int_to_decimal(x, &d);
  r = s21_negate(d);
  s21_from_decimal_to_int(r, &x);
  ck_assert_int_eq(-100, x);

  x = -100;
  s21_from_int_to_decimal(x, &d);
  r = s21_negate(d);
  s21_from_decimal_to_int(r, &x);
  ck_assert_int_eq(100, x);

  zeroing_and_normalize(&d);
  d.value_type = s21_INFINITY;
  r = s21_negate(d);
  ck_assert_int_eq(r.value_type, s21_NEGATIVE_INFINITY);

  zeroing_and_normalize(&d);
  d.value_type = s21_NEGATIVE_INFINITY;
  r = s21_negate(d);
  ck_assert_int_eq(r.value_type, s21_INFINITY);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_decimal_lib");
  tc_core = tcase_create("Result");

  // arithmetic_functions
  tcase_add_test(tc_core, add);
  tcase_add_test(tc_core, sub);
  tcase_add_test(tc_core, mul);
  tcase_add_test(tc_core, div_);
  tcase_add_test(tc_core, mod);

  // compare_function

  tcase_add_test(tc_core, is_less);
  tcase_add_test(tc_core, is_less_or_equal);
  tcase_add_test(tc_core, is_greather);
  tcase_add_test(tc_core, is_greather_or_equal);
  tcase_add_test(tc_core, is_equal);
  tcase_add_test(tc_core, is_not_equal);

  // conversion_function

  tcase_add_test(tc_core, from_int_to_decimal);
  tcase_add_test(tc_core, from_float_to_decimal);
  tcase_add_test(tc_core, from_decimal_to_int);
  tcase_add_test(tc_core, from_decimal_to_float);

  // another__functions
  tcase_add_test(tc_core, truncate);
  tcase_add_test(tc_core, negate);
  tcase_add_test(tc_core, round_);
  tcase_add_test(tc_core, floor_);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int no_failed = 0;
  Suite *s;
  SRunner *runner;

  s = s21_decimal_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_VERBOSE);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? 0 : 1;
}
