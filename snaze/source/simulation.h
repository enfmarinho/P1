#ifndef __SIMULATION__
#define __SIMULATION__

#include "lib/CLI.h"
#include "lib/level.h"
#include "lib/snake.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

namespace sg {

class SnazeSimulation {
public:
  typedef std::pair<short, short> Position;
  typedef std::vector<std::vector<SnazeSimulation::Position>> Parents;
  typedef std::vector<std::vector<bool>> Visited;

  enum game_state_e {
    READING = 0,    //!< Reads input file.
    WELCOME,        //!< Displays welcome message.
    THINKING,       //!< Search for the snakes path.
    MOVING,         //!< Moves the snake.
    CHANGING_LEVEL, //!< Changes the level.
    WINNING,        //!< Goes to the next level.
    LOOSING,        //!< Restarts current level.
    ENDING,         //!< Ending game
  };

  CLI m_arguments;                              //!< Command-line arguments.
  game_state_e m_game_state;                    //!< STATES
  Snake m_snake;                                //!< Snake simulation.
  size_t m_number_levels{0};                    //!< Number of levels.
  std::vector<Level> m_levels;                  //!< Levels of the simulation.
  size_t m_level_index{0};                      //!< Index of the current level.
  Position m_head;                              //!< Position of the head
  std::deque<SnazeSimulation::Position> m_tail; //!< Position of the tail, FIFO
  std::deque<SnazeSimulation::Position>
      m_moves; //!< Moves of the snake, FIFO, not the one for the algorithm,
               //!< moves post-calculation

  SnazeSimulation() = default;
  /// Initializes variables and reads input file
  void initialize(int argc, char *argv[]);
  /// Process game state events
  void process_events();
  /// Update game state
  void update();
  /// Rendering output
  void render();
  /// Checks whether game should end.
  bool is_over();

private:
  /// Reads input file.
  bool read_input_file();
  /// Displays welcome message
  void display_welcome() const;
  /// Displays board of the match.
  void display_board(bool alive = true);
  /// Displays information of the match.
  void display_information();
  /// Displays confirmation of changing match.
  void display_confirmation(bool died);
  /// Displays victory message.
  void display_victory_message();
  /// Displays defeated message.
  void display_defeated_message();
  /// Performs a BFS algorithm.
  bool bfs();
  /// Make the snake go in random direction(as far as it cans).
  void random(SnazeSimulation::Position p, Visited &v);
  /// Auxiliary functions for the bfs algorithm.
  bool aux_bfs(SnazeSimulation::Position u, SnazeSimulation::Position v,
               const Parents &p);
  /// Checks whether a movement is not valid.
  bool invalid_bfs(SnazeSimulation::Position p, short x, short y,
                   const Visited &v);
};

}; // namespace sg

#endif
