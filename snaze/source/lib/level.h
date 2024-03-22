#ifndef LEVEL_H
#define LEVEL_H

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

class Level {
public:
  struct coordinate {
    short x;
    short y;
    coordinate(short x = 0, short y = 0) {
      this->x = x;
      this->y = y;
    }
  };

  enum position_e {
    WALL = '#',
    HEAD = '&',
    BODY = '=',
    FREE = ' ',
    INVISIBLE = '.',
    FOOD = '@',
    INVALID = '!'
  };

private:
  std::string m_err_msg; //!< Error message in case level is invalid.
  size_t m_height;       //!< Height of the level matrix.
  size_t m_width;        //!< Width of the level matrix.
  std::optional<coordinate> m_spawn; //!< position of the spawn.
  coordinate m_food;                 //!< position of the food.
  std::vector<position_e> m_matrix;  //!< Level matrix.

public:
  /// Reads a level data from a file.
  Level() = delete;
  /// Instantiates class
  Level(size_t height, size_t width, std::vector<position_e> matrix,
        std::optional<coordinate> spawn);
  Level(const Level &copy) = default;
  /// Assign a value in the level matrix.
  void assign(size_t row, size_t column, position_e character);
  /// Consults a position in the level matrix.
  position_e consult(size_t row, size_t column) {
    return m_matrix[row * m_width + column];
  }
  /// Consults the spawn location of the snake.
  coordinate get_spawn() const { return m_spawn.value(); }
  /// Consults the food location.
  coordinate get_food_pos() const { return m_food; }
  /// Consults the level height.
  size_t get_height() const { return m_height; }
  /// Consults the level width.
  size_t get_width() const { return m_width; }
  /// Consults error message
  std::string get_err_msg() const { return m_err_msg; }
  /// Generates a food randomly in the level matrix
  void generates_food();
};

#endif
