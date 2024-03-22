#include "CLI.h"
#include "text_color.h"

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

std::optional<int> convert_to_int(string input) {
  std::optional<int> auxiliary;
  try {
    auxiliary = std::stoi(input);
  } catch (std::exception &err) {
    string message = "Integer was not provided.\n";
    std::cout << Color::tcolor(message, Color::RED);
  }
  return auxiliary;
}

bool CLI::read_command_line(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage();
    return false;
  }

  m_program_name = argv[0];
  m_input_file = argv[argc - 1];
  bool helped = false;
  for (int index{1}; index < argc - 1; ++index) {
    if (strcmp(argv[index], "--fps") == 0) {
      m_animation_speed = convert_to_int(argv[index + 1]);
      if (m_animation_speed)
        ++index;
    } else if (strcmp(argv[index], "--lives") == 0) {
      m_number_lives = convert_to_int(argv[index + 1]);
      if (m_number_lives)
        ++index;
    } else if (strcmp(argv[index], "--food") == 0) {
      m_number_food = convert_to_int(argv[index + 1]);
      if (m_number_food)
        ++index;
    } else if (not helped) {
      helped = true;
      print_usage();
    }
  }
  return true;
}

void CLI::print_usage() {
  std::string message =
      "Usage: " + m_program_name + "[<options>] <input_level_file>\n" +
      "--help           Print this help message.\n" +
      "--fps <num>      Number of frames presented per second. Default = 10\n" +
      "--lives <num>    Number of lives the snake shall have. Default = 5.\n" +
      "--food <num>     Number of food pellets for the entire simulation. "
      "Default = 10.\n";
  std::cerr << Color::tcolor(message, Color::YELLOW);
}
