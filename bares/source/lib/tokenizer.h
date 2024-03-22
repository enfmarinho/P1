#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <set>
#include <vector>
#include <string>

/// This class tokenizes a string based on delimiters and limiters, trimming the
/// data or not.
class Tokenizer {
  std::vector<std::string> m_tokens; //!< Stores the tokens.
  std::vector<char>
      m_delimiters; //!< Stores the delimiters(marks the end of token).
  std::vector<char>
      m_limiters;  //!< Stores the limiters(mark the end but are included).
  bool m_trimming; //!< Flag that indicates whether data should be trimmed.
  int m_index;     //!< Contains current index of the tokens.

  /*!
   * Process input for tokens.
   * @param input string with data to be processed.
   */
  void process_input(const std::string &input);

  /*!
   * Checks if m_index is in the valid range.
   * @return flag that indicates whether or not the index is in the valid range.
   */
  bool in_range();

public:
  /// Just to debug.
  void print_all_tokens();
  /// Removes the default constructor, as using it would not work.
  Tokenizer() = delete;

  /*!
   * Initializes values and process input.
   * @param input string to be tokenized.
   * @param delimiters string with delimiters (chars that mark the end of a
   * token and are not included).
   * @param limiters string with limiters (chars that mark the end of a token
   * but are included).
   * @param trimming flag that indicates whether or not to trim the input.
   */
  Tokenizer(const std::string &input, const std::string &delimiters,
            const std::string &limiters, bool trimming = true);

  /// Increase current index for the tokens.
  void next() { m_index++; }

  /// Decrease current index for the tokens.
  void previous() { m_index--; }

  /// Set current token to be the first one.
  void beginning() { m_index = 0; }

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

  /*!
   * Consults all tokens
   * \return vector containing all tokens.
   */
  std::vector<std::string> get_all_tokens() const { return m_tokens; }
};

#endif // TOKENIZER_H
