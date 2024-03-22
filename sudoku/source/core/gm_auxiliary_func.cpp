#include "sudoku_gm.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string.h>
#include <string>
#include "../lib/CLI.h"
#include "../lib/text_color.h"
#include "sudoku_board.h"
#include "tokenizer.h"

namespace sdkg {
void SudokuGame::read_enter() const {
  string enter;
  std::getline(std::cin, enter);
}

void SudokuGame::initialize(int argc, char* argv[]) {
  CLI arguments;
  arguments.read_command_line(argc, argv);
  m_quitting_match = true;
  m_opt.total_checks = arguments.get_checks_number();
  m_opt.input_filename = arguments.get_input_file();
  m_game_state = game_state_e::STARTING;
  m_quitting_match = false;
  m_game_index = 1;

  // Creating hardest match
  std::array<short, SB_SIZE * SB_SIZE> table
    = { 8,  -1, -2, -7, -5, -3, -6, -4, -9, -9, -4, 3,  6,  -8, -2, -1, -7, -5, -6, 7, -5,
        -4, 9,  -1, 2,  -8, -3, -1, 5,  -4, -2, -3, 7,  -8, -9, -6, -3, -6, -9, -8, 4, 5,
        7,  -2, -1, -2, -8, -7, 1,  -6, -9, -5, 3,  -4, -5, -2, 1,  -9, -7, -4, -3, 6, 8,
        -4, -3, 8,  5,  -2, -6, -9, 1,  -7, -7, 9,  -6, -3, -1, -8, 4,  -5, -2 };

  std::shared_ptr<PlayerBoard> p_board = std::make_shared<PlayerBoard>(SBoard(table));
  m_undo_log.push_back(std::deque<Command>());
  m_boards.push_back(p_board);
  m_checks_left.push_back(0);
  m_moves.push_back(0);
  m_incorrect_moves.push_back(0);
}

bool SudokuGame::game_over() const { return m_quitting_match; }

bool SudokuGame::confirm_quitting() const {
  string choice;
  std::cin >> choice;
  if (tolower(choice[0]) == 'y') {
    return true;
  } else {
    return false;
  }
}

bool SudokuGame::confirm_hardest_match() {
  string choice;
  getline(std::cin, choice);
  for (int index{ 0 }; index < choice.size(); index++) {
    choice[index] = tolower(choice[index]);
  }
  if (choice == (string) "i am brave enough!") {
    return true;
  } else {
    return false;
  }
}

bool SudokuGame::check_ongoing_match() const {
  for (size_t index = 0; index < m_boards.size(); index++) {
    if (m_boards[index]->ongoing()) {
      return true;
    }
  }
  return false;
}

bool SudokuGame::check_board(std::shared_ptr<PlayerBoard> board) {
  for (char row_index{ 'A' }; row_index < 'A' + SB_SIZE; row_index++) {
    for (short column_index{ 0 }; column_index < SB_SIZE; column_index++) {
      if (not board->check_status(row_index, column_index, true)) {
        return false;
      }
    }
  }
  return true;
}

bool SudokuGame::check_victory() {
  for (char row_index{ 'A' }; row_index < 'A' + SB_SIZE; row_index++) {
    for (short column_index{ 0 }; column_index < SB_SIZE; column_index++) {
      if (not m_boards[m_game_index]->check_status(row_index, column_index)) {
        return false;
      }
    }
  }
  m_curr_play = Play();
  return true;
}

void SudokuGame::read_puzzles() {
  std::ifstream File(m_opt.input_filename);
  if (not File.good()) {
    m_curr_msg = "File could not be read.";
    m_quitting_match = true;
    return;
  }

  size_t n_puzzles;
  File >> n_puzzles;
  for (size_t counter{ 0 }; counter < n_puzzles; counter++) {
    std::array<short, SB_SIZE * SB_SIZE> tmp;
    for (size_t index{ 0 }; index < SB_SIZE * SB_SIZE; index++) {
      File >> tmp[index];
    }
    std::shared_ptr<PlayerBoard> p_board = std::make_shared<PlayerBoard>(SBoard(tmp));
    if (check_board(p_board)) {
      m_undo_log.push_back(std::deque<Command>());
      m_boards.push_back(p_board);
      m_checks_left.push_back(m_opt.total_checks);
      m_moves.push_back(0);
      m_incorrect_moves.push_back(0);
    } else {
      m_invalid_puzzles.push_back(counter + 1);
    }
  }
  File.close();
  m_curr_msg = "Finished reading input data file.";
}

void SudokuGame::save_ongoing_game(const string& filename) {
  std::ofstream File(filename, std::ios::binary);
  if (not File.good()) {
    m_curr_msg = ">>> File could not be opened.\n";
    return;
  } else {
    for (char row{ 'A' }; row < 'A' + SB_SIZE; row++) {
      for (short column{ 0 }; column < SB_SIZE; column++) {
        short tmp = m_boards[m_game_index]->get_element(row, column, true);
        File.write((char*)&tmp, sizeof(tmp));
      }
    }
    int size = m_undo_log[m_game_index].size();
    File.write((char*)&size, sizeof(size));
    std::deque<Command> copy(m_undo_log[m_game_index]);
    while (not copy.empty()) {
      short zero = 0;
      short one = 1;
      switch (copy.front().action) {
      case Command::type_e::PLACE:
        File.write((char*)&zero, sizeof(zero));
        break;
      case Command::type_e::REMOVE:
        File.write((char*)&one, sizeof(one));
        break;
      }
      File.write((char*)&copy.front().data.row, sizeof(copy.front().data.row));
      File.write((char*)&copy.front().data.col, sizeof(copy.front().data.col));
      File.write((char*)&copy.front().data.value, sizeof(copy.front().data.value));
      File.write((char*)&copy.front().data.value_replaced,
                 sizeof(copy.front().data.value_replaced));
      copy.pop_front();
    }
    File.write((char*)&m_checks_left[m_game_index], sizeof(m_checks_left[m_game_index]));
    File.write((char*)&m_moves[m_game_index], sizeof(m_moves[m_game_index]));
    File.write((char*)&m_incorrect_moves[m_game_index], sizeof(m_incorrect_moves[m_game_index]));
  }
  File.close();
  m_curr_msg = "The game has been saved successfully.";
}

void SudokuGame::load_old_game(const string& filename) {
  std::ifstream File(filename, std::ios::binary);
  if (not File.good() or File.eof()) {
    throw(">>> File could not be read.");
  } else {
    array<short, SB_SIZE * SB_SIZE> tmp;
    for (short index{ 0 }; index < SB_SIZE * SB_SIZE; index++) {
      File.read((char*)&tmp[index], sizeof(short));
    }
    std::shared_ptr<PlayerBoard> p_board = std::make_shared<PlayerBoard>(SBoard(tmp));
    if (check_board(p_board)) {
      int size_undo_log;
      File.read((char*)&size_undo_log, sizeof(size_undo_log));
      std::deque<Command> undo_log_tmp;
      for (int counter{ 0 }; counter < size_undo_log; counter++) {
        char row;
        short option, column, value, value_replaced;
        File.read((char*)&option, sizeof(option));
        File.read((char*)&row, sizeof(row));
        File.read((char*)&column, sizeof(column));
        File.read((char*)&value, sizeof(value));
        File.read((char*)&value_replaced, sizeof(value_replaced));
        Play play_tmp(row, column, value, value_replaced);
        if (option == 0) {  // place
          Command tmp(Command::type_e::PLACE, play_tmp);
          undo_log_tmp.push_back(tmp);
          p_board->change_element(row, column, value);
        } else if (option == 1) {  // remove
          Command tmp(Command::type_e::REMOVE, play_tmp);
          undo_log_tmp.push_back(tmp);
          p_board->change_element(row, column, 0);
        } else {
          throw(">>> File is corrupted.");
        }
      }

      short checks_remaining;
      File.read((char*)&checks_remaining, sizeof(checks_remaining));
      int moves, incorrect_moves;
      File.read((char*)&moves, sizeof(moves));
      File.read((char*)&incorrect_moves, sizeof(incorrect_moves));

      m_boards.push_back(p_board);
      m_undo_log.push_back(undo_log_tmp);
      m_checks_left.push_back(checks_remaining);
      m_moves.push_back(moves);
      m_incorrect_moves.push_back(incorrect_moves);
    }
  }
  File.close();
  m_curr_msg = ">>> The file has been read successfully.";
}

SudokuGame::main_menu_opt_e SudokuGame::read_main_menu_opt() {
  string input;
  std::cin >> input;
  int option;
  try {
    option = std::stoi(input);
  } catch (std::exception& err) {
    return main_menu_opt_e::INVALID;
  }
  read_enter();
  switch (option - 1) {
  case main_menu_opt_e::HELP:
    return main_menu_opt_e::HELP;
  case main_menu_opt_e::PLAY:
    return main_menu_opt_e::PLAY;
  case main_menu_opt_e::QUIT:
    return main_menu_opt_e::QUIT;
  case main_menu_opt_e::NEW_GAME:
    return main_menu_opt_e::NEW_GAME;
  case main_menu_opt_e::LOAD:
    return main_menu_opt_e::LOAD;
  case main_menu_opt_e::SAVE:
    return main_menu_opt_e::SAVE;
  case main_menu_opt_e::HARDEST:
    return main_menu_opt_e::HARDEST;
  default:
    return main_menu_opt_e::INVALID;
  }
}

SudokuGame::play_opt_e SudokuGame::read_user_play() {
  char play = getchar();
  play = tolower(play);
  switch (play) {
  case '\n':
    return play_opt_e::MENU;
  case 'p':
    return play_opt_e::PLACE;
  case 'r':
    return play_opt_e::REMOVE;
  case 'c':
    return play_opt_e::CHECK;
  case 'u':
    return play_opt_e::UNDO;
  default:
    return play_opt_e::INVALID_P;
  }
}

void SudokuGame::play() {
  std::string input;
  getline(std::cin, input);
  Tokenizer tokens(input, " ");

  char row;
  short column, value;
  for (int index = 0; index < 3 or not tokens.ended(); index++, tokens.next()) {
    if (index == 0) {
      row = toupper(tokens.get_token()[0]);
    } else if (index == 1) {
      try {
        column = std::stoi(tokens.get_token());
        column--;
      } catch (std::exception& err) {
        m_curr_msg = "Invalid command";
        return;
      }
    } else if (index == 2) {
      try {
        value = stoi(tokens.get_token());
      } catch (std::exception& err) {
        m_curr_msg = "Invalid command.";
        return;
      }
    }
  }
  if (value < 1 or value > 9) {
    m_curr_msg = "Value must be in range [1,9].";
  } else {
    short value_replaced = m_boards[m_game_index]->consult(row, column).value;
    Play player_action(row, column, value, value_replaced);
    Command player_command(Command::type_e::PLACE, player_action);
    m_moves[m_game_index]++;
    switch (m_boards[m_game_index]->change_element(row, column, value)) {
    case PlayerBoard::loc_type_e::INVALID:
      m_curr_msg = "You chose a place outside the boundaries of the board.";
      m_incorrect_moves[m_game_index]++;
      break;
    case PlayerBoard::loc_type_e::ORIGINAL:
      m_incorrect_moves[m_game_index]++;
      m_curr_play = player_action;
      m_curr_msg = "This is a value from the original board. It cannot be changed.";
      break;
    case PlayerBoard::loc_type_e::INCORRECT:
      m_incorrect_moves[m_game_index]++;
      m_curr_play = player_action;
      m_undo_log[m_game_index].push_back(player_command);
      m_curr_msg = "Hey, this is not a valid move to make. BUT it`s up to you to keep it or not.";
      break;
    case PlayerBoard::loc_type_e::CORRECT:
      m_curr_play = player_action;
      m_undo_log[m_game_index].push_back(player_command);
      m_curr_msg = "";
      break;
    default:
      break;
    }
  }
}

void SudokuGame::remove() {
  std::string input;
  getline(std::cin, input);
  Tokenizer tokens(input, " ");

  char row;
  short column;
  for (int index = 0; index < 2 or tokens.ended(); index++, tokens.next()) {
    if (index == 0) {
      row = toupper(tokens.get_token()[0]);
    } else if (index == 1) {
      try {
        column = std::stoi(tokens.get_token());
        column--;
      } catch (std::exception& err) {
        m_curr_msg = "Invalid command";
        return;
      }
    }
  }
  short value_replaced = m_boards[m_game_index]->consult(row, column).value;
  Play player_action(row, column, 0, value_replaced);
  Command player_command(Command::type_e::REMOVE, player_action);
  switch (m_boards[m_game_index]->change_element(row, column, 0)) {
  case PlayerBoard::loc_type_e::INVALID:
    m_curr_msg = "You chose a place outside the boundaries of the board.";
    break;
  case PlayerBoard::loc_type_e::ORIGINAL:
    m_curr_play = player_action;
    m_curr_msg = "This is a value from the original board. It cannot be removed.";
    break;
  default:
    m_curr_play = player_action;
    m_undo_log[m_game_index].push_back(player_command);
    m_curr_msg = "";
    break;
  }
}

void SudokuGame::undo() {
  read_enter();
  if (m_undo_log[m_game_index].empty()) {
    m_curr_msg = "There are no previous moves. Therefore undo command can not be used.";
  } else {
    Command undo = m_undo_log[m_game_index].back();
    m_undo_log[m_game_index].pop_back();
    m_boards[m_game_index]->change_element(undo.data.row, undo.data.col, undo.data.value_replaced);
    m_curr_play = undo.data;
    m_curr_msg = "";
  }
}

void SudokuGame::checks_left() {
  read_enter();
  if (m_checks_left[m_game_index] > 0) {
    m_checks_left[m_game_index]--;
    m_curr_msg = "";
    read_enter();
  } else {
    m_curr_msg = "There are no checks left for you!";
  }
}

short SudokuGame::get_str_pos(short col) {
  if (col == 0)
    return 5;
  else if (col == 1)
    return 7;
  else if (col == 2)
    return 9;
  else if (col == 3)
    return 13;
  else if (col == 4)
    return 15;
  else if (col == 5)
    return 17;
  else if (col == 6)
    return 21;
  else if (col == 7)
    return 23;
  else if (col == 8)
    return 25;
  else
    return -1;
}

void SudokuGame::display_welcome() const {
  std::cout << "\n==============================================================\n";
  std::cout << "\tWelcome to a terminal version of sudoku, v1.0\n";
  std::cout << "\tCopyright(C) 2020, Selan R. dos Santos\n";
  std::cout << "==============================================================\n\n";
}

void SudokuGame::display_game_rules() const {
  std::string tmp
    = "-----------------------------------------------------------------------------------"
      "-------\n"
      "\tThe goal of Sudoku is to fill a 9x9 grid with numbers so that each row,\n"
      "\tcolumn and 3x3 section (nonet) contain all of the digits between 1 and 9.\n\n"
      "\tThe Sudoku rules are:\n"
      "\t1. Each row, column, and nonet can contain each number (typically 1 to 9)\n"
      "\t   exactly once.\n"
      "\t2. The sum of all numbers in any nonet, row, or column must be equal to 45.\n"
      "-----------------------------------------------------------------------------------"
      "-------\n\n";
  std::cout << Color::tcolor(tmp, Color::BRIGHT_MAGENTA);
}

void SudokuGame::display_commands_syntax() const {
  std::string tmp
    = "Commands syntax:\n\t\'enter\' (without typing anything)  -> go back to previous menu.\n"
      "\t\'p\' <row> <col> <number> + 'enter' -> place <number> on board at location "
      "(<row>, <col>).\n"
      "\t\'r\' <row> <col> + 'enter'          -> remove number already on the board at "
      "location (<row>, <col>).\n"
      "\t'c' + 'enter'                      -> check which moves made are correct.\n"
      "\t'u' + 'enter'                      -> undo last play.\n"
      "\t<col>, <number> must be in range [1,9].\n"
      "\t<row> must be in range [A,I].\n";
  std::cout << Color::tcolor(tmp, Color::BRIGHT_GREEN);
}

void SudokuGame::print_header(std::string str) const {
  string tmp = "\n|--------[" + str + "]--------|\n\n";
  std::cout << Color::tcolor(tmp, Color::BRIGHT_CYAN);
}

void SudokuGame::print_message(std::string str = "") const {
  string tmp = "MSG: [" + str + "]\n\n";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::print_line(char C, short idx, bool checking) {
  std::cout << C << " | ";
  for (short i = 0; i < SB_SIZE; i++) {
    if (i == 3 || i == 6)
      std::cout << "| ";

    PlayerBoard::element ele
      = checking ? m_boards[idx]->check_ans(C, i) : m_boards[idx]->consult(C, i);
    if (ele.status == PlayerBoard::loc_type_e::ORIGINAL) {
      std::string val = "  ";
      val[0] = ele.value + '0';
      std::cout << Color::tcolor(val, Color::BRIGHT_BLUE);
    } else if (ele.status == PlayerBoard::loc_type_e::INVALID) {
      std::string val = " ";
      val[0] = ele.value + '0';
      std::cout << Color::tcolor(val, Color::BRIGHT_RED);
      std::cout << " ";
    } else if (ele.status == PlayerBoard::loc_type_e::CORRECT) {
      std::string val = " ";
      val[0] = ele.value + '0';
      std::cout << Color::tcolor(val, Color::BRIGHT_GREEN);
      std::cout << " ";
    } else if (ele.status == PlayerBoard::loc_type_e::INCORRECT) {
      std::string val = " ";
      val[0] = ele.value + '0';
      std::cout << Color::tcolor(val, Color::BRIGHT_RED);
      std::cout << " ";
    } else
      std::cout << "  ";
  }
  std::cout << "|\n";
}

void SudokuGame::print_board(short idx, bool checking = false, char row, short col) {
  for (short i = 0; i < 27; i++) {
    if (i == get_str_pos(m_curr_play.col))
      std::cout << Color::tcolor("V", Color::BRIGHT_YELLOW);
    else
      std::cout << " ";
  }

  std::cout << "\n";

  std::cout << "     1 2 3   4 5 6   7 8 9\n";
  std::cout << "   +-------+-------+-------+\n";
  for (short i = 0; i < SB_SIZE; i++) {
    if (i % 3 == 0 and i != 0)
      std::cout << "   |-------+-------+-------|\n";
    if ('A' + i == m_curr_play.row)
      std::cout << Color::tcolor(">", Color::BRIGHT_YELLOW);
    else
      std::cout << " ";
    print_line('A' + i, idx, checking);
  }
  std::cout << "   +-------+-------+-------+\n";
}

void SudokuGame::print_checks_left() const {
  string tmp = "Checks left: [" + std::to_string(m_checks_left[m_game_index]) + "]\n";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::print_digits_left() {
  string tmp = "Digits Left: [";
  for (short num = 1; num <= 9; num++) {
    if (m_boards[m_game_index]->still_missing(num))
      tmp += std::to_string(num) + " ";
    else
      tmp += "  ";
  }
  tmp += "]\n";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::display_opt() const {
  string tmp = "1-Play 2-New Game 3-Quit 4-Help 5-Load Game 6-Save Game 7-Hardest Match\n";
  std::cout << Color::tcolor(tmp, Color::GREEN);
  tmp = "Select option [1,7] > ";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::display_opening_input_file() const {
  string tmp = ">>> Opening input file [" + m_opt.input_filename
               + "].\n"
                 ">>> Processing data, please wait.\n";
  std::cout << Color::tcolor(tmp, Color::BRIGHT_GREEN);
}

void SudokuGame::print_match_status() {
  string tmp = "You made " + std::to_string(m_moves[m_game_index]) + " moves, in which ";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
  if (m_incorrect_moves[m_game_index] == 0) {
    tmp = Color::tcolor("none was invalid. Amazing!!!\n", Color::YELLOW);
  } else if (m_incorrect_moves[m_game_index] == 1) {
    tmp = Color::tcolor(" 1 was invalid. Not bad!\n", Color::YELLOW);
  } else {
    tmp = std::to_string(m_incorrect_moves[m_game_index]) + " were invalid. It could be better!\n";
  }
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::print_press_enter() const {
  string tmp = ">>> Press enter to continue";
  std::cout << Color::tcolor(tmp, Color::YELLOW);
}

void SudokuGame::print_hardest_confirmation() {
  string tmp = "You really want to do that? I am warning you!\n"
               "If you are say \"I am brave enough!\", otherwise say \"I am a little "
               "chicken\"\n>>> ";
  std::cout << Color::tcolor(tmp, Color::BRIGHT_RED);
}
}  // namespace sdkg
