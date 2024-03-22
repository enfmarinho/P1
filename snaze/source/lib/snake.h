#ifndef SNAKE_H
#define SNAKE_H

#include "level.h"
#include <cstddef>
#include <deque>
#include <utility>

class Snake {
public:
  enum direction_e {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
  };

private:
  std::deque<Level::coordinate> m_body; //!< Coordinates for snake body.
  Level *m_level;          //!< Reference to current level being simulated.
  short m_foods_remaining; //!< Number of foods left to eat.
  short m_lives_remaining; //!< Number of lives remaining.
  short m_foods_per_level; //!< Number of lives per level.
  direction_e m_direction; //!< Current direction of the snake.
  int m_score{0};          //!< Score of the snake.
  bool m_alive{true};      //!< Indicates whether the snake is alive.
  /*!
   * Add a new 'node' after the snake's head in the chosen direction.
   * \param direction direction to move.
   */
  void append(direction_e direction);

public:
  /// Deallocates m_level.
  ~Snake();
  /// Instantiates class.
  void initialize(short lives, short foods);
  /// Consults coordinates to snake's head.
  [[nodiscard]] Level::coordinate get_head_coord() const {
    return m_body.front();
  };
  /// Consults the position of the food.
  [[nodiscard]] Level::coordinate get_food_pos() const {
    return m_level->get_food_pos();
  }
  /// Consults current board
  [[nodiscard]] Level::position_e consult(size_t row, size_t col) const {
    return m_level->consult(row, col);
  }
  /// Gets the position of the snake's body.
  [[nodiscard]] std::deque<std::pair<short, short>> get_tail() const {
    std::deque<std::pair<short, short>> tmp;
    for (auto it = --m_body.end(); it != m_body.begin(); --it) {
      tmp.push_back({it->y, it->x});
    }
    return tmp;
  };
  /// Consults number of foods remaining.
  [[nodiscard]] short get_foods_remaining() const { return m_foods_remaining; }
  /// Consults number of lives remaining.
  [[nodiscard]] short get_lives_remaining() const { return m_lives_remaining; }
  /// Consults current direction of the snake.
  [[nodiscard]] direction_e get_direction() const { return m_direction; }
  /// Consults whether the snake are alive.
  [[nodiscard]] bool alive() const { return m_alive; }
  /// Consults the score of the snake.
  [[nodiscard]] int get_score() const { return m_score; }
  /// Kills the snake
  void kill();
  /*!
   * Move snake to chosen direction. In practice removes its tail and append its
   * head to the chosen direction.
   * \param direction direction to move.
   */
  void move(direction_e direction);
  /*!
   * Assign a level for the snake to run.
   * \param level pointer to Level class to simulate.
   */
  void assign_level(Level &level);
  /// Restart current level.
  void restart_level(Level &level);
};

#endif
