#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

/*!
 *  Sudoku game class header.
 */

#include <cstddef>
#include <deque>
#include <memory>
#include <vector>

#include "../lib/text_color.h"
#include "sudoku_board.h"

namespace sdkg {

/// Game class representing a Life Game simulation manager.
class SudokuGame {
private:
  //=== Structs

  //!< Possible game states.
  enum class game_state_e : uint {
    STARTING = 0,            //!< Beginning the game.
    WELCOMING,               //!< Welcome menu.
    BEGGINING_LOAD_PUZZLES,  //!< Loading puzzles from input file.
    ENDING_LOAD_PUZZLES,     //
    LOADING_OLD_GAME,        //!< Loading puzzles from old game.
    SAVING_PUZZLE,           //!< Salving last open game ongoing.
    //====================================================
    READING_MAIN_OPT,  //!< Reading main menu option.
    //----------------------------------------------------
    HELPING,         //!< Displaying game rules (main menu)
    ASKING_TO_QUIT,  //!< Game is quiting either because user requested (main menu).
    //====================================================
    PLAYING_MODE,  //!< Reading user command (Play sub-menu).
    //----------------------------------------------------
    REQUESTING_NEW_GAME,        //!< User wants to start a new game.
    REQUESTING_HARDEST_GAME,    //!< User asked to the hardest match.
    CONFIRMING_QUITTING_MATCH,  //!< User has already started a match. We need
                                //!< to confirm s/he wants to bail out.
    VALIDATING_PLAY,            //!< User wants to place a number on the board.
    REMOVING_PLAY,              //!< User wants to remove a number from the board.
    CHECKING_MOVES,             //!< User wants to check whether his previous moves are
                                //!< correct.
    QUITTING_PLAY_MODE,         //!< User wants to go back to main menu.
    UNDOING_PLAY,               //!< User wants to undo a previous move he did.
    // ====================================================
    PUZZLE_WON  //!< User has won a puzzle.
  };

  /// Internal game options
  struct Options {
    std::string input_filename;  //!< Input cfg file.
    short total_checks;          //!< # of checks user has left.
  };

  /// Options from the main menu.
  enum main_menu_opt_e : uint {
    PLAY = 0,   //!< Play option.
    NEW_GAME,   //!< New puzzle option.
    QUIT,       //!< Quit option.
    HELP,       //!< Print help option.
    LOAD,       //!< Load old game.
    SAVE,       //!< Save old game.
    HARDEST,    //!< Request hardest game.
    N_OPTIONS,  //!< Total number of options.
    INVALID     //!< Invalid option.
  };

  enum play_opt_e : uint {
    MENU = 0,  //!< Menu option.
    PLACE,     //!< Place element option.
    REMOVE,    //!< Remove element option.
    CHECK,     //!< Check board option.
    UNDO,      //!< Undo option.
    INVALID_P  //!< Invalid option.
  };

  /// Represents a user move.
  struct Play {
    char row;              //!< row selected by the user.
    short col;             //!< col selected by the user.
    short value;           //!< value to play selected by the user.
    short value_replaced;  //!< Value before the change.
    /// Constructor.
    Play(char r = -1, short c = -1, short v = 0, short v_r = 0)
        : row{ r }, col{ c }, value{ v }, value_replaced(v_r) { /*empty*/
    }
  };

  /// Represents a command issued during a match (UNDO command).
  struct Command {
    enum class type_e : uint {
      PLACE = 0,  //!< Place command.
      REMOVE      //!< Remove command.
    } action;
    Play data;  //!< Play data.

    Command(type_e a, Play p) : action{ a }, data{ p } { /* empty */
    }
  };

  //=== Data members
  Options m_opt;                    //!< Overall game Options to set up the game configuration.
  game_state_e m_game_state;        //!< Current game state.
  std::string m_curr_msg;           //!< Current message to display on screen.
  Play m_curr_play;                 //!< Current user play.
  play_opt_e m_curr_play_menu_opt;  //!< Current play menu option.
  bool m_quitting_match;  //!< Flag that indicates whether the user wants to end an ongoing game.
  std::vector<short> m_checks_left;                    //!< Current # of checks user can request.
  main_menu_opt_e m_curr_main_menu_opt;                //!< Current main menu option.
  std::vector<std::deque<Command>> m_undo_log;         //!< Log of commands to support undoing.
  std::vector<std::shared_ptr<PlayerBoard>> m_boards;  //!< List of matches.
  short m_game_index;                                  //!< Index to current game.
  std::vector<int> m_moves;                            //!< List with number of moves per game.
  std::vector<int> m_incorrect_moves;     //!< List with number of invalid moves per game.
  std::vector<size_t> m_invalid_puzzles;  //!< List of invalid puzzles indexes in the input file.

  /// Reads puzzles from input file.
  void read_puzzles();
  /// Reads old game.
  void load_old_game(const string& filename);
  /*!
   * Checks if board solution is valid.
   * \parameter board board to be checked.
   * \return flag that indicates whether the board is valid.
   */
  bool check_board(std::shared_ptr<PlayerBoard> board);
  /*!
   * Saves last opened ongoing game.
   * \parameter filename file path to save data on.
   */
  void save_ongoing_game(const string& filename);
  /*!
   * Reads user option on menu from the standard input stream.
   * \return enumeration representing option.
   */
  main_menu_opt_e read_main_menu_opt();
  /*!
   * Reads user play from the standard input stream.
   * \return enumeration representing user play.
   */
  play_opt_e read_user_play();
  /// Auxiliary function to play option in PLAYING_MODE.
  void play();
  /// Auxiliary function to remove option in PLAYING_MODE.
  void remove();
  /// Auxiliary function to undo option in PLAYING_MODE.
  void undo();
  /// Auxiliary function to check option in PLAYING_MODE.
  void checks_left();
  /*!
   * Checks if there is an ongoing match.
   * \return flag that indicates whether the user has an ongoing match.
   */
  bool check_ongoing_match() const;
  /*!
   * Checks if user has won the puzzle.
   * \return flag that indicates whether the user has won the puzzle.
   */
  bool check_victory();
  /// displays game rules.
  void display_game_rules() const;
  /// displays welcome screen.
  void display_welcome() const;
  /// displays the commands syntax.
  void display_commands_syntax() const;
  /// displays the header.
  void print_header(std::string str) const;
  /// prints the message on the screen.
  void print_message(std::string str) const;
  /// Prints message to alert user to press enter.
  void print_press_enter() const;
  /// informs that the input file is being read.
  void display_opening_input_file() const;
  /// gets the position of the column indicator in the string to be printed.
  short get_str_pos(short col);
  /**
   * Prints each row of the board.
   *
   * @param C the row of the board to be printed.
   * @param idx the index of the board that is being printed.
   * @param checking flag for print a checking board.
   */
  void print_line(char C, short idx, bool checking);
  /**
   * Prints the board.
   *
   * @param idx the index of the board to be printed.
   * @param checking flag that indicates if its a board that has been checked.
   * @param row the row of the last element placed in the board.
   * @param col the columns of the last element placed in the board.
   */
  void print_board(short idx, bool checking, char row = '-', short col = -1);
  /// Prints the number of checks left.
  void print_checks_left() const;
  /// Prints status of the match.
  void print_match_status();
  /// Prints the digits left.
  void print_digits_left();
  /// Prints message of confirmation to the hardest game.
  void print_hardest_confirmation();
  /// Displays the menu options.
  void display_opt() const;
  /// Reads enter from standard input.
  void read_enter() const;
  /// Reads user choice to confirm decision.
  bool confirm_hardest_match();
  /// Reads user confirmation.
  bool confirm_quitting() const;

public:
  SudokuGame() = default;
  ~SudokuGame() = default;

  void initialize(int argc, char* argv[]);
  void process_events();
  void update();
  void render();
  /*!
   * Check if game should end.
   * \return flag that indicates whether the game should end.
   */
  bool game_over() const;

};  // SudokuGame class.
}  // namespace sdkg
#endif
