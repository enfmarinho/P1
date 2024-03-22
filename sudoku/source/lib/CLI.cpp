#include "CLI.h"
#include "text_color.h"

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

void CLI::read_command_line(int argc, char* argv[]) {
  if (argc < 2) {
    print_usage();
  } else {
    m_program_name = argv[0];
    m_input_file = argv[argc - 1];
    bool helped = false;
    for (size_t index{ 0 }; index < argc; index++) {
      if (std::strcmp(argv[index], "-c") == 0) {
        index++;
        int tmp;
        try {
          tmp = std::stoi(argv[index]);
        } catch (std::exception& err) {
          std::string message = "No integer was provided for -c argument.\n";
          std::cerr << Color::tcolor(message, Color::RED);
          index--;
        }
        m_checks_number = tmp;
      } else if (std::strcmp(argv[index], "-h") == 0 and not helped) {
        print_usage();
        helped = true;
      }
    }
  }
}

void CLI::print_usage() {
  std::string message
    = "Usage: " + m_program_name + "[-c <num>] [-h] <input_file>\n" + "\tGame options:\n"
      + "\t\t-c <num> Number of checks per game. Default = 3.\n"
      + "\t\t-h       Print this help message.\n"
      + "Note: if no argument is provided the input file used will be data/input.txt by "
        "default.\n      But if some argument is provided the input file must be explicitly "
        "provided.\n";
  std::cerr << Color::tcolor(message, Color::YELLOW);
}
