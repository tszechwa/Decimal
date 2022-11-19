#include "s21_decimal.h"

const values_table_t ADDITIONAL_VALUES_TABLE[TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, s21_INFINITY},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY},
    {s21_NORMAL_VALUE, s21_NAN, s21_NAN},

    {s21_INFINITY, s21_INFINITY, s21_INFINITY},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_INFINITY, s21_NAN, s21_NAN},

    {s21_NEGATIVE_INFINITY, s21_INFINITY, s21_NAN},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY},
    {s21_NEGATIVE_INFINITY, s21_NAN, s21_NAN},

    {s21_NAN, s21_INFINITY, s21_NAN},
    {s21_NAN, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_NAN, s21_NAN, s21_NAN}};

const values_table_t SUBTRACTION_VALUES_TABLE[TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, s21_NAN},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, s21_INFINITY},
    {s21_NORMAL_VALUE, s21_NAN, s21_NAN},

    {s21_INFINITY, s21_INFINITY, s21_NAN},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY},
    {s21_INFINITY, s21_NAN, s21_NAN},

    {s21_NEGATIVE_INFINITY, s21_INFINITY, s21_INFINITY},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_NEGATIVE_INFINITY, s21_NAN, s21_NAN},

    {s21_NAN, s21_INFINITY, s21_NAN},
    {s21_NAN, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_NAN, s21_NAN, s21_NAN}};

const values_table_t MULTIPLICATION_VALUES_TABLE[TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, s21_INFINITY},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY},
    {s21_NORMAL_VALUE, s21_NAN, s21_NAN},

    {s21_INFINITY, s21_INFINITY, s21_INFINITY},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_INFINITY, s21_NAN, s21_NAN},

    {s21_NEGATIVE_INFINITY, s21_INFINITY, s21_NAN},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, s21_INFINITY},
    {s21_NEGATIVE_INFINITY, s21_NAN, s21_NAN},

    {s21_NAN, s21_INFINITY, s21_NAN},
    {s21_NAN, s21_NEGATIVE_INFINITY, s21_NAN},
    {s21_NAN, s21_NAN, s21_NAN}};

long long s21_int_pow(int base, int exp) {
  long long res = 1;
  for (; exp > 0; exp--) {
    res *= base;
  }
  return res;
}

int get_bit_by_index(s21_decimal *decimal, const int index) {
  return (decimal->bits[index / INT_SIZE] & (1U << (index % INT_SIZE))) != 0;
}

void set_bit_by_index(s21_decimal *decimal, int index) {
  uint mask = 1U << (index % SIZE_INT_IN_BITS);
  if (index <= BITS_LOW_MAX) {
    decimal->bits[LOW] |= mask;
  } else if (index <= BITS_MIDDLE_MAX) {
    decimal->bits[MIDDLE] |= mask;
  } else if (index <= BITS_HIGH_MAX) {
    decimal->bits[HIGH] |= mask;
  } else if (index <= BITS_SCALE_MAX) {
    decimal->bits[SCALE] |= mask;
  }
}

void remove_bit_by_index(s21_decimal *decimal, const int index) {
  uint mask = ~(1U << (index % SIZE_INT_IN_BITS));
  if (index <= BITS_LOW_MAX) {
    decimal->bits[LOW] &= mask;
  } else if (index <= BITS_MIDDLE_MAX) {
    decimal->bits[MIDDLE] &= mask;
  } else if (index <= BITS_HIGH_MAX) {
    decimal->bits[HIGH] &= mask;
  } else if (index <= BITS_SCALE_MAX) {
    decimal->bits[SCALE] &= mask;
  }
}

void set_sign(s21_decimal *decimal, sign_values_t sign_value) {
  if (sign_value == NEGATIVE) {
    set_bit_by_index(decimal, SIGN_BIT_INDEX_IN_DECIMAL);
  } else {
    remove_bit_by_index(decimal, SIGN_BIT_INDEX_IN_DECIMAL);
  }
}

sign_values_t get_sign(s21_decimal *decimal) {
  uint mask = 1U << (SIGN_BIT_INDEX % SIZE_INT_IN_BITS);
  int sign = POSITIVE;
  if (decimal->bits[SCALE] & mask) {
    sign = NEGATIVE;
  }
  return sign;
}

int get_scale(s21_decimal *decimal) {
  return (decimal->bits[SCALE] & SCALE_MASK) >> SCALE_BIT_MIN;
}

void set_scale(s21_decimal *decimal, const int scale) {
  sign_values_t current_sign = get_sign(decimal);
  if (scale <= SCALE_MAX) {
    decimal->bits[SCALE] = scale << SCALE_BIT_MIN;
  }
  if (current_sign == NEGATIVE) {
    set_sign(decimal, NEGATIVE);
  }
}

void zeroing_decimal(s21_decimal *decimal) {
  if (NULL != decimal) {
    decimal->bits[LOW] = 0;
    decimal->bits[MIDDLE] = 0;
    decimal->bits[HIGH] = 0;
    decimal->bits[SCALE] = 0;
  }
}

void zeroing_and_normalize(s21_decimal *decimal) {
  zeroing_decimal(decimal);
  decimal->value_type = s21_NORMAL_VALUE;
}

int s21_is_nan(double x) { return x != x; }

int s21_is_pos_inf(double x) { return x == s21_POS_INF; }

int s21_is_neg_inf(double x) { return x == s21_NEG_INF; }

int s21_abs(int n) {
  if (n < 0) {
    n *= -1;
  }
  return n;
}

int get_result_scale(s21_decimal *first_decimal, s21_decimal *second_decimal) {
  int first_scale = get_scale(first_decimal),
      second_scale = get_scale(second_decimal), result_scale = 0;
  if (first_scale > second_scale) {
    result_scale = first_scale;
  } else if (first_scale < second_scale) {
    result_scale = second_scale;
  } else {
    result_scale = first_scale;
  }
  return result_scale;
}

int get_result_scale_for_mul(s21_decimal *first_decimal,
                             s21_decimal *second_decimal) {
  int first_scale = get_scale(first_decimal),
      second_scale = get_scale(second_decimal);
  return first_scale + second_scale;
}

int get_result_scale_for_div(s21_decimal *first_decimal,
                             s21_decimal *second_decimal) {
  int first_scale = get_scale(first_decimal),
      second_scale = get_scale(second_decimal);
  return s21_abs(first_scale - second_scale);
}

sign_values_t get_result_sign_for_add(s21_decimal first_decimal,
                                      s21_decimal second_decimal) {
  sign_values_t result_sign = POSITIVE;
  if (is_signs_equal(&first_decimal, &second_decimal) == EQUAL) {
    result_sign = get_sign(&first_decimal);
  } else {
    sign_values_t first_sign = get_sign(&first_decimal),
                  second_sign = get_sign(&second_decimal);
    compare_values_t abs_compare_result =
        compare_abs_decimals_by_bits(&first_decimal, &second_decimal);
    if (abs_compare_result == FIRST_BIGGER) {
      if (first_sign == POSITIVE && second_sign == NEGATIVE) {
        result_sign = POSITIVE;
      } else {
        result_sign = NEGATIVE;
      }
    } else {
      if (first_sign == NEGATIVE && second_sign == POSITIVE) {
        result_sign = POSITIVE;
      } else {
        result_sign = NEGATIVE;
      }
    }
  }
  return result_sign;
}

sign_values_t get_result_sign_for_sub(s21_decimal first_decimal,
                                      s21_decimal second_decimal) {
  sign_values_t result_sign = POSITIVE, first_sign = get_sign(&first_decimal),
                second_sign = get_sign(&second_decimal);
  compare_values_t abs_compare_result =
      compare_abs_decimals_by_bits(&first_decimal, &second_decimal);
  if (abs_compare_result == FIRST_BIGGER) {
    if (first_sign == POSITIVE && second_sign == POSITIVE) {
      result_sign = POSITIVE;
    } else if (first_sign == POSITIVE && second_sign == NEGATIVE) {
      result_sign = POSITIVE;
    } else if (first_sign == NEGATIVE && second_sign == NEGATIVE) {
      result_sign = NEGATIVE;
    } else {
      result_sign = NEGATIVE;
    }
  } else if (abs_compare_result == SECOND_BIGGER) {
    if (first_sign == POSITIVE && second_sign == POSITIVE) {
      result_sign = NEGATIVE;
    } else if (first_sign == POSITIVE && second_sign == NEGATIVE) {
      result_sign = POSITIVE;
    } else if (first_sign == NEGATIVE && second_sign == NEGATIVE) {
      result_sign = POSITIVE;
    } else {
      result_sign = NEGATIVE;
    }
  } else if (abs_compare_result == EQUAL) {
    if (first_sign == NEGATIVE && second_sign == POSITIVE) {
      result_sign = NEGATIVE;
    } else if (first_sign == POSITIVE && second_sign == NEGATIVE) {
      result_sign = POSITIVE;
    }
  }
  return result_sign;
}

sign_values_t get_result_sign_for_mul(s21_decimal first_decimal,
                                      s21_decimal second_decimal) {
  sign_values_t result_sign = POSITIVE, first_sign = get_sign(&first_decimal),
                second_sign = get_sign(&second_decimal);
  if (first_sign == POSITIVE && second_sign == POSITIVE) {
    result_sign = POSITIVE;
  } else if ((first_sign == POSITIVE && second_sign == NEGATIVE) ||
             (first_sign == NEGATIVE && second_sign == POSITIVE)) {
    result_sign = NEGATIVE;
  } else {
    result_sign = POSITIVE;
  }
  return result_sign;
}

sign_values_t get_result_sign_for_div(s21_decimal *first_decimal,
                                      s21_decimal *second_decimal) {
  sign_values_t result_sign = POSITIVE, first_sign = get_sign(first_decimal),
                second_sign = get_sign(second_decimal);
  if (first_sign == POSITIVE && second_sign == POSITIVE) {
    result_sign = POSITIVE;
  } else if ((first_sign == POSITIVE && second_sign == NEGATIVE) ||
             (first_sign == NEGATIVE && second_sign == POSITIVE)) {
    result_sign = NEGATIVE;
  } else {
    result_sign = POSITIVE;
  }
  return result_sign;
}

compare_values_t is_signs_equal(s21_decimal *first_decimal,
                                s21_decimal *second_decimal) {
  compare_values_t result = EQUAL;
  int first_sign = get_sign(first_decimal),
      second_sign = get_sign(second_decimal);
  if (first_sign != second_sign) {
    result = NOT_EQUAL;
  }
  return result;
}

void copy_decimal(s21_decimal *src, s21_decimal *dst) {
  dst->bits[LOW] = src->bits[LOW];
  dst->bits[MIDDLE] = src->bits[MIDDLE];
  dst->bits[HIGH] = src->bits[HIGH];
  dst->bits[SCALE] = src->bits[SCALE];
  dst->value_type = src->value_type;
}

compare_values_t is_equal_scales(s21_decimal *first_decimal,
                                 s21_decimal *second_decimal) {
  compare_values_t compare_result = NOT_EQUAL;
  if (get_scale(first_decimal) == get_scale(second_decimal)) {
    compare_result = EQUAL;
  }
  return compare_result;
}

void to_zero_scale_for_long_decimals(s21_long_decimal *first_decimal,
                                     s21_long_decimal *second_decimal) {
  s21_long_decimal ten_decimal, first_decimal_copy, second_decimal_copy;
  zeroing_and_normalize_long_decimal(&ten_decimal);
  copy_long_decimal(first_decimal, &first_decimal_copy);
  copy_long_decimal(second_decimal, &second_decimal_copy);
  int first_scale = get_long_scale(first_decimal),
      second_scale = get_long_scale(second_decimal), delta_scale = 0;
  set_long_decimal_bit_by_index(&ten_decimal, 1);
  set_long_decimal_bit_by_index(&ten_decimal, 3);

  set_long_scale(&first_decimal_copy, 0);
  set_long_scale(&second_decimal_copy, 0);

  if (first_scale > second_scale) {
    delta_scale = first_scale - second_scale;
    for (int i = 0; i < delta_scale; i++) {
      second_decimal_copy =
          mul_long_decimals_by_bits(second_decimal_copy, ten_decimal);
    }
  } else {
    delta_scale = second_scale - first_scale;
    for (int i = 0; i < delta_scale; i++) {
      first_decimal_copy =
          mul_long_decimals_by_bits(first_decimal_copy, ten_decimal);
    }
  }
  copy_long_decimal(&first_decimal_copy, first_decimal);
  copy_long_decimal(&second_decimal_copy, second_decimal);
}

bool_t is_zero_decimal(s21_decimal *decimal) {
  bool_t flag = FALSE;
  if (decimal->bits[LOW] == 0 && decimal->bits[MIDDLE] == 0 &&
      decimal->bits[HIGH] == 0) {
    flag = TRUE;
  }
  return flag;
}

void zeroing_and_normalize_long_decimal(s21_long_decimal *decimal) {
  for (int i = 0; i < 8; i++) {
    decimal->bits[i] = 0;
  }
  decimal->value_type = s21_NORMAL_VALUE;
}

void decimal_to_long_decimal(s21_decimal *decimal,
                             s21_long_decimal *long_decimal) {
  long_decimal->bits[LOW] = decimal->bits[LOW];
  long_decimal->bits[MIDDLE] = decimal->bits[MIDDLE];
  long_decimal->bits[HIGH] = decimal->bits[HIGH];
  long_decimal->bits[3] = 0;
  long_decimal->bits[4] = 0;
  long_decimal->bits[5] = 0;
  long_decimal->bits[6] = 0;
  long_decimal->bits[7] = decimal->bits[SCALE];
}

void long_decimal_to_decimal(s21_long_decimal *long_decimal,
                             s21_decimal *decimal) {
  decimal->bits[LOW] = long_decimal->bits[LOW];
  decimal->bits[MIDDLE] = long_decimal->bits[MIDDLE];
  decimal->bits[HIGH] = long_decimal->bits[HIGH];
  decimal->bits[SCALE] = long_decimal->bits[7];
}

void copy_long_decimal(s21_long_decimal *src, s21_long_decimal *dst) {
  for (int i = 0; i < 8; i++) {
    dst->bits[i] = src->bits[i];
  }
  dst->value_type = src->value_type;
}

void zeroing_long_decimal(s21_long_decimal *decimal) {
  for (int i = 0; i < 8; i++) {
    decimal->bits[i] = 0;
  }
}

s21_long_decimal shift_long_decimal_bits_to_left(s21_long_decimal decimal,
                                                 const int bits) {
  s21_long_decimal result;
  zeroing_and_normalize_long_decimal(&result);
  for (int i = LONG_DECIMAL_MIN; i <= LONG_DECIMAL_MAX; i++) {
    if (get_long_decimal_bit_by_index(&decimal, i) == 1) {
      if (i + bits <= LONG_DECIMAL_MAX) {
        set_long_decimal_bit_by_index(&result, i + bits);
      }
    }
  }
  return result;
}

s21_long_decimal add_long_by_bits(s21_long_decimal first_decimal,
                                  s21_long_decimal second_decimal) {
  s21_long_decimal result;
  zeroing_and_normalize_long_decimal(&result);
  int buffer_bit = 0;
  for (int i = LONG_DECIMAL_MIN; i <= LONG_DECIMAL_MAX; i++) {
    int bit1 = get_long_decimal_bit_by_index(&first_decimal, i);
    int bit2 = get_long_decimal_bit_by_index(&second_decimal, i);
    if (bit1 == 0 && bit2 == 0) {
      if (buffer_bit == 1) {
        buffer_bit = 0;
        set_long_decimal_bit_by_index(&result, i);
      }
    } else if ((bit1 == 0 && bit2 == 1) || (bit1 == 1 && bit2 == 0)) {
      if (buffer_bit != 1) {
        set_long_decimal_bit_by_index(&result, i);
      }
    } else {
      if (buffer_bit == 1) {
        set_long_decimal_bit_by_index(&result, i);
      }
      buffer_bit = 1;
    }
  }
  return result;
}

s21_long_decimal sub_long_by_bits(s21_long_decimal first_decimal,
                                  s21_long_decimal second_decimal) {
  s21_long_decimal bigger_decimal, smaller_decimal, result;
  zeroing_and_normalize_long_decimal(&result);
  if (compare_abs_long_decimals_by_bits(first_decimal, second_decimal) ==
      FIRST_BIGGER) {
    copy_long_decimal(&first_decimal, &bigger_decimal);
    copy_long_decimal(&second_decimal, &smaller_decimal);
  } else {
    copy_long_decimal(&second_decimal, &bigger_decimal);
    copy_long_decimal(&first_decimal, &smaller_decimal);
  }
  for (int i = LONG_DECIMAL_MIN; i <= LONG_DECIMAL_MAX; i++) {
    int j = 0;
    int bit1 = get_long_decimal_bit_by_index(&bigger_decimal, i);
    int bit2 = get_long_decimal_bit_by_index(&smaller_decimal, i);
    if (bit1 == 1 && bit2 == 0) {
      set_long_decimal_bit_by_index(&result, i);
    } else if (bit1 == 0 && bit2 == 1) {
      set_long_decimal_bit_by_index(&result, i);
      j = i + 1;
      while (get_long_decimal_bit_by_index(&bigger_decimal, j) == 0 &&
             j <= LONG_DECIMAL_MAX) {
        set_long_decimal_bit_by_index(&bigger_decimal, j);
        j++;
      }
      remove_long_decimal_bit_by_index(&bigger_decimal, j);
    }
  }
  return result;
}

s21_long_decimal mul_long_decimals_by_bits(s21_long_decimal first_decimal,
                                           s21_long_decimal second_decimal) {
  s21_long_decimal result, buffer_sum;
  zeroing_and_normalize_long_decimal(&result);
  zeroing_and_normalize_long_decimal(&buffer_sum);
  for (int i = LONG_DECIMAL_MIN; i <= LONG_DECIMAL_MAX; i++) {
    int second_bit = get_long_decimal_bit_by_index(&second_decimal, i);
    for (int j = LONG_DECIMAL_MIN; j <= LONG_DECIMAL_MAX; j++) {
      int first_bit = get_long_decimal_bit_by_index(&first_decimal, j);
      if (first_bit == 1 && second_bit == 1) {
        set_long_decimal_bit_by_index(&buffer_sum, j);
      }
    }
    buffer_sum = shift_long_decimal_bits_to_left(buffer_sum, i);
    result = add_long_by_bits(result, buffer_sum);
    zeroing_long_decimal(&buffer_sum);
  }

  return result;
}

s21_long_decimal div_mod(s21_long_decimal *first_decimal,
                         s21_long_decimal *second_decimal, int max_iterations,
                         int *len) {
  s21_long_decimal result, buffer, remainder, long_second_decimal,
      first_decimal_copy;
  zeroing_and_normalize_long_decimal(&long_second_decimal);
  copy_long_decimal(second_decimal, &long_second_decimal);
  copy_long_decimal(first_decimal, &first_decimal_copy);
  zeroing_and_normalize_long_decimal(&result);
  zeroing_and_normalize_long_decimal(&buffer);
  zeroing_and_normalize_long_decimal(&remainder);

  int first_decimal_len = get_long_decimal_len(first_decimal);
  int iteration = 0;
  bool_t skip_check = FALSE;
  int scale = 0;
  int shifts_quantity = 0;
  while (iteration < max_iterations) {
    int buffer_len = 0;
    int buffer_shifts_quantity = 0;
    bool_t use_buffer_bits = FALSE;
    buffer_shifts_quantity = 0;
    int first_decimal_len_copy = get_long_decimal_len(first_decimal);
    int buffer_iteration = 0;
    while ((compare_abs_long_decimals_by_bits(long_second_decimal, buffer) ==
                FIRST_BIGGER &&
            first_decimal_len >= 0) ||
           skip_check == TRUE) {
      int first_decimal_bit =
          get_long_decimal_bit_by_index(first_decimal, first_decimal_len);
      skip_check = FALSE;
      if (first_decimal_bit == 1) {
        set_long_decimal_bit_by_index(&buffer, 0);
      }
      if (compare_abs_long_decimals_by_bits(long_second_decimal, buffer) ==
          FIRST_BIGGER) {
        buffer = shift_long_decimal_bits_to_left(buffer, 1);
        skip_check = TRUE;
      }
      if (first_decimal_len < 0) {
        buffer_shifts_quantity++;
        use_buffer_bits = TRUE;
      }
      scale++;
      first_decimal_len--;
      buffer_iteration++;
    }

    buffer_len = get_long_decimal_len(&buffer);

    result =
        shift_long_decimal_bits_to_left(result, buffer_shifts_quantity - 1);
    shifts_quantity += buffer_shifts_quantity - 1;

    zeroing_long_decimal(&remainder);
    remainder = sub_long_by_bits(buffer, long_second_decimal);

    copy_long_decimal(&remainder, first_decimal);
    first_decimal_len = get_long_decimal_len(first_decimal);

    result = shift_long_decimal_bits_to_left(result, 1);
    shifts_quantity++;
    set_long_decimal_bit_by_index(&result, 0);

    zeroing_long_decimal(&buffer);
    iteration++;
  }

  *len = shifts_quantity;
  return result;
}

s21_long_decimal div_long_by_bits(s21_long_decimal *first_decimal,
                                  s21_long_decimal *second_decimal,
                                  s21_long_decimal *mod_) {
  s21_long_decimal result, buffer, remainder, long_second_decimal,
      first_decimal_copy, mod;
  zeroing_and_normalize_long_decimal(&long_second_decimal);
  copy_long_decimal(second_decimal, &long_second_decimal);
  copy_long_decimal(first_decimal, &first_decimal_copy);
  zeroing_and_normalize_long_decimal(&result);
  zeroing_and_normalize_long_decimal(&buffer);
  zeroing_and_normalize_long_decimal(&remainder);
  zeroing_and_normalize_long_decimal(&mod);

  int first_decimal_len = get_long_decimal_len(first_decimal);
  int iteration = 0;
  bool_t skip_check = FALSE;
  while (is_zero_long_decimal(first_decimal) == FALSE) {
    int buffer_len = 0;
    bool_t use_buffer_bits = FALSE;
    int first_decimal_len_copy = get_long_decimal_len(first_decimal);
    while ((compare_abs_long_decimals_by_bits(long_second_decimal, buffer) ==
                FIRST_BIGGER &&
            first_decimal_len >= 0) ||
           skip_check == TRUE) {
      int first_decimal_bit =
          get_long_decimal_bit_by_index(first_decimal, first_decimal_len);
      skip_check = FALSE;
      if (first_decimal_bit == 1) {
        set_long_decimal_bit_by_index(&buffer, 0);
      }
      if (compare_abs_long_decimals_by_bits(long_second_decimal, buffer) ==
          FIRST_BIGGER) {
        buffer = shift_long_decimal_bits_to_left(buffer, 1);
        skip_check = TRUE;
      }
      if (first_decimal_len < 0) {
        use_buffer_bits = TRUE;
      }
      first_decimal_len--;
    }

    buffer_len = get_long_decimal_len(&buffer);
    int delta = first_decimal_len_copy - buffer_len;
    zeroing_long_decimal(&remainder);

    remainder = sub_long_by_bits(buffer, long_second_decimal);
    int remainder_len = get_long_decimal_len(&remainder);

    for (int i = LONG_DECIMAL_MAX; i > first_decimal_len; i--) {
      remove_long_decimal_bit_by_index(first_decimal, i);
    }

    int first_decimal_len_start = first_decimal_len;
    if (is_zero_long_decimal(&remainder) == FALSE) {
      for (int i = 0; i <= remainder_len; i++) {
        if (get_long_decimal_bit_by_index(&remainder, i)) {
          set_long_decimal_bit_by_index(first_decimal,
                                        first_decimal_len_start + i + 1);
        }
        first_decimal_len++;
      }
    }
    if (first_decimal_len > 0) {
      copy_long_decimal(&remainder, &mod);
    }
    if ((compare_abs_long_decimals_by_bits(remainder, long_second_decimal) ==
             SECOND_BIGGER &&
         first_decimal_len <= 0 && is_zero_long_decimal(&remainder) == FALSE &&
         use_buffer_bits == TRUE) ||
        compare_abs_long_decimals_by_bits(buffer, long_second_decimal) ==
                EQUAL &&
            first_decimal_len <= 0 && use_buffer_bits == TRUE ||
        compare_abs_long_decimals_by_bits(remainder, long_second_decimal) ==
                SECOND_BIGGER &&
            iteration > 0 && use_buffer_bits == TRUE) {
      copy_long_decimal(&result, mod_);
      s21_long_decimal multiply, mod_res;
      zeroing_and_normalize_long_decimal(&mod_res);
      zeroing_and_normalize_long_decimal(&multiply);
      multiply = mul_long_decimals_by_bits(result, long_second_decimal);
      mod_res = sub_long_by_bits(first_decimal_copy, multiply);
      s21_long_decimal devided_mod;
      zeroing_and_normalize_long_decimal(&devided_mod);
      int devided_mod_len = 0;
      int max_mod_len = BITS_HIGH_MAX - get_long_decimal_len(&result);
      devided_mod = div_mod(&mod_res, &long_second_decimal, max_mod_len,
                            &devided_mod_len);

      for (int i = devided_mod_len - 1; i > 0; i--) {
        if (get_long_decimal_len(&result) == BITS_HIGH_MAX) {
          break;
        }
        result = shift_long_decimal_bits_to_left(result, 1);
        if (get_long_decimal_bit_by_index(&devided_mod, i)) {
          set_long_decimal_bit_by_index(&result, 0);
        }
      }

      break;
    }

    s21_long_decimal remainder_copy;
    copy_long_decimal(&remainder, &remainder_copy);

    set_long_decimal_bit_by_index(&result, 0);

    while (compare_abs_long_decimals_by_bits(
               remainder_copy, long_second_decimal) == SECOND_BIGGER &&
           delta > 0) {
      remainder_copy = shift_long_decimal_bits_to_left(remainder_copy, 1);
      if (get_long_decimal_bit_by_index(first_decimal, delta - 1)) {
        set_long_decimal_bit_by_index(&remainder_copy, 0);
      }
      result = shift_long_decimal_bits_to_left(result, 1);
      delta--;
    }

    if (is_zero_long_decimal(first_decimal) && first_decimal_len_copy == 0) {
      while (delta >= 0) {
        result = shift_long_decimal_bits_to_left(result, 1);
        delta--;
      }
    }
    zeroing_long_decimal(&buffer);
    iteration++;
  }

  return result;
}

int get_long_scale(s21_long_decimal *decimal) {
  return (decimal->bits[7] & SCALE_MASK) >> SCALE_BIT_MIN;
}

void set_long_scale(s21_long_decimal *decimal, const int scale) {
  if (scale <= SCALE_MAX) {
    decimal->bits[7] = scale << SCALE_BIT_MIN;
  }
}

int get_long_decimal_bit_by_index(s21_long_decimal *decimal, const int index) {
  return (decimal->bits[index / INT_SIZE] & (1U << (index % INT_SIZE))) != 0;
}

void set_long_decimal_bit_by_index(s21_long_decimal *long_decimal,
                                   const int index) {
  long_decimal->bits[index / INT_SIZE] |= (1U << (index % INT_SIZE));
}

bool_t has_overflow(s21_long_decimal *long_decimal) {
  bool_t flag = FALSE;
  if (long_decimal->bits[3] != 0 || long_decimal->bits[4] != 0 ||
      long_decimal->bits[5] != 0 || long_decimal->bits[6] != 0) {
    flag = TRUE;
  }
  return flag;
}

void remove_long_decimal_bit_by_index(s21_long_decimal *decimal,
                                      const int index) {
  decimal->bits[index / INT_SIZE] &= ~(1U << (index % INT_SIZE));
}

int get_long_decimal_len(s21_long_decimal *decimal) {
  int index = 0;
  for (int i = LONG_DECIMAL_MAX; i >= LONG_DECIMAL_MIN; i--) {
    if (get_long_decimal_bit_by_index(decimal, i)) {
      index = i;
      break;
    }
  }
  return index;
}

bool_t is_zero_long_decimal(s21_long_decimal *decimal) {
  bool_t flag = TRUE;
  for (int i = 0; i < 8; i++) {
    if (decimal->bits[i] != 0) {
      flag = FALSE;
    }
  }
  return flag;
}

s21_decimal add_by_value_type(value_type_t first_value_type,
                              value_type_t second_value_type) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  for (int i = 0; i < TABLE_ROWS; i++) {
    int first_value = ADDITIONAL_VALUES_TABLE[i].first_value;
    int second_value = ADDITIONAL_VALUES_TABLE[i].second_value;
    int result_value_type = ADDITIONAL_VALUES_TABLE[i].result_value;
    if (first_value_type == first_value && second_value_type == second_value ||
        first_value_type == second_value && second_value_type == first_value) {
      result.value_type = result_value_type;
      break;
    }
  }
  return result;
}

s21_decimal sub_by_value_type(value_type_t first_value_type,
                              value_type_t second_value_type) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (first_value_type == s21_NEGATIVE_INFINITY &&
      second_value_type == s21_INFINITY) {
    result.value_type = s21_INFINITY;
  } else {
    for (int i = 0; i < TABLE_ROWS; i++) {
      int first_value = SUBTRACTION_VALUES_TABLE[i].first_value;
      int second_value = SUBTRACTION_VALUES_TABLE[i].second_value;
      int result_value_type = SUBTRACTION_VALUES_TABLE[i].result_value;
      if (first_value_type == first_value &&
              second_value_type == second_value ||
          first_value_type == second_value &&
              second_value_type == first_value) {
        result.value_type = result_value_type;
        break;
      }
    }
  }
  return result;
}

s21_decimal mul_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  value_type_t first_value_type = first_decimal.value_type;
  value_type_t second_value_type = second_decimal.value_type;

  sign_values_t first_sign = get_sign(&first_decimal);
  sign_values_t second_sign = get_sign(&second_decimal);

  if (first_value_type == s21_NORMAL_VALUE &&
      first_sign == POSITIVE) {  //  Первый нормальный положительный
    if (second_value_type == s21_INFINITY) {
      result.value_type = s21_INFINITY;
    } else if (second_value_type == s21_NEGATIVE_INFINITY) {
      result.value_type = s21_NEGATIVE_INFINITY;
    } else {
      result.value_type = s21_NAN;
    }
  } else if (first_value_type == s21_NORMAL_VALUE &&
             first_sign == NEGATIVE) {  //  Первый нормальный отрицательный
    if (second_value_type == s21_INFINITY) {
      result.value_type = s21_NEGATIVE_INFINITY;
    } else if (second_value_type == s21_NEGATIVE_INFINITY) {
      result.value_type = s21_INFINITY;
    } else {
      result.value_type = s21_NAN;
    }
  } else if (first_value_type == s21_INFINITY &&
             second_value_type == s21_NORMAL_VALUE) {
    if (second_sign == POSITIVE) {
      result.value_type = s21_INFINITY;
    } else if (second_sign == NEGATIVE) {
      result.value_type = s21_NEGATIVE_INFINITY;
    }
  } else if (first_value_type == s21_NEGATIVE_INFINITY &&
             second_value_type == s21_NORMAL_VALUE) {
    if (second_sign == POSITIVE) {
      result.value_type = s21_NEGATIVE_INFINITY;
    } else if (second_sign == NEGATIVE) {
      result.value_type = s21_INFINITY;
    }
  } else if (is_zero_decimal(&first_decimal) &&
                 first_value_type == s21_NORMAL_VALUE ||
             second_value_type == s21_NORMAL_VALUE &&
                 is_zero_decimal(&second_decimal)) {
    result.value_type = s21_NAN;
  } else {
    for (int i = 0; i < TABLE_ROWS; i++) {
      int first_value = MULTIPLICATION_VALUES_TABLE[i].first_value;
      int second_value = MULTIPLICATION_VALUES_TABLE[i].second_value;
      if (first_value_type == first_value &&
              second_value_type == second_value ||
          first_value_type == second_value &&
              second_value_type == first_value) {
        result.value_type = MULTIPLICATION_VALUES_TABLE[i].result_value;;
        break;
      }
    }
  }
  return result;
}

s21_decimal div_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);

  value_type_t first_value_type = first_decimal.value_type;
  value_type_t second_value_type = second_decimal.value_type;

  sign_values_t first_sign = get_sign(&first_decimal);
  sign_values_t second_sign = get_sign(&second_decimal);

  if (first_value_type != s21_NORMAL_VALUE &&
      second_value_type != s21_NORMAL_VALUE) {
    result.value_type = s21_NAN;
  } else if (first_value_type == s21_INFINITY &&
             second_value_type == s21_NORMAL_VALUE) {
    if (second_sign == NEGATIVE) {
      set_sign(&result, NEGATIVE);
    }
  } else if (first_value_type == s21_NEGATIVE_INFINITY &&
             second_value_type == s21_NORMAL_VALUE) {
    if (second_sign == POSITIVE) {
      set_sign(&result, NEGATIVE);
    }
  } else if (first_value_type == s21_NORMAL_VALUE &&
             second_value_type == s21_INFINITY) {
    if (first_sign == NEGATIVE) {
      result.value_type = s21_NEGATIVE_INFINITY;
    } else {
      result.value_type = s21_INFINITY;
    }
  } else if (first_value_type == s21_NORMAL_VALUE &&
             second_value_type == s21_NEGATIVE_INFINITY) {
    if (first_sign == POSITIVE) {
      result.value_type = s21_NEGATIVE_INFINITY;
    } else {
      result.value_type = s21_INFINITY;
    }
  } else {
    result.value_type = s21_NAN;
  }

  return result;
}

s21_decimal mod_by_value_type(s21_decimal first_decimal,
                              s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);

  value_type_t first_value_type = first_decimal.value_type;
  value_type_t second_value_type = second_decimal.value_type;

  if ((first_value_type == s21_INFINITY ||
       first_value_type == s21_NEGATIVE_INFINITY) &&
      second_value_type == s21_NORMAL_VALUE) {
    result.value_type = s21_NORMAL_VALUE;
    if (is_zero_decimal(&second_decimal) == FALSE) {
      copy_decimal(&second_decimal, &result);
    }
  } else {
    result.value_type = s21_NAN;
  }
  return result;
}

s21_decimal zero_division(s21_decimal zero_decimal, s21_decimal decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_zero_decimal(&decimal)) {
    result.value_type = s21_NAN;
  } else if (get_sign(&decimal) == POSITIVE &&
             decimal.value_type == s21_NORMAL_VALUE) {
    result.value_type = s21_INFINITY;
  } else if (get_sign(&decimal) == NEGATIVE &&
             decimal.value_type == s21_NORMAL_VALUE) {
    result.value_type = s21_NEGATIVE_INFINITY;
  } else if (decimal.value_type == s21_INFINITY) {
    result.value_type = s21_INFINITY;
  } else if (decimal.value_type == s21_NEGATIVE_INFINITY) {
    result.value_type = s21_NEGATIVE_INFINITY;
  } else {
    result.value_type = s21_NAN;
  }
  return result;
}

void init_ten_long_decimal(s21_long_decimal *decimal) {
  zeroing_and_normalize_long_decimal(decimal);
  set_long_decimal_bit_by_index(decimal, 1);
  set_long_decimal_bit_by_index(decimal, 3);
}






