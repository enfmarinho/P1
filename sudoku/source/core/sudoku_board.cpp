#include "sudoku_board.h"

#include <algorithm>
#include <array>
#include <vector>

namespace sdkg {
SBoard::SBoard() {
  for (int index{ 0 }; index < SB_SIZE * SB_SIZE; index++) {
    m_board[index] = 0;
  }
}

SBoard::SBoard(const std::array<short, SB_SIZE * SB_SIZE>& table) {
  for (short index{ 0 }; index < SB_SIZE * SB_SIZE; index++) {
    m_board[index] = table[index];
  }
}

short SBoard::consult(char row, short column) const {
  if (row < 'A' + SB_SIZE and row >= 'A' and column < SB_SIZE and column >= 0) {
    return m_board[(row - 'A') * SB_SIZE + column];
  } else {
    return 0;
  }
}

void SBoard::change_element(char row, short column, short value) {
  if (row < 'A' + SB_SIZE and row >= 'A' and column < SB_SIZE and column >= 0) {
    m_board[(row - 'A') * SB_SIZE + column] = value;
  }
}

PlayerBoard::PlayerBoard(const SBoard& sb) {
  // Stores the solution passed down by the client.
  m_solution = sb;
  m_ongoing = false;
  missing_numbers.resize(SB_SIZE);
  std::fill(missing_numbers.begin(), missing_numbers.end(), SB_SIZE);
  for (char row_index{ 'A' }; row_index < 'A' + SB_SIZE; row_index++) {
    for (short column_index{ 0 }; column_index < SB_SIZE; column_index++) {
      short value = m_solution.consult(row_index, column_index);
      if (value > 0) {
        missing_numbers[value - 1]--;
      }
    }
  }
}

PlayerBoard::element PlayerBoard::check_ans(char row, short column) const {
  short value = get_element(row, column, false);
  short expected = get_element(row, column, true);
  if (value == 0) {
    return { NOT_PLACED, value };
  } else if (expected > 0) {
    return { ORIGINAL, value };
  } else if (value != abs(expected)) {
    return { INCORRECT, value };
  } else {
    return { CORRECT, value };
  }
}

bool PlayerBoard::check_status(char row, short column, bool just_solution) const {
  short value = abs(get_element(row, column, just_solution));
  if (value == 0) {
    return true;
  }
  // Checks line and column.
  for (short index{ 0 }; index < SB_SIZE; index++) {
    short current_column = abs(get_element(row, index, just_solution));
    short current_row = abs(get_element(index + 'A', column, just_solution));
    if (current_column == value and column != index) {
      return false;
    } else if (current_row == value and row != index + 'A') {
      return false;
    }
  }
  // Checks block.
  short column_block = column / 3;
  short row_block = (row - 'A') / 3;
  for (short block_row_index{ 0 }; block_row_index < 3; ++block_row_index) {
    for (short block_column_index{ 0 }; block_column_index < 3; ++block_column_index) {
      short consult_column = column_block * 3 + block_column_index;
      short consult_row = row_block * 3 + 'A' + block_row_index;
      short consulted = abs(get_element(consult_row, consult_column, just_solution));
      if (consulted == value and consult_row != row and consult_column != column) {
        return false;
      }
    }
  }

  return true;
}

short PlayerBoard::get_element(char row, short column, bool just_solution) const {
  short solution_element = m_solution.consult(row, column);
  if (solution_element > 0 or just_solution) {
    return solution_element;
  } else {
    return m_player_moves.consult(row, column);
  }
}

bool PlayerBoard::still_missing(const short& number) {
  if (number > 0 and number < 10 and missing_numbers[number - 1] > 0) {
    return true;
  } else {
    return false;
  }
}

bool PlayerBoard::game_ended() {
  for (short index{ 0 }; index < SB_SIZE; index++) {
    if (missing_numbers[index] > 0) {
      return false;
    }
  }
  m_ongoing = false;
  return true;
}

PlayerBoard::loc_type_e PlayerBoard::change_element(char row, short column, short value) {
  m_ongoing = true;
  row = toupper(row);
  if (row < 'A' or row > 'A' + SB_SIZE or column < 0 or column > 9) {
    return loc_type_e::INVALID;
  } else if (m_solution.consult(row, column) > 0) {
    m_coord = { row, column };
    return loc_type_e::ORIGINAL;
  } else {
    m_coord = { row, column };
    missing_numbers[value - 1]--;
    element previous = consult(row, column);
    if (previous.value > 0 and previous.value < 10) {
      missing_numbers[previous.value - 1]++;
    }
    m_player_moves.change_element(row, column, value);
    return (check_status(row, column)) ? loc_type_e::CORRECT : loc_type_e::INCORRECT;
  }
}

PlayerBoard::element PlayerBoard::consult(char row, short column) const {
  short solution_value = m_solution.consult(row, column);
  short player_value = m_player_moves.consult(row, column);
  if (solution_value > 0) {
    element status(loc_type_e::ORIGINAL, solution_value);
    return status;
  } else if (player_value == 0) {
    element status(loc_type_e::NOT_PLACED, 0);
    return status;
  } else if (check_status(row, column)) {
    element status(loc_type_e::CORRECT, player_value);
    return status;
  } else {
    element status(loc_type_e::INCORRECT, player_value);
    return status;
  }
}
}  // namespace sdkg
