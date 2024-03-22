#include "parser.h"
#include <algorithm>
#include <string>

Parser::Parser(std::string &input) {
  m_input = input;
  ignore_wsp();
  if (m_index >= m_input.size()) {
    m_err_msg = "Unexpected end of expression at column (" +
                std::to_string(m_index + 1) + ")!";
  } else {
    expr();
  }
}

bool Parser::expr() {
  ignore_wsp();
  if (term()) {
    ignore_wsp();
    if (m_index >= m_input.size())
      return true;
    return second_half();
  } else {
    m_err_msg =
        "Missing <term> at column (" + std::to_string(m_index + 1) + ")!";
    return false;
  }
}

bool Parser::second_half() {
  ignore_wsp();
  if (m_index >= m_input.size())
    return true;
  if (m_in_brackets and m_input[m_index] == ')')
    return true;
  std::string operation = "+-*/%^";
  if (std::find(operation.begin(), operation.end(), m_input[m_index]) !=
      operation.end()) {
    m_index++;
    ignore_wsp();
    if (term()) {
      return second_half();
    } else {
      return false;
    }
  } else if (m_input[m_index] == ' ') {
    return second_half();
  } else {
    m_err_msg = "Extra symbol after valid expression found at column (" +
                std::to_string(m_index + 1) + ")!";
    return false;
  }
}

bool Parser::term() {
  if (integer()) {
    return true;
  } else if (m_input[m_index] == '(') {
    ++m_in_brackets;
    ++m_index;
    ignore_wsp();
    if (not expr()) {
      return false;
    }
    ignore_wsp();
    --m_in_brackets;
    if (m_input[m_index] == ')') {
      ++m_index;
      return true;
    } else {
      m_err_msg = "Missing closing \")\" at column (" +
                  std::to_string(m_index + 1) + ")!";
      return true;
    }
  } else {
    return false;
  }
}

bool Parser::integer() {
  if (m_input[m_index] == '0') {
    ++m_index;
    return true;
  } else {
    if (m_input[m_index] == '-') {
      ++m_index;
    }
    return natural_num();
  }
}

bool Parser::natural_num() {
  if (dg_excl_zero()) {
    while (m_index < m_input.size()) {
      if (not digit()) {
        m_err_msg = "";
        break;
      }
    }
    return true;
  }
  return false;
}

bool Parser::digit() {
  if (m_input[m_index] == '0') {
    ++m_index;
    return true;
  } else {
    return dg_excl_zero();
  }
}

bool Parser::dg_excl_zero() {
  for (char number = '1'; number <= '9'; number++) {
    if (m_input[m_index] == number) {
      ++m_index;
      return true;
    }
  }
  m_err_msg = "Missing <term> at column (" + std::to_string(m_index + 1) + ")!";
  return false;
}

void Parser::ignore_wsp() {
  while (m_index < m_input.size() and m_input[m_index] == ' ') {
    ++m_index;
  }
}
