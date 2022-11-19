#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INT_SIZE 32
#ifndef NAN
#define NAN 0.0 / 0.0
#endif
#define s21_POS_INF 1.0 / 0.0
#define s21_NEG_INF -1.0 / 0.0
#define s21_NULL ((void *)0)
#define SCALE_MASK 0xFF0000
#define SIZE_INT_IN_BITS 32
#define BITS_LOW_MIN 0
#define BITS_LOW_MAX 31
#define BITS_MIDDLE_MIN 32
#define BITS_MIDDLE_MAX 63
#define BITS_HIGH_MIN 64
#define BITS_HIGH_MAX 95
#define BITS_SCALE_MIN 96
#define BITS_SCALE_MAX 127
#define SIGN_BIT_INDEX 31
#define SIGN_BIT_INDEX_IN_DECIMAL 127
#define SCALE_MAX 28
#define SCALE_BIT_MIN 16
#define SCALE_BIT_MAX 23
#define LONG_DECIMAL_MIN 0
#define LONG_DECIMAL_MAX 224
#define LONG_DECIMAL_SCALE 7
#define TABLE_ROWS 12
#define COMPARE_TABLE_ROWS 15
#define LESS_OPERATOR '<'
#define LESS_OR_EQUAL_OPERATOR 'l'
#define GREATHER_OPERATOR '>'
#define GREATHER_OR_EQUAL_OPERATOR 'g'
#define EQUAL_OPERATOR '='
#define NOT_EQUAL_OPERATOR '!'
#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef unsigned int uint;

typedef enum { FLAG_SUCCESS, FLAG_FAILURE } flag_values_t;

enum { LOW, MIDDLE, HIGH, SCALE };

typedef enum { FALSE, TRUE } bool_t;

typedef enum {
  EQUAL,
  NOT_EQUAL,
  FIRST_BIGGER,
  SECOND_BIGGER,
  COMPARE_ERROR,
} compare_values_t;

typedef enum { POSITIVE, NEGATIVE } sign_values_t;

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

typedef struct {
  uint bits[4];
  value_type_t value_type;
} s21_decimal;

typedef struct {
  uint bits[8];
  value_type_t value_type;
} s21_long_decimal;

typedef struct {
  value_type_t first_value;
  value_type_t second_value;
  value_type_t result_value;
} values_table_t;

typedef struct {
  value_type_t first_value;
  value_type_t second_value;
  bool_t compare_result;
} compare_table_t;

// Арифметические операторы

// Сложение +
s21_decimal s21_add(s21_decimal, s21_decimal);  // tested

// Вычитание -
s21_decimal s21_sub(s21_decimal, s21_decimal);  // tested

// Умножение *
s21_decimal s21_mul(s21_decimal, s21_decimal);  // tested

// Деление /
s21_decimal s21_div(s21_decimal, s21_decimal);  // tested

// Остаток от деления Mod
s21_decimal s21_mod(s21_decimal, s21_decimal);  // tested

// Операторы сравнения

// Меньше <
int s21_is_less(s21_decimal, s21_decimal);  // tested

// Меньше или равно <=
int s21_is_less_or_equal(s21_decimal, s21_decimal);  // tested

// Больше >
int s21_is_greater(s21_decimal, s21_decimal);  // tested

// Больше или равно >=
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // tested

// Равно ==
int s21_is_equal(s21_decimal, s21_decimal);  // tested

// Не равно !=
int s21_is_not_equal(s21_decimal, s21_decimal);  // tested

// Преобразователи

// Из int
int s21_from_int_to_decimal(int src, s21_decimal *dst);  // tested

// Из float
int s21_from_float_to_decimal(float src, s21_decimal *dst);  // tested

// В int
int s21_from_decimal_to_int(s21_decimal src, int *dst);  // tested

// В float
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Другие функции

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
s21_decimal s21_floor(s21_decimal);  // tested

// Округляет Decimal до ближайшего целого числа.
s21_decimal s21_round(s21_decimal);  // tested

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
s21_decimal s21_truncate(s21_decimal);  // tested

// Возвращает результат умножения указанного Decimal на -1.
s21_decimal s21_negate(s21_decimal);  // tested

// decimal utils
long long s21_int_pow(int base, int exp);
int get_bit_by_index(s21_decimal *decimal, const int index);
void set_bit_by_index(s21_decimal *decimal, const int index);
void remove_bit_by_index(s21_decimal *decimal, const int index);
void set_sign(s21_decimal *decimal, sign_values_t sign_value);
sign_values_t get_sign(s21_decimal *decimal);
void zeroing_decimal(s21_decimal *decimal);
void zeroing_and_normalize(s21_decimal *decimal);
int get_scale(s21_decimal *decimal);
void set_scale(s21_decimal *decimal, const int scale);
compare_values_t compare_decimals(s21_decimal *first_decimal,
                                  s21_decimal *second_decimal,
                                  const char operator);
compare_values_t compare_decimals_by_bits(s21_decimal *first_decimal,
                                          s21_decimal *second_decimal);
compare_values_t compare_decimals_by_value_type(s21_decimal *first_decimal,
                                                s21_decimal *second_decimal,
                                                const char operator);
compare_values_t compare_abs_decimals_by_bits(s21_decimal *first_decimal,
                                              s21_decimal *second_decimal);
compare_values_t is_equal_value_type(s21_decimal *first_decimal,
                                     s21_decimal *second_decimal);
compare_values_t is_signs_equal(s21_decimal *first_decimal,
                                s21_decimal *second_decimal);
compare_values_t is_abs_greater(s21_decimal first_decimal,
                                s21_decimal second_decimal);
compare_values_t is_equal_scales(s21_decimal *first_decimal,
                                 s21_decimal *second_decimal);
int s21_is_nan(double x);
int s21_is_pos_inf(double x);
int s21_is_neg_inf(double x);
bool_t is_same_and_normal_value_type(s21_decimal *first_decimal,
                                     s21_decimal *second_decimal);
void copy_decimal(s21_decimal *src, s21_decimal *dst);
s21_decimal add_by_bits(s21_decimal first_decimal, s21_decimal second_decimal);
s21_decimal sub_by_bits(s21_decimal first_decimal, s21_decimal second_decimal);
s21_decimal mul_by_bits(s21_decimal first_decimal, s21_decimal second_decimal);
int get_result_scale(s21_decimal *first_decimal, s21_decimal *second_decimal);
int get_result_scale_for_mul(s21_decimal *first_decimal,
                             s21_decimal *second_decimal);
int get_result_scale_for_div(s21_decimal *first_decimal,
                             s21_decimal *second_decimal);
int s21_abs(int n);
sign_values_t get_result_sign_for_add(s21_decimal first_decimal,
                                      s21_decimal second_decimal);
sign_values_t get_result_sign_for_sub(s21_decimal first_decimal,
                                      s21_decimal second_decimal);
sign_values_t get_result_sign_for_mul(s21_decimal first_decimal,
                                      s21_decimal second_decimal);
sign_values_t get_result_sign_for_div(s21_decimal *first_decimal,
                                      s21_decimal *second_decimal);
int get_decimal_len(s21_decimal *decimal);
s21_decimal shift_decimal_bits_to_left(s21_decimal decimal, const int bits);
s21_decimal shift_decimal_bits_to_right(s21_decimal decimal, const int bits);
void to_zero_scale(s21_decimal *first_decimal, s21_decimal *second_decimal);
bool_t is_zero_decimal(s21_decimal *decimal);
int get_decimal_len(s21_decimal *decimal);
bool_t has_overflow(s21_long_decimal *long_decimal);
s21_decimal add_by_value_type(value_type_t first_value_type,
                              value_type_t second_value_type);
s21_decimal sub_by_value_type(value_type_t first_value_type,
                              value_type_t second_value_type);
s21_decimal mul_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal);
s21_decimal div_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal);
s21_decimal mod_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal);
s21_decimal zero_division(s21_decimal zero_decimal, s21_decimal decimal);
bool_t is_less_by_value_type(value_type_t first_value_type,
                             value_type_t second_value_type);
bool_t is_less_or_equal_by_value_type(value_type_t first_value_type,
                                      value_type_t second_value_type);
bool_t is_greather_by_value_type(value_type_t first_value_type,
                                 value_type_t second_value_type);
bool_t is_greather_or_equal_by_value_type(value_type_t first_value_type,
                                          value_type_t second_value_type);

// long decimal utils
void set_long_decimal_bit_by_index(s21_long_decimal *long_decimal,
                                   const int index);
void zeroing_and_normalize_long_decimal(s21_long_decimal *decimal);
void zeroing_long_decimal(s21_long_decimal *decimal);
void decimal_to_long_decimal(s21_decimal *decimal,
                             s21_long_decimal *long_decimal);
void long_decimal_to_decimal(s21_long_decimal *long_decimal,
                             s21_decimal *decimal);
void copy_long_decimal(s21_long_decimal *src, s21_long_decimal *dst);
void to_zero_scale_for_long_decimals(s21_long_decimal *first_decimal,
                                     s21_long_decimal *second_decimal);
int get_long_decimal_bit_by_index(s21_long_decimal *decimal, const int index);
s21_long_decimal shift_long_decimal_bits_to_left(s21_long_decimal decimal,
                                                 const int bits);
s21_long_decimal mul_long_decimals_by_bits(s21_long_decimal first_decimal,
                                           s21_long_decimal second_decimal);
s21_long_decimal add_long_by_bits(s21_long_decimal first_decimal,
                                  s21_long_decimal second_decimal);
s21_long_decimal sub_long_by_bits(s21_long_decimal first_decimal,
                                  s21_long_decimal second_decimal);
s21_long_decimal div_long_by_bits(s21_long_decimal *first_decimal,
                                  s21_long_decimal *second_decimal,
                                  s21_long_decimal *mod_);
int get_long_scale(s21_long_decimal *decimal);
void remove_long_decimal_bit_by_index(s21_long_decimal *decimal,
                                      const int index);
void set_long_scale(s21_long_decimal *decimal, const int scale);
void print_long_decimal(s21_long_decimal *decimal);
compare_values_t compare_abs_long_decimals_by_bits(
    s21_long_decimal long_first_decimal, s21_long_decimal long_second_decimal);
int get_long_decimal_len(s21_long_decimal *decimal);
bool_t is_zero_long_decimal(s21_long_decimal *decimal);
void init_ten_long_decimal(s21_long_decimal *decimal);
s21_long_decimal mod_ten(s21_long_decimal first_decimal,
                         s21_long_decimal second_decimal);
void set_long_sign(s21_long_decimal *decimal, sign_values_t sign_value);
sign_values_t get_long_sign(s21_long_decimal *decimal);

// develop only
void print_decimal(s21_decimal *decimal);
void print_decimal_detal(s21_decimal *decimal);

#endif  // SRC_S21_DECIMAL_H_
