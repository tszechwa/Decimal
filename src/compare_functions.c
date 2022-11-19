#include "s21_decimal.h"

int s21_is_less(s21_decimal first_decimal, s21_decimal second_decimal) {
  return compare_decimals(&first_decimal, &second_decimal, LESS_OPERATOR) != SECOND_BIGGER;
}

int s21_is_less_or_equal(s21_decimal first_decimal,
                         s21_decimal second_decimal) {
  int result = 1;
  compare_values_t compare_result =
      compare_decimals(&first_decimal, &second_decimal, LESS_OR_EQUAL_OPERATOR);
  if (compare_result == SECOND_BIGGER || compare_result == EQUAL) {
    result = 0;
  }
  return result;
}

int s21_is_greater(s21_decimal first_decimal, s21_decimal second_decimal) {
  return compare_decimals(&first_decimal, &second_decimal, GREATHER_OPERATOR) != FIRST_BIGGER;
}

int s21_is_greater_or_equal(s21_decimal first_decimal,
                            s21_decimal second_decimal) {
  int result = 1;
  compare_values_t compare_result = compare_decimals(
      &first_decimal, &second_decimal, GREATHER_OR_EQUAL_OPERATOR);
  if (compare_result == FIRST_BIGGER || compare_result == EQUAL) {
    result = 0;
  }
  return result;
}

int s21_is_equal(s21_decimal first_decimal, s21_decimal second_decimal) {
  return compare_decimals(&first_decimal, &second_decimal, EQUAL_OPERATOR) != EQUAL;
}

int s21_is_not_equal(s21_decimal first_decimal, s21_decimal second_decimal) {
  return compare_decimals(&first_decimal, &second_decimal, NOT_EQUAL_OPERATOR) == EQUAL;
}

// Нужно добавить приведение к одинаковому скейлу
compare_values_t compare_decimals(s21_decimal *first_decimal,
                                  s21_decimal *second_decimal,
                                  const char operator) {
  compare_values_t compare_result = EQUAL;
  if (is_same_and_normal_value_type(first_decimal, second_decimal)) {
    compare_result = compare_decimals_by_bits(first_decimal, second_decimal);
  } else {
    compare_result =
        compare_decimals_by_value_type(first_decimal, second_decimal, operator);
  }
  return compare_result;
}

bool_t is_same_and_normal_value_type(s21_decimal *first_decimal,
                                     s21_decimal *second_decimal) {
  bool_t flag = FALSE;
  if (is_equal_value_type(first_decimal, second_decimal) == EQUAL &&
      first_decimal->value_type == s21_NORMAL_VALUE) {
    flag = TRUE;
  }
  return flag;
}

compare_values_t is_equal_value_type(s21_decimal *first_decimal,
                                     s21_decimal *second_decimal) {
  compare_values_t compare_result = NOT_EQUAL;
  if (first_decimal->value_type == second_decimal->value_type) {
    compare_result = EQUAL;
  }
  return compare_result;
}

compare_values_t compare_decimals_by_bits(s21_decimal *first_decimal,
                                          s21_decimal *second_decimal) {
  compare_values_t compare_result = EQUAL;

  sign_values_t first_sign = get_sign(first_decimal);
  sign_values_t second_sign = get_sign(second_decimal);

  if (is_zero_decimal(first_decimal) && is_zero_decimal(second_decimal)) {
    compare_result = EQUAL;
  } else if (first_sign == NEGATIVE && second_sign == POSITIVE) {
    compare_result = SECOND_BIGGER;
  } else if (first_sign == POSITIVE && second_sign == NEGATIVE) {
    compare_result = FIRST_BIGGER;
  } else if (first_sign == POSITIVE && second_sign == POSITIVE) {
    compare_result = compare_abs_decimals_by_bits(first_decimal, second_decimal);
  } else {
    compare_result = compare_abs_decimals_by_bits(first_decimal, second_decimal);
    if (compare_result == SECOND_BIGGER) {
      compare_result = FIRST_BIGGER;
    } else if (compare_result == FIRST_BIGGER) {
      compare_result = SECOND_BIGGER;
    }
  }
  return compare_result;
}

compare_values_t compare_abs_decimals_by_bits(s21_decimal *first_decimal,
                                              s21_decimal *second_decimal) {
  compare_values_t compare_result = EQUAL;
  s21_long_decimal long_first_decimal, long_second_decimal;
  decimal_to_long_decimal(first_decimal, &long_first_decimal);
  decimal_to_long_decimal(second_decimal, &long_second_decimal);

  if (is_equal_scales(first_decimal, second_decimal) == NOT_EQUAL) {
    to_zero_scale_for_long_decimals(&long_first_decimal, &long_second_decimal);
  }

  for (int i = LONG_DECIMAL_MAX; i >= LONG_DECIMAL_MIN; i--) {
    int bit1 = get_long_decimal_bit_by_index(&long_first_decimal, i);
    int bit2 = get_long_decimal_bit_by_index(&long_second_decimal, i);
    if (bit1 > bit2) {
      compare_result = FIRST_BIGGER;
      break;
    } else if (bit1 < bit2) {
      compare_result = SECOND_BIGGER;
      break;
    }
  }
  return compare_result;
}

compare_values_t compare_abs_long_decimals_by_bits(
    s21_long_decimal long_first_decimal, s21_long_decimal long_second_decimal) {
  compare_values_t compare_result = EQUAL;
  if (long_first_decimal.bits[7] != long_second_decimal.bits[7]) {
    to_zero_scale_for_long_decimals(&long_first_decimal, &long_second_decimal);
  }

  for (int i = LONG_DECIMAL_MAX; i >= LONG_DECIMAL_MIN; i--) {
    int bit1 = get_long_decimal_bit_by_index(&long_first_decimal, i);
    int bit2 = get_long_decimal_bit_by_index(&long_second_decimal, i);
    if (bit1 > bit2) {
      compare_result = FIRST_BIGGER;
      break;
    } else if (bit1 < bit2) {
      compare_result = SECOND_BIGGER;
      break;
    }
  }
  return compare_result;
}


const compare_table_t LESS_OPEROTOR_TABLE[COMPARE_TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, TRUE},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NORMAL_VALUE, s21_NAN, FALSE},

    {s21_INFINITY, s21_NORMAL_VALUE, FALSE},
    {s21_INFINITY, s21_INFINITY, FALSE},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, TRUE},
    {s21_INFINITY, s21_NAN, FALSE},

    {s21_NEGATIVE_INFINITY, s21_NORMAL_VALUE, TRUE},
    {s21_NEGATIVE_INFINITY, s21_INFINITY, TRUE},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NEGATIVE_INFINITY, s21_NAN, FALSE},

    {s21_NAN, s21_NORMAL_VALUE, FALSE},
    {s21_NAN, s21_INFINITY, FALSE},
    {s21_NAN, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NAN, s21_NAN, FALSE}
};

const compare_table_t LESS_OR_EQUAL_OPEROTOR_TABLE[COMPARE_TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, TRUE},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NORMAL_VALUE, s21_NAN, FALSE},

    {s21_INFINITY, s21_NORMAL_VALUE, FALSE},
    {s21_INFINITY, s21_INFINITY, TRUE},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, TRUE},
    {s21_INFINITY, s21_NAN, FALSE},

    {s21_NEGATIVE_INFINITY, s21_NORMAL_VALUE, TRUE},
    {s21_NEGATIVE_INFINITY, s21_INFINITY, TRUE},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, TRUE},
    {s21_NEGATIVE_INFINITY, s21_NAN, FALSE},

    {s21_NAN, s21_NORMAL_VALUE, FALSE},
    {s21_NAN, s21_INFINITY, FALSE},
    {s21_NAN, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NAN, s21_NAN, FALSE}};

const compare_table_t GREATHER_OPERATOR_TABLE[COMPARE_TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, FALSE},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, TRUE},
    {s21_NORMAL_VALUE, s21_NAN, FALSE},

    {s21_INFINITY, s21_NORMAL_VALUE, TRUE},
    {s21_INFINITY, s21_INFINITY, FALSE},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, FALSE},
    {s21_INFINITY, s21_NAN, FALSE},

    {s21_NEGATIVE_INFINITY, s21_NORMAL_VALUE, FALSE},
    {s21_NEGATIVE_INFINITY, s21_INFINITY, FALSE},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NEGATIVE_INFINITY, s21_NAN, FALSE},

    {s21_NAN, s21_NORMAL_VALUE, FALSE},
    {s21_NAN, s21_INFINITY, FALSE},
    {s21_NAN, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NAN, s21_NAN, FALSE}};

const compare_table_t GREATHER_OR_EQUAL_OPERATOR_TABLE[COMPARE_TABLE_ROWS] = {
    {s21_NORMAL_VALUE, s21_INFINITY, FALSE},
    {s21_NORMAL_VALUE, s21_NEGATIVE_INFINITY, TRUE},
    {s21_NORMAL_VALUE, s21_NAN, FALSE},

    {s21_INFINITY, s21_NORMAL_VALUE, TRUE},
    {s21_INFINITY, s21_INFINITY, TRUE},
    {s21_INFINITY, s21_NEGATIVE_INFINITY, FALSE},
    {s21_INFINITY, s21_NAN, FALSE},

    {s21_NEGATIVE_INFINITY, s21_NORMAL_VALUE, FALSE},
    {s21_NEGATIVE_INFINITY, s21_INFINITY, FALSE},
    {s21_NEGATIVE_INFINITY, s21_NEGATIVE_INFINITY, TRUE},
    {s21_NEGATIVE_INFINITY, s21_NAN, FALSE},

    {s21_NAN, s21_NORMAL_VALUE, FALSE},
    {s21_NAN, s21_INFINITY, FALSE},
    {s21_NAN, s21_NEGATIVE_INFINITY, FALSE},
    {s21_NAN, s21_NAN, FALSE}};

compare_values_t compare_decimals_by_value_type(s21_decimal *first_decimal,
                                                s21_decimal *second_decimal,
                                                const char operator) {
  compare_values_t compare_result = EQUAL;
  if (operator== LESS_OPERATOR) {
    if (is_less_by_value_type(first_decimal->value_type,
                              second_decimal->value_type)) {
      compare_result = SECOND_BIGGER;
    } else {
      compare_result = FIRST_BIGGER;
    }
  } else if (operator== LESS_OR_EQUAL_OPERATOR) {
    if (is_less_or_equal_by_value_type(first_decimal->value_type,
                                       second_decimal->value_type)) {
      compare_result = EQUAL;
    } else {
      compare_result = NOT_EQUAL;
    }
  } else if (operator== GREATHER_OPERATOR) {
    if (is_greather_by_value_type(first_decimal->value_type,
                                  second_decimal->value_type)) {
      compare_result = FIRST_BIGGER;
    } else {
      compare_result = SECOND_BIGGER;
    }
  } else if (operator== GREATHER_OR_EQUAL_OPERATOR) {
    if (is_greather_or_equal_by_value_type(first_decimal->value_type,
                                           second_decimal->value_type)) {
      compare_result = EQUAL;
    } else {
      compare_result = NOT_EQUAL;
    }
  } else if (operator== EQUAL_OPERATOR || operator== NOT_EQUAL_OPERATOR) {
    if (first_decimal->value_type == s21_NAN &&
        second_decimal->value_type == s21_NAN) {
      compare_result = NOT_EQUAL;
    } else if (first_decimal->value_type == second_decimal->value_type) {
      compare_result = EQUAL;
    } else {
      compare_result = NOT_EQUAL;
    }
  }

  return compare_result;
}

bool_t is_less_by_value_type(value_type_t first_value_type,
                             value_type_t second_value_type) {
  bool_t flag = FALSE;
  for (int i = 0; i < COMPARE_TABLE_ROWS; i++) {
    int first_value = LESS_OPEROTOR_TABLE[i].first_value;
    int second_value = LESS_OPEROTOR_TABLE[i].second_value;
    if (first_value_type == first_value && second_value_type == second_value) {
      flag = LESS_OPEROTOR_TABLE[i].compare_result;
      break;
    }
  }
  return flag;
}

bool_t is_less_or_equal_by_value_type(value_type_t first_value_type,
                                      value_type_t second_value_type) {
  bool_t flag = FALSE;
  for (int i = 0; i < COMPARE_TABLE_ROWS; i++) {
    int first_value = LESS_OR_EQUAL_OPEROTOR_TABLE[i].first_value;
    int second_value = LESS_OR_EQUAL_OPEROTOR_TABLE[i].second_value;
    if (first_value_type == first_value && second_value_type == second_value) {
      flag = LESS_OR_EQUAL_OPEROTOR_TABLE[i].compare_result;
      break;
    }
  }
  return flag;
}

bool_t is_greather_by_value_type(value_type_t first_value_type,
                                 value_type_t second_value_type) {
  bool_t flag = FALSE;
  for (int i = 0; i < COMPARE_TABLE_ROWS; i++) {
    int first_value = GREATHER_OPERATOR_TABLE[i].first_value;
    int second_value = GREATHER_OPERATOR_TABLE[i].second_value;
    if (first_value_type == first_value && second_value_type == second_value) {
      flag = GREATHER_OPERATOR_TABLE[i].compare_result;
      break;
    }
  }
  return flag;
}

bool_t is_greather_or_equal_by_value_type(value_type_t first_value_type,
                                          value_type_t second_value_type) {
  bool_t flag = FALSE;
  for (int i = 0; i < COMPARE_TABLE_ROWS; i++) {
    int first_value = GREATHER_OR_EQUAL_OPERATOR_TABLE[i].first_value;
    int second_value = GREATHER_OR_EQUAL_OPERATOR_TABLE[i].second_value;
    if (first_value_type == first_value && second_value_type == second_value) {
      flag = GREATHER_OR_EQUAL_OPERATOR_TABLE[i].compare_result;
      break;
    }
  }
  return flag;
}


