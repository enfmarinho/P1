#include "sudoku_gm.h"
#include "../lib/text_color.h"

#include <iostream>
#include <memory>
#include <string>
#include <_ctype.h>

namespace sdkg {
void SudokuGame::process_events() {
  if (m_game_state == game_state_e::STARTING or m_game_state == game_state_e::ASKING_TO_QUIT
      or m_game_state == game_state_e::WELCOMING) {
    // Do nothing
  } else if (m_game_state == game_state_e::BEGGINING_LOAD_PUZZLES) {
    read_puzzles();
  } else if (m_game_state == game_state_e::ENDING_LOAD_PUZZLES
             or m_game_state == game_state_e::HELPING or m_game_state == game_state_e::PUZZLE_WON) {
    m_curr_msg = "";
    read_enter();
  } else if (m_game_state == game_state_e::READING_MAIN_OPT) {
    m_curr_msg = "";
    m_curr_main_menu_opt = read_main_menu_opt();
  } else if (m_game_state == game_state_e::REQUESTING_NEW_GAME) {
    (m_game_index + 1 == m_boards.size()) ? m_game_index = 1 : m_game_index++;
    m_curr_msg = "New puzzle loaded.";
  } else if (m_game_state == game_state_e::REQUESTING_HARDEST_GAME) {
    if (confirm_hardest_match()) {
      m_game_index = 0;
      m_curr_msg = "Good luck! You will need it.";
    } else {
      m_curr_msg = "You are a coward!";
    }
  } else if (m_game_state == game_state_e::LOADING_OLD_GAME) {
    string filename;
    std::cin >> filename;
    try {
      load_old_game(filename);
      m_game_index = m_boards.size() - 1;
      m_curr_msg = "Game successfully loaded.";
    } catch (string& error_message) {
      m_curr_msg = error_message;
    }
  } else if (m_game_state == game_state_e::SAVING_PUZZLE and check_ongoing_match()) {
    std::string filename;
    std::cin >> filename;
    save_ongoing_game(filename);
  } else if (m_game_state == game_state_e::SAVING_PUZZLE and not check_ongoing_match()) {
    m_curr_msg = "There are no ongoing games to save.";
  } else if (m_game_state == game_state_e::CONFIRMING_QUITTING_MATCH) {
    m_quitting_match = confirm_quitting();
    if (m_quitting_match) {
      m_curr_msg = "";
    } else {
      m_curr_msg = "You chose not to give up. Great!";
    }
  } else if (m_game_state == game_state_e::PLAYING_MODE) {
    m_curr_msg = "";
    m_curr_play_menu_opt = read_user_play();
    m_curr_play = Play();
  } else if (m_game_state == game_state_e::VALIDATING_PLAY) {
    play();
  } else if (m_game_state == game_state_e::REMOVING_PLAY) {
    remove();
  } else if (m_game_state == game_state_e::UNDOING_PLAY) {
    undo();
  } else if (m_game_state == game_state_e::CHECKING_MOVES) {
    checks_left();
  }
}

void SudokuGame::update() {
  if (m_game_state == game_state_e::STARTING) {
    m_game_state = game_state_e::WELCOMING;
  } else if (m_game_state == game_state_e::WELCOMING) {
    m_game_state = game_state_e::BEGGINING_LOAD_PUZZLES;
  } else if (m_game_state == game_state_e::BEGGINING_LOAD_PUZZLES) {
    m_game_state = game_state_e::ENDING_LOAD_PUZZLES;
  } else if (m_game_state == game_state_e::ENDING_LOAD_PUZZLES) {
    m_game_state = game_state_e::READING_MAIN_OPT;
  } else if (m_game_state == game_state_e::READING_MAIN_OPT) {
    switch (m_curr_main_menu_opt) {
    case main_menu_opt_e::HELP:
      m_game_state = game_state_e::HELPING;
      break;
    case main_menu_opt_e::PLAY:
      m_game_state = game_state_e::PLAYING_MODE;
      m_curr_msg = "Good luck!";
      break;
    case main_menu_opt_e::LOAD:
      m_game_state = game_state_e::LOADING_OLD_GAME;
      break;
    case main_menu_opt_e::SAVE:
      m_game_state = game_state_e::SAVING_PUZZLE;
      break;
    case main_menu_opt_e::NEW_GAME:
      m_game_state = game_state_e::REQUESTING_NEW_GAME;
      break;
    case main_menu_opt_e::QUIT:
      if (check_ongoing_match()) {
        m_game_state = game_state_e::ASKING_TO_QUIT;
        m_curr_msg = "Are you sure you want to quit?";
      } else {
        m_game_state = game_state_e::CONFIRMING_QUITTING_MATCH;
        m_quitting_match = true;
      }
      break;
    case main_menu_opt_e::HARDEST:
      if (m_game_index == 0) {
        m_curr_msg = "You are already at the hardest puzzle.";
      } else {
        m_game_state = game_state_e::REQUESTING_HARDEST_GAME;
      }
      break;
    default:
      m_curr_msg = "Invalid option";
      break;
    }
  } else if (m_game_state == game_state_e::REQUESTING_HARDEST_GAME) {
    if (m_game_index == 0) {
      m_game_state = game_state_e::PLAYING_MODE;
    } else {
      m_game_state = game_state_e::READING_MAIN_OPT;
    }
  } else if (m_game_state == game_state_e::VALIDATING_PLAY and m_boards[m_game_index]->game_ended()
             and check_victory()) {
    m_game_state = game_state_e::PUZZLE_WON;
  } else if (m_game_state == game_state_e::HELPING
             or m_game_state == game_state_e::REQUESTING_NEW_GAME
             or m_game_state == game_state_e::LOADING_OLD_GAME
             or m_game_state == game_state_e::SAVING_PUZZLE) {
    m_game_state = game_state_e::READING_MAIN_OPT;
  } else if (m_game_state == game_state_e::ASKING_TO_QUIT) {
    m_game_state = game_state_e::CONFIRMING_QUITTING_MATCH;
  } else if (m_game_state == game_state_e::CONFIRMING_QUITTING_MATCH and not m_quitting_match) {
    m_game_state = game_state_e::READING_MAIN_OPT;
  } else if (m_game_state == game_state_e::PLAYING_MODE) {
    switch (m_curr_play_menu_opt) {
    case play_opt_e::REMOVE:
      m_game_state = game_state_e::REMOVING_PLAY;
      break;
    case play_opt_e::PLACE:
      m_game_state = game_state_e::VALIDATING_PLAY;
      break;
    case play_opt_e::MENU:
      m_game_state = game_state_e::READING_MAIN_OPT;
      break;
    case play_opt_e::UNDO:
      m_game_state = game_state_e::UNDOING_PLAY;
      break;
    case play_opt_e::CHECK:
      m_game_state = game_state_e::CHECKING_MOVES;
      break;
    default:
      m_curr_msg = "Invalid command";
      break;
    }
  } else if (m_game_state == game_state_e::REMOVING_PLAY
             or m_game_state == game_state_e::VALIDATING_PLAY
             or m_game_state == game_state_e::UNDOING_PLAY
             or m_game_state == game_state_e::CHECKING_MOVES) {
    m_game_state = game_state_e::PLAYING_MODE;
  } else if (m_game_state == game_state_e::PUZZLE_WON) {
    m_game_state = game_state_e::REQUESTING_NEW_GAME;
  }
}

void SudokuGame::render() {
  if (m_game_state == game_state_e::STARTING or m_game_state == game_state_e::REQUESTING_NEW_GAME
      or m_game_state == game_state_e::REMOVING_PLAY
      or m_game_state == game_state_e::VALIDATING_PLAY
      or m_game_state == game_state_e::UNDOING_PLAY) {
    // Do nothing.
  } else if (m_game_state == game_state_e::WELCOMING) {
    display_welcome();
  } else if (m_game_state == game_state_e::BEGGINING_LOAD_PUZZLES) {
    display_opening_input_file();
  } else if (m_game_state == game_state_e::ENDING_LOAD_PUZZLES) {
    if (m_quitting_match) {
      std::cout << Color::tcolor(">>> " + m_curr_msg + "\n", Color::BRIGHT_RED);
    } else {
      std::cout << Color::tcolor(">>> " + m_curr_msg + "\n", Color::BRIGHT_GREEN);
      print_press_enter();
    }
  } else if (m_game_state == game_state_e::READING_MAIN_OPT
             or m_game_state == SudokuGame::game_state_e::ASKING_TO_QUIT) {
    print_header((std::string) "MAIN SCREEN");
    print_board(m_game_index, true);
    print_message(m_curr_msg);
    if (m_game_state == game_state_e::READING_MAIN_OPT)
      display_opt();
  } else if (m_game_state == game_state_e::HELPING) {
    display_game_rules();
    print_press_enter();
  } else if (m_game_state == game_state_e::LOADING_OLD_GAME) {
    print_message("Write the file's path.");
  } else if (m_game_state == game_state_e::SAVING_PUZZLE) {
    if (check_ongoing_match())
      print_message("Write the file's path.");
  } else if (m_game_state == game_state_e::CONFIRMING_QUITTING_MATCH and not m_quitting_match) {
    std::cout << Color::tcolor("Your choice [y/N] >", Color::YELLOW);
  } else if (m_game_state == game_state_e::PLAYING_MODE) {
    print_header((std::string) "ACTION MODE");
    if (m_undo_log.empty()) {
      print_board(m_game_index, false, m_curr_play.row, m_curr_play.col);
    } else {
      print_board(m_game_index, false, m_curr_play.row, m_curr_play.col);
    }
    print_checks_left();
    print_digits_left();
    print_message(m_curr_msg);
    display_commands_syntax();
    string tmp = "Enter command > ";
    std::cout << Color::tcolor(tmp, Color::YELLOW);
  } else if (m_game_state == game_state_e::CHECKING_MOVES) {
    if (m_checks_left[m_game_index] > 0)
      print_board(m_game_index, true);
    print_press_enter();
  } else if (m_game_state == game_state_e::REQUESTING_HARDEST_GAME) {
    print_hardest_confirmation();
  } else if (m_game_state == game_state_e::PUZZLE_WON) {
    print_header("ACTION MODE");
    print_board(m_game_index, true);
    print_checks_left();
    print_digits_left();
    print_message("Congratulations, you solved the puzzle!");
    print_match_status();
    print_press_enter();
  }
}
}  // namespace sdkg
