#include "s21_decimal.h"

s21_decimal s21_add(s21_decimal first_decimal, s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_same_and_normal_value_type(&first_decimal, &second_decimal)) {
    s21_long_decimal long_first_decimal, long_second_decimal, long_result;
    zeroing_and_normalize_long_decimal(&long_result);
    decimal_to_long_decimal(&first_decimal, &long_first_decimal);
    decimal_to_long_decimal(&second_decimal, &long_second_decimal);

    if (is_equal_scales(&first_decimal, &second_decimal) == NOT_EQUAL) {
      to_zero_scale_for_long_decimals(&long_first_decimal,
                                      &long_second_decimal);
    }

    if (is_signs_equal(&first_decimal, &second_decimal) == EQUAL) {
      long_result = add_long_by_bits(long_first_decimal, long_second_decimal);
    } else {
      long_result = sub_long_by_bits(long_first_decimal, long_second_decimal);
    }

    if (has_overflow(&long_result)) {
      zeroing_decimal(&result);
      result.value_type = s21_INFINITY;
    } else {
      long_decimal_to_decimal(&long_result, &result);
      sign_values_t result_sign =
          get_result_sign_for_add(first_decimal, second_decimal);
      int result_scale = get_result_scale(&first_decimal, &second_decimal);
      set_scale(&result, result_scale);
      set_sign(&result, result_sign);
    }
  } else {
    result =
        add_by_value_type(first_decimal.value_type, second_decimal.value_type);
  }

  return result;
}

s21_decimal s21_sub(s21_decimal first_decimal, s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_same_and_normal_value_type(&first_decimal, &second_decimal)) {
    s21_long_decimal long_first_decimal, long_second_decimal, long_result;
    zeroing_and_normalize_long_decimal(&long_result);
    decimal_to_long_decimal(&first_decimal, &long_first_decimal);
    decimal_to_long_decimal(&second_decimal, &long_second_decimal);

    if (is_equal_scales(&first_decimal, &second_decimal) == NOT_EQUAL) {
      to_zero_scale_for_long_decimals(&long_first_decimal,
                                      &long_second_decimal);
    }

    if (is_signs_equal(&first_decimal, &second_decimal) == EQUAL) {
      long_result = sub_long_by_bits(long_first_decimal, long_second_decimal);
    } else {
      long_result = add_long_by_bits(long_first_decimal, long_second_decimal);
    }

    long_decimal_to_decimal(&long_result, &result);
    sign_values_t result_sign =
        get_result_sign_for_sub(first_decimal, second_decimal);
    int result_scale = get_result_scale(&first_decimal, &second_decimal);
    if (is_zero_decimal(&result) == FALSE) {
      set_scale(&result, result_scale);
    }
    set_sign(&result, result_sign);

  } else {
    result =
        sub_by_value_type(first_decimal.value_type, second_decimal.value_type);
  }

  return result;
}

s21_decimal s21_mul(s21_decimal first_decimal, s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_same_and_normal_value_type(&first_decimal, &second_decimal) &&
      (is_zero_decimal(&second_decimal) || is_zero_decimal(&first_decimal))) {
    sign_values_t result_sign = get_result_sign_for_mul(first_decimal, second_decimal);
    int result_scale = 0;
    if (is_zero_decimal(&second_decimal)) {
      result_scale = get_scale(&second_decimal);
    } else if (is_zero_decimal(&first_decimal)) {
      result_scale = get_scale(&first_decimal);
    }
    set_scale(&result, result_scale);
    set_sign(&result, result_sign);
  } else if (is_same_and_normal_value_type(&first_decimal, &second_decimal)) {
    s21_long_decimal long_first_decimal, long_second_decimal, long_result;

    zeroing_and_normalize_long_decimal(&long_first_decimal);
    zeroing_and_normalize_long_decimal(&long_second_decimal);
    zeroing_and_normalize_long_decimal(&long_result);

    decimal_to_long_decimal(&first_decimal, &long_first_decimal);
    decimal_to_long_decimal(&second_decimal, &long_second_decimal);

    long_result =
        mul_long_decimals_by_bits(long_first_decimal, long_second_decimal);

    s21_long_decimal ten, long_result_copy, delta;
    zeroing_and_normalize_long_decimal(&long_result_copy);
    zeroing_and_normalize_long_decimal(&delta);
    init_ten_long_decimal(&ten);

    bool_t use_overflow = TRUE;
    int t_scale = 0;
    copy_long_decimal(&long_result, &long_result_copy);
    while (has_overflow(&long_result)) {
      t_scale++;
      use_overflow = FALSE;
      long_result = mod_ten(long_result, ten);
    }

    delta = sub_long_by_bits(long_result_copy, long_result);

    s21_decimal one;
    zeroing_and_normalize(&one);
    set_bit_by_index(&one, 0);

    long_decimal_to_decimal(&long_result, &result);
    sign_values_t result_sign =
        get_result_sign_for_mul(first_decimal, second_decimal);
    int result_scale =
        get_result_scale_for_mul(&first_decimal, &second_decimal);
    if (use_overflow) {
      set_scale(&result, result_scale);
    } else {
      int s = result_scale - t_scale;
      if (s > 28) {
        s = 28;
      }
      set_scale(&result, s);
    }
    set_sign(&result, result_sign);
  } else {
    result = mul_by_value_type(first_decimal, second_decimal);
  }

  return result;
}

s21_decimal s21_div(s21_decimal first_decimal, s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_zero_decimal(&second_decimal) &&
      first_decimal.value_type == s21_NORMAL_VALUE) {
    result = zero_division(first_decimal, second_decimal);
  } else if (is_same_and_normal_value_type(&first_decimal, &second_decimal)) {
    s21_long_decimal long_result, long_first_decimal, long_second_decimal, ten,
        mod;
    init_ten_long_decimal(&ten);

    decimal_to_long_decimal(&first_decimal, &long_first_decimal);
    decimal_to_long_decimal(&second_decimal, &long_second_decimal);

    zeroing_and_normalize_long_decimal(&long_result);

    long_first_decimal.bits[LONG_DECIMAL_SCALE] = 0;
    long_second_decimal.bits[LONG_DECIMAL_SCALE] = 0;
    long_result =
        div_long_by_bits(&long_first_decimal, &long_second_decimal, &mod);
    long_decimal_to_decimal(&long_result, &result);
    sign_values_t result_sign =
        get_result_sign_for_div(&first_decimal, &second_decimal);
    set_sign(&result, result_sign);
    if (long_result.bits[LONG_DECIMAL_SCALE] == 0) {
      int result_scale =
          get_result_scale_for_div(&first_decimal, &second_decimal);
      set_scale(&result, result_scale);
    }
  } else {
    result = div_by_value_type(first_decimal, second_decimal);
  }

  return result;
}

s21_decimal s21_mod(s21_decimal first_decimal, s21_decimal second_decimal) {
  s21_decimal result;
  zeroing_and_normalize(&result);
  if (is_same_and_normal_value_type(&first_decimal, &second_decimal)) {
    s21_long_decimal long_div_result, long_first_decimal, long_second_decimal,
        long_result, long_first_decimal_copy, long_second_decimal_copy, ten;

    zeroing_and_normalize_long_decimal(&ten);
    set_long_decimal_bit_by_index(&ten, 1);
    set_long_decimal_bit_by_index(&ten, 3);

    zeroing_and_normalize_long_decimal(&long_first_decimal);
    zeroing_and_normalize_long_decimal(&long_second_decimal);
    zeroing_and_normalize_long_decimal(&long_result);
    zeroing_and_normalize_long_decimal(&long_first_decimal_copy);
    zeroing_and_normalize_long_decimal(&long_second_decimal_copy);

    decimal_to_long_decimal(&first_decimal, &long_first_decimal);
    decimal_to_long_decimal(&second_decimal, &long_second_decimal);

    int first_scale = get_scale(&first_decimal),
        second_scale = get_scale(&second_decimal);
    int bigger_scale = 0;
    if (first_scale > second_scale) {
      bigger_scale = first_scale;
    } else {
      bigger_scale = second_scale;
    }

    to_zero_scale_for_long_decimals(&long_first_decimal, &long_second_decimal);
    copy_long_decimal(&long_first_decimal, &long_first_decimal_copy);
    copy_long_decimal(&long_second_decimal, &long_second_decimal_copy);

    zeroing_and_normalize_long_decimal(&long_div_result);
    div_long_by_bits(&long_first_decimal, &long_second_decimal,
                     &long_div_result);
    long_decimal_to_decimal(&long_div_result, &result);
    if (is_zero_long_decimal(&long_div_result) == FALSE) {
      s21_long_decimal multiply;
      zeroing_and_normalize_long_decimal(&multiply);
      multiply =
          mul_long_decimals_by_bits(long_div_result, long_second_decimal_copy);

      long_result = sub_long_by_bits(long_first_decimal_copy, multiply);
      long_decimal_to_decimal(&long_result, &result);
      set_scale(&result, bigger_scale);
    }
    sign_values_t result_sign =
        get_result_sign_for_mul(first_decimal, second_decimal);
    set_sign(&result, result_sign);
  } else {
    result = mod_by_value_type(first_decimal, second_decimal);
  }

  return result;
}

s21_long_decimal mod_ten(s21_long_decimal first_decimal,
                         s21_long_decimal second_decimal) {
  s21_long_decimal result, buffer, remainder, long_second_decimal,
      first_decimal_copy;
  zeroing_and_normalize_long_decimal(&long_second_decimal);
  copy_long_decimal(&second_decimal, &long_second_decimal);
  copy_long_decimal(&first_decimal, &first_decimal_copy);

  s21_long_decimal remainder_save;
  zeroing_and_normalize_long_decimal(&remainder_save);

  zeroing_and_normalize_long_decimal(&result);
  zeroing_and_normalize_long_decimal(&buffer);
  zeroing_and_normalize_long_decimal(&remainder);

  s21_long_decimal mod;
  zeroing_and_normalize_long_decimal(&mod);

  int first_decimal_len = get_long_decimal_len(&first_decimal);
  int iteration = 0;
  bool_t skip_check = FALSE;
  while (is_zero_long_decimal(&first_decimal) == FALSE) {
    int buffer_len = 0;
    int buffer_shifts_quantity = 0;
    bool_t use_buffer_bits = FALSE;
    buffer_shifts_quantity = 0;
    int first_decimal_len_copy = get_long_decimal_len(&first_decimal);
    while ((compare_abs_long_decimals_by_bits(long_second_decimal, buffer) ==
                FIRST_BIGGER &&
            first_decimal_len >= 0) ||
           skip_check == TRUE) {
      int first_decimal_bit =
          get_long_decimal_bit_by_index(&first_decimal, first_decimal_len);
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
      first_decimal_len--;
    }

    buffer_len = get_long_decimal_len(&buffer);

    int delta = first_decimal_len_copy - buffer_len;
    zeroing_long_decimal(&remainder);
    remainder = sub_long_by_bits(buffer, long_second_decimal);

    copy_long_decimal(&remainder, &remainder_save);
    int remainder_len = get_long_decimal_len(&remainder);

    for (int i = LONG_DECIMAL_MAX; i > first_decimal_len; i--) {
      remove_long_decimal_bit_by_index(&first_decimal, i);
    }


    int first_decimal_len_start = first_decimal_len;
    if (is_zero_long_decimal(&remainder) == FALSE) {
      for (int i = 0; i <= remainder_len; i++) {
        if (get_long_decimal_bit_by_index(&remainder, i)) {
          set_long_decimal_bit_by_index(&first_decimal,
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
      break;
    }

    s21_long_decimal remainder_copy;
    copy_long_decimal(&remainder, &remainder_copy);

    set_long_decimal_bit_by_index(&result, 0);

    while (compare_abs_long_decimals_by_bits(
               remainder_copy, long_second_decimal) == SECOND_BIGGER &&
           delta > 0) {
      remainder_copy = shift_long_decimal_bits_to_left(remainder_copy, 1);
      if (get_long_decimal_bit_by_index(&first_decimal, delta - 1)) {
        set_long_decimal_bit_by_index(&remainder_copy, 0);
      }
      result = shift_long_decimal_bits_to_left(result, 1);
      delta--;
    }
    if (is_zero_long_decimal(&first_decimal) && first_decimal_len_copy == 0) {
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

