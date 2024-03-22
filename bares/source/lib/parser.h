#ifndef PARSER_H
#define PARSER_H

#include <string>

/// Class that implements the parser for the input expression.
class Parser {
  std::string m_input;       //!< Input string.
  std::string m_err_msg{""}; //!< Error message.
  int m_index{0};            //!< Current index in the input string.
  int m_in_brackets{0};      //!<

public:
  Parser() = delete;
  Parser(std::string &input);
  ~Parser() = default;

  /// Checks if next element in m_input is a valid expression.
  bool expr();
  bool second_half();
  /// Checks if next element in m_input is a valid term.
  bool term();
  /// Checks if next element in m_input is a valid integer.
  bool integer();
  /// Checks if next element in m_input is a natural number.
  bool natural_num();
  /// Checks if current char is a digit.
  bool digit();
  /// Checks if current char is a digit different than 0.
  bool dg_excl_zero();
  /// Skips white spaces.
  void ignore_wsp();
  /// Consults the error message.
  [[nodiscard]] std::string consult_err_msg() const { return m_err_msg; }
};

#endif // PARSER_H
