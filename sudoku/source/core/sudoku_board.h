#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <algorithm>
#include <array>
#include <utility>
#include <vector>
/*!
 *  In this header file we have two classes: SBoard and SudokuPlayerBoard.
 *
 *  The former represents only the board data, as a matrix of short.
 *
 *  The latter represents the board and the actions necessary to support
 *  the player's moves and all the information associated with a
 *  Sudoku match. This involves enconding information regarding incorrect
 *  moves, invalid plays, correct play, original numbers, etc.
 *  These information is required to display the board with special colors
 *  representing all thesse different conditions.
 */

constexpr short SB_SIZE{ 9 };

namespace sdkg {

/// This class stores values for a 9x9 Sudoku board.
class SBoard {
private:
  std::array<short, SB_SIZE * SB_SIZE> m_board;

public:
  SBoard();
  SBoard(const std::array<short, SB_SIZE * SB_SIZE>& table);
  SBoard& operator=(const SBoard& source) = default;
  ~SBoard() = default;
  /*!
   * Consults element in specific position in board.
   * \parameter row row of the element to consult.
   * \parameter column column of the element to consult.
   */
  short consult(char row, short column) const;
  /*!
   * Changes a element in the board.
   * \parameter row row of the element to change.
   * \parameter column column of the element to change.
   * \parameter value value to insert.
   */
  void change_element(char row, short column, short value);
};

class PlayerBoard {
private:
  bool m_ongoing;                      //!< Flag that indicates whether the game in ongoing.
  std::vector<short> missing_numbers;  //!< List of number of missing values.
  SBoard m_player_moves;               //!< The Sudoku matrix where the user moves are stored.
  SBoard m_solution;                   //!< The Sudoku matrix with the solution.
  std::pair<char, short> m_coord;      //!< Position of last change in the board.

public:
  /// Possible types associated with a location on the board during a match.
  enum loc_type_e : short {
    ORIGINAL = 0,  //!< Original puzzle value, not mutable.
    CORRECT,       //!< Correct value.
    INCORRECT,     //!< Incorrect value.
    NOT_PLACED,    //!< Value is empty.
    INVALID        //!< Invalid play.
  };

  struct element {
    loc_type_e status;
    short value;
    element(loc_type_e e, short v) : status{ e }, value{ v } {}
  };

  //=== Regular methods.
  PlayerBoard() = delete;
  PlayerBoard& operator=(const PlayerBoard&) = delete;
  PlayerBoard(const PlayerBoard&) = delete;
  PlayerBoard(const SBoard& sb);
  ~PlayerBoard() = default;

  /*!
   * Checks if the game is ongoing.
   * \return flag that indicates whether the game in ongoing.
   */
  bool ongoing() const { return m_ongoing; }
  /*!
   * Checks if a number is still missing in the puzzle.
   * \parameter number number to check.
   * \return flag that indicates whether the number is missing.
   */
  bool still_missing(const short& number);
  /*!
   * Checks if game has ended.
   * \return flag that indicates whether the game has ended.
   */
  bool game_ended();
  /*!
   * Checks whether a element at a specific row and column is correct.
   * \parameter row row of the element to check.
   * \parameter column column of the element to check.
   * \parameter flag that indicates whether or not to consider just the solution board.
   * \return flag that indicates whether the value at that position is correct.
   */
  bool check_status(char row, short column, bool just_solution = false) const;
  /*!
   * Checks if a value is equal to the expected one.
   * \parameter row row to check.
   * \parameter column column to check.
   */
  element check_ans(char row, short column) const;
  /*!
   * Consults the element in the position.
   * \parameter row row to check.
   * \parameter column column to check.
   * \return element in the position.
   */
  short get_element(char row, short column, bool just_solution) const;
  /*!
   * Consults a place in the puzzle.
   * \parameter row row to check.
   * \parameter column column to check.
   * \return structure representing element.
   */
  element consult(char row, short column) const;
  /*!
   * Changes element in puzzle, if it is possible.
   * \parameter row row of the element to change.
   * \parameter column column of the element to change.
   * \parameter value value to insert.
   * \return enumeration representing status of change.
   */
  loc_type_e change_element(char row, short column, short value);
};
}  // namespace sdkg
#endif
