#include "CLI.h"
#include "text_color.h"

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

bool CLI::read_command_line(int argc, char *argv[]) {
  m_animation_speed = 6;
  m_max_n_bars = 5;
  m_program_name = argv[0];
  if (argc < 2) {
    std::string message = "Path to input file was not provided.\n";
    std::cerr << Color::tcolor(message, Color::RED);
    print_usage();
    return false;
  }

  m_input_file = argv[argc - 1];
  bool helped = false;
  for (int index{1}; index < argc - 1; index++) {
    if (strcmp(argv[index], "-f") == 0) {
      index++;
      int tmp;
      try {
        tmp = std::stoi(argv[index]);
      } catch (std::exception &err) {
        std::string message = "No integer was provided for -f argument.\n";
        std::cerr << Color::tcolor(message, Color::RED);
      }
      if (tmp > 0 and tmp < 25) {
        m_animation_speed = tmp;
      } else {
        std::string message =
            "Invalid animation speed: " + std::to_string(tmp) +
            "\nValue must be in the range [1,24]\n";
        std::cerr << Color::tcolor(message, Color::RED);
        index--;
      }
    } else if (strcmp(argv[index], "-b") == 0) {
      index++;
      int tmp;
      try {
        tmp = std::stoi(argv[index]);
      } catch (std::exception &err) {
        std::string message = "No integer was provided for -b argument.\n";
        std::cerr << Color::tcolor(message, Color::RED);
      }
      if (tmp > 0 and tmp < 16) {
        m_max_n_bars = tmp;
      } else {
        std::string message = "Invalid number of bars: " + std::to_string(tmp) +
                              "\nValid must be in the range [1,15]\n";
        std::cerr << Color::tcolor(message, Color::RED);
        index--;
      }
    } else if (not helped) {
      helped = true;
    }
  }
  if (helped) {
    print_usage();
  }
  return true;
}

void CLI::print_usage() {
  std::string message =
      "Usage: " + m_program_name + " [<options>] <input_data_file>\n" + '\t' +
      "-b <num> Max number of bars in a single barchart.\n" + "\t\t" +
      "Valid range is [1,15]. Default value is 5.\n" + "\t" +
      "-f <num> Animation speed in fps (frames per second).\n" + "\t\t" +
      "Valid range is [1,24]. Default value is 6.\n" +
      "Note: a valid input file is required.\n" +
      "      <options> are optional commands.\n";
  std::cerr << Color::tcolor(message, Color::YELLOW);
}
