#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "tokenizer.h"
/// Class representing a billboard.
class Billboard {
public:
  /// Alignment options.
  enum align_t : int { LEFT = 0, RIGHT, CENTERED };

  /// Removes the default constructor, as using it would not work.
  Billboard() = delete;

  /*!
   * Initializes values.
   * @param data tokenizer class with tokens to be printed.
   * @param wrap_column size of the billboard column.
   * @param alignment alignment option.
   * @param padding size of the left margin.
   */
  Billboard(Tokenizer& data, int wrap_column, align_t alignment, int padding);

  /// Prints billboard.
  void print();

private:
  Tokenizer* m_data;    //!< Tokens to print.
  align_t m_alignment;  //!< Alignment option.
  int m_wrap_column;    //!< Size of the billboard column.
  int m_padding;        //!< Size of the left margin.

  /*!
   * Prints a character n times.
   * @param c char to be printed.
   * @param n number of times to print the character.
   */
  void print_n_times(int n, const char& c) const;
  /*!
   * Prints line after calls function print_padding().
   * @param line string to be printed.
   * @param line_size size of the string "line".
   */
  void print_line(std::ostringstream& line, int line_size);
};

#endif  // BILLBOARD_H
