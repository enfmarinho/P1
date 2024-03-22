#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <set>
#include <string>
#include <vector>

/// This class tokenizes a string based on delimiters and limiters, trimming the
/// data or not.
class Tokenizer {
  std::vector<std::string> m_tokens; //!< Stores the tokens.
  std::set<char> m_delimiters;       //!< Stores the delimiters.
  int m_index;                       //!< Contains current index of the tokens.

  /*!
   * Checks if m_index is in the valid range.
   * @return flag that indicates whether or not the index is in the valid range.
   */
  bool in_range();

public:
  /// Removes the default constructor, as using it would not work.
  Tokenizer() = delete;

  /*!
   * Initializes values and process input.
   * @param delimiters string with delimiters (chars that mark the end of a
   * token and are not included).
   * @param trimming flag that indicates whether or not to trim the input.
   */
  Tokenizer(const std::string &delimiters);

  /*!
   * Process input for tokens.
   * @param input string with data to be processed.
   */
  void process_input(const std::string &input);

  /// Increase current index for the tokens.
  void next() { m_index++; }

  /// Decrease current index for the tokens.
  void previous() { m_index--; }

  /// Set current token to be the first one.
  void beginning() { m_index = 0; }

  /// Consult index.
  int get_index() const { return m_index; }

  /*!
   * Checks whether or not the end of the vector has been reached.
   * @return flag that indicates whether or not the end of the vector has been
   * reached.
   */
  bool ended() { return m_index == m_tokens.size(); }

  /*!
   * Checks if the token index is valid, if so, gets current token.
   * Otherwise returns an empty string.
   * @return string with current token or an empty string.
   */
  std::string get_token();

  /// Return copy of vector with tokens.
  std::vector<std::string> get_all_tokens() const { return m_tokens; }
};

#endif // TOKENIZER_H
