#include "level.h"
#include <cstddef>
#include <iostream>
#include <random>

size_t get_random_number(size_t smallest, size_t greatest) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(smallest, greatest);
  return distribution(gen);
}

void Level::assign(size_t row, size_t column, position_e character) {
  m_matrix[row * m_width + column] = character;
}

Level::Level(size_t height, size_t width, std::vector<position_e> matrix,
             std::optional<coordinate> spawn) {
  m_height = height;
  m_width = width;
  m_spawn = spawn;
  m_matrix = matrix;
}

void Level::generates_food() {
  int random = get_random_number(0, m_width * m_height);
  int counter{0};
  while (m_matrix[random] != position_e::FREE and
         counter < m_width * m_height) {
    ++counter;
    ++random;
    random %= m_width * m_height;
  }

  if (counter < m_height * m_width) {
    short x = random % m_width;
    short y = random / m_width;
    m_food = coordinate(x, y);
    assign(y, x, position_e::FOOD);
  }
}
