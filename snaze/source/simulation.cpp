#include "simulation.h"
#include "lib/CLI.h"
#include "lib/level.h"
#include "lib/snake.h"
#include "lib/text_color.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

using namespace sg;

/// Prints 50 '-'
void print_line() {
  for (int counter = 0; counter < 50; ++counter) {
    std::cout << Color::tcolor("-", Color::BRIGHT_CYAN);
  }
  std::cout << '\n';
}

void SnazeSimulation::initialize(int argc, char *argv[]) {
  if (not m_arguments.read_command_line(argc, argv)) {
    m_game_state = game_state_e::ENDING;
  } else {
    m_game_state = game_state_e::READING;
  }
  m_snake.initialize(m_arguments.get_number_lives(),
                     m_arguments.get_number_food());
}

bool SnazeSimulation::read_input_file() {
  std::ifstream file(m_arguments.get_input_file());
  if (not file.good()) {
    string message = "File could not be read. Ending execution.\n";
    std::cout << Color::tcolor(message, Color::BRIGHT_RED);
  }
  string err_msg;
  while (true) {
    std::optional<Level::coordinate> spawn;
    err_msg = "";
    size_t height, width;
    if (not(file >> height)) {
      break;
    }
    if (not(file >> width)) {
      break;
    }
    file.get();
    std::vector<Level::position_e> matrix;
    matrix.resize(height * width);
    for (size_t row{0}; row < height; ++row) {
      for (size_t column{0}; column < width; ++column) {
        char tmp;
        tmp = file.get();

        Level::position_e pos;
        switch (tmp) {
        case '#':
          pos = Level::position_e::WALL;
          break;
        case '&':
          if (spawn) {
            err_msg = "Level invalid due to multiples spawns locations.";
          }
          pos = Level::position_e::HEAD;
          spawn = Level::coordinate(column, row);
          break;
        case '.':
          pos = Level::position_e::INVISIBLE;
          break;
        case ' ':
          pos = Level::position_e::FREE;
          break;
        default:
          pos = Level::position_e::INVALID;
          err_msg = "Level invalid due to an incorrect character in the tabel.";
          break;
        }
        matrix[row * width + column] = pos;
      }
      file.get();
    }

    if (not spawn) {
      err_msg = "Level invalid. Spawn has not been defined.";
    } else if (err_msg != "") {
      std::cout << Color::tcolor(err_msg + '\n', Color::BRIGHT_RED);
    } else {
      m_levels.emplace_back(Level(height, width, matrix, spawn));
      ++m_number_levels;
    }
  }
  file.close();
  return m_levels.size() != 0;
}

void SnazeSimulation::process_events() {
  if (m_game_state == game_state_e::READING) {
    if (not read_input_file()) {
      m_game_state = game_state_e::ENDING;
    } else {
      m_snake.assign_level(m_levels[m_level_index]);
    }
  } else if (m_game_state == game_state_e::WELCOME) {
    string s;
    getline(std::cin, s);
  } else if (m_game_state == game_state_e::CHANGING_LEVEL) {
    m_snake.assign_level(m_levels[++m_level_index]);
  } else if (m_game_state == game_state_e::LOOSING and
             m_snake.get_lives_remaining() > 0) {
    m_snake.restart_level(m_levels[m_level_index]);
    string s;
    getline(std::cin, s);
  } else if (m_game_state == game_state_e::WINNING and
             m_level_index < m_number_levels - 1) {
    string s;
    getline(std::cin, s);
  } else if (m_game_state == game_state_e::THINKING or
             m_game_state == game_state_e::MOVING or
             m_game_state == game_state_e::ENDING) {
    // Do nothing.
  }
}

void SnazeSimulation::update() {
  if (m_game_state == game_state_e::READING) {
    m_game_state = game_state_e::WELCOME;
  } else if (m_game_state == game_state_e::WELCOME) {
    m_game_state = game_state_e::THINKING;
  } else if (m_game_state == game_state_e::THINKING) {
    // Calculate the moves.
    if (!bfs()) {
      m_moves.clear();
      Visited v(m_levels[m_level_index].get_height(),
                std::vector<bool>(m_levels[m_level_index].get_width()));
      v[m_snake.get_head_coord().y][m_snake.get_head_coord().x] = true;
      random({m_snake.get_head_coord().y, m_snake.get_head_coord().x}, v);
    }
    m_game_state = game_state_e::MOVING;
  } else if (m_game_state == game_state_e::MOVING and not m_snake.alive()) {
    m_game_state = game_state_e::LOOSING;
  } else if (m_game_state == game_state_e::MOVING and
             m_snake.get_foods_remaining() == 0) {
    m_game_state = game_state_e::WINNING;
  } else if (m_game_state == game_state_e::MOVING and m_moves.empty()) {
    m_game_state = game_state_e::THINKING;
  } else if (m_game_state == game_state_e::WINNING and
             m_level_index < m_number_levels - 1) {
    m_game_state = game_state_e::CHANGING_LEVEL;
  } else if (m_game_state == game_state_e::LOOSING and
             m_snake.get_lives_remaining() > 0) {
    m_game_state = game_state_e::THINKING;
  } else if (m_game_state == game_state_e::LOOSING or
             m_game_state == game_state_e::WINNING) {
    m_game_state = game_state_e::ENDING;
  } else if (m_game_state == game_state_e::CHANGING_LEVEL) {
    m_game_state = game_state_e::THINKING;
  } else if (m_game_state == game_state_e::ENDING) {
    // Do nothing
  }
}

void SnazeSimulation::render() {
  if (m_game_state == game_state_e::WELCOME) {
    display_welcome();
  } else if (m_game_state == game_state_e::MOVING) {
    std::pair<short, short> head_pos = {m_snake.get_head_coord().y,
                                        m_snake.get_head_coord().x};
    auto next_move = m_moves.front();
    m_moves.pop_front();
    if (head_pos.second > next_move.second) {
      m_snake.move(Snake::direction_e::LEFT);
    } else if (head_pos.second < next_move.second) {
      m_snake.move(Snake::direction_e::RIGHT);
    } else if (head_pos.first < next_move.first) {
      m_snake.move(Snake::direction_e::DOWN);
    } else if (head_pos.first > next_move.first) {
      m_snake.move(Snake::direction_e::UP);
    }
    std::cout << "\n";
    display_information();
    print_line();
    display_board();
    print_line();
    // Limit fps.
    std::chrono::milliseconds duration{1000 /
                                       m_arguments.get_animation_speed()};
    std::this_thread::sleep_for(duration);
  } else if (m_game_state == game_state_e::WINNING) {
    if (m_level_index == m_number_levels - 1) {
      display_victory_message();
    } else {
      display_confirmation(false);
      display_information();
      print_line();
      display_board();
      print_line();
    }
  } else if (m_game_state == game_state_e::LOOSING) {
    display_confirmation(true);
    display_information();
    print_line();
    display_board(false);
    print_line();
    if (m_snake.get_lives_remaining() == 0) {
      display_defeated_message();
    }
  } else if (m_game_state == game_state_e::READING or
             m_game_state == game_state_e::THINKING or
             m_game_state == game_state_e::ENDING) {
    // Do nothing.
  }
}

bool SnazeSimulation::is_over() { return m_game_state == game_state_e::ENDING; }

void SnazeSimulation::display_welcome() const {
  std::cout << "--> Welcome to the classic Snake Game <--\n";
  std::cout << "\tcopyright DIMAp/UFRN 2017-2023\n";
  std::cout << "------------------------------------------------------------\n";
  std::cout << "Levels loaded: " << m_number_levels
            << " | Snake lives: " << m_arguments.get_number_lives()
            << " | Apples to eat: " << m_arguments.get_number_food() << "\n";
  std::cout << "Clear all levels to win the game. GANBATTE!!!\n";
  std::cout << "------------------------------------------------------------\n";
  std::cout << ">>> Press <ENTER> to start the game!\n";
}

void SnazeSimulation::display_confirmation(bool died) {
  std::cout << "\n>>> Press <ENTER> to ";
  if (died) {
    std::cout << "try again.";
  } else {
    std::cout << "start next level!";
  }
}

void SnazeSimulation::display_information() {
  string message = "\nLives: " + std::to_string(m_snake.get_lives_remaining()) +
                   "  |  Score: " + std::to_string(m_snake.get_score()) +
                   "  |  Food eaten: " +
                   std::to_string(m_arguments.get_number_food() -
                                  m_snake.get_foods_remaining()) +
                   +" of " + std::to_string(m_arguments.get_number_food()) +
                   "\n\n";
  std::cout << Color::tcolor(message, Color::BRIGHT_CYAN);
}

void SnazeSimulation::display_victory_message() {
  string message = "+--------------------------------+\n"
                   "|  CONGRATILATIONS Anacond WON!  |\n"
                   "|       Thanks for playing!      |\n"
                   "+--------------------------------+\n";
  std::cout << Color::tcolor(message, Color::BRIGHT_GREEN);
}

void SnazeSimulation::display_defeated_message() {
  string message = "+--------------------------------+\n"
                   "|      SORRY not this time!      |\n"
                   "|      Thanks for playing!       |\n"
                   "+--------------------------------+\n";
  std::cout << Color::tcolor(message, Color::BRIGHT_RED);
}

void SnazeSimulation::display_board(bool alived) {
  std::cout << '\n';
  for (auto row{0}; row < m_levels[m_level_index].get_height(); ++row) {
    for (auto column{0}; column < m_levels[m_level_index].get_width();
         ++column) {
      auto dir = m_snake.get_direction();
      auto value = m_snake.consult(row, column);
      switch (value) {
      case Level::position_e::BODY:
        if (alived) {
          std::cout << Color::tcolor("=", Color::BRIGHT_GREEN);
        } else {
          std::cout << Color::tcolor("*", Color::BRIGHT_RED);
        }
        break;
      case Level::position_e::FOOD:
        std::cout << Color::tcolor("@", Color::BRIGHT_MAGENTA);
        break;
      case Level::position_e::FREE:
        std::cout << " ";
        break;
      case Level::position_e::HEAD:
        if (not alived) {
          std::cout << Color::tcolor("%", Color::BRIGHT_RED);
        } else if (dir == Snake::direction_e::UP) {
          std::cout << Color::tcolor("^", Color::BRIGHT_GREEN);
        } else if (dir == Snake::direction_e::DOWN) {
          std::cout << Color::tcolor("v", Color::BRIGHT_GREEN);
        } else if (dir == Snake::direction_e::LEFT) {
          std::cout << Color::tcolor("<", Color::BRIGHT_GREEN);
        } else {
          std::cout << Color::tcolor(">", Color::BRIGHT_GREEN);
        }
        break;
      case Level::position_e::WALL:
        std::cout << Color::tcolor("\u2588", Color::BRIGHT_BLUE);
        break;
      case Level::position_e::INVISIBLE:
        std::cout << " ";
        break;
      default:
        std::cout << " ";
        break;
      }
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

bool SnazeSimulation::bfs() {
  std::queue<SnazeSimulation::Position> q;
  auto m_map_height = m_levels[m_level_index].get_height();
  auto m_map_width = m_levels[m_level_index].get_width();
  Visited v(m_map_height, std::vector<bool>(m_map_width, false));
  Parents p(m_map_height, std::vector<SnazeSimulation::Position>(m_map_width));

  m_head = {m_snake.get_head_coord().y, m_snake.get_head_coord().x};

  p[m_head.first][m_head.second] = {m_head.first, m_head.second};
  q.push({m_head.first, m_head.second});
  v[m_head.first][m_head.second] = true;

  while (!q.empty()) {
    Position Node = q.front();
    q.pop();
    for (short i = -1; i <= 1; i++) {
      for (short j = -1; j <= 1; j++) {
        if (invalid_bfs({Node.first, Node.second}, i, j, v) or
            aux_bfs({Node.first + i, Node.second + j},
                    {Node.first, Node.second}, p))
          continue;
        v[Node.first + i][Node.second + j] = true;
        p[Node.first + i][Node.second + j] = {Node.first, Node.second};
        q.push({Node.first + i, Node.second + j});
      }
    }
  }

  Position m_food_spawn = {m_snake.get_food_pos().y, m_snake.get_food_pos().x};
  if (v[m_food_spawn.first][m_food_spawn.second]) {
    Position aux = m_food_spawn;
    m_moves.clear();

    while (aux != p[aux.first][aux.second]) {
      m_moves.push_front(aux);
      aux = p[aux.first][aux.second];
    }
    return true;
  } else {
    return false;
  }
}

bool SnazeSimulation::aux_bfs(SnazeSimulation::Position u,
                              SnazeSimulation::Position v, const Parents &p) {
  m_tail = m_snake.get_tail();
  if (v == m_head and not m_tail.empty() and u == m_tail.back())
    return true;
  if (m_tail.size() <= 1)
    return false;
  short cnt = 0;
  Position aux = v;

  while (aux != p[aux.first][aux.second]) {
    cnt++;
    aux = p[aux.first][aux.second];
  }
  cnt++;

  if (cnt < m_tail.size()) {
    auto it = m_tail.end();
    --it;
    --it;
    cnt = m_tail.size() - cnt;
    while (cnt) {
      if (u == *it)
        return true;
      --it;
      cnt--;
    }
    return false;
  } else
    return false;
}

bool SnazeSimulation::invalid_bfs(SnazeSimulation::Position p, short x, short y,
                                  const Visited &v) {
  return (x == -1 and y == -1) or (x == -1 and y == 1) or
         (x == 1 and y == -1) or (x == 1 and y == 1) or ((p.first + x) < 0) or
         ((p.first + x) >= m_levels[m_level_index].get_height()) or
         ((p.second + y) < 0) or
         ((p.second + y) >= m_levels[m_level_index].get_width()) or
         v[p.first + x][p.second + y] or
         m_levels[m_level_index].consult(p.first + x, p.second + y) == '#';
}

void SnazeSimulation::random(SnazeSimulation::Position p, Visited &v) {
  m_tail = m_snake.get_tail();

  Position up = {p.first - 1, p.second};
  Position down = {p.first + 1, p.second};
  Position left = {p.first, p.second - 1};
  Position right = {p.first, p.second + 1};
  if (m_levels[m_level_index].consult(up.first, up.second) != '#' and
      (m_tail.empty() or up != m_tail.back()) and !v[up.first][up.second]) {
    v[up.first][up.second] = true;
    m_moves.push_back(up);
    random(up, v);
  } else if (m_levels[m_level_index].consult(down.first, down.second) != '#' and
             (m_tail.empty() or down != m_tail.back()) and
             !v[down.first][down.second]) {
    v[down.first][down.second] = true;
    m_moves.push_back(down);
    random(down, v);
  } else if (m_levels[m_level_index].consult(left.first, left.second) != '#' and
             (m_tail.empty() or left != m_tail.back()) and
             !v[left.first][left.second]) {
    v[left.first][left.second] = true;
    m_moves.push_back(left);
    random(left, v);
  } else if (m_levels[m_level_index].consult(right.first, right.second) !=
                 '#' and
             (m_tail.empty() or right != m_tail.back()) and
             !v[right.first][right.second]) {
    v[right.first][right.second] = true;
    m_moves.push_back(right);
    random(right, v);
  } else {
    m_moves.push_back(up);
    return;
  }
}
