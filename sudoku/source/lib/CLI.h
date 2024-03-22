#ifndef CLI_H
#define CLI_H

#include <cstddef>
#include <optional>
#include <string>
#include "text_color.h"

/*
 * This class keeps records of all command line arguments,
 * contaning methods to read them.
 */
class CLI {
  std::string m_program_name;
  std::optional<size_t> m_checks_number;
  std::optional<std::string> m_input_file;

public:
  /// Constructor
  CLI() = default;
  /// Destructor.
  ~CLI() = default;

  /// Print help message.
  void print_usage();

  /*!
   * Reads command line arguments and set them to respective variables.
   * \param argv string with command line arguments.
   * \param argc size of argv.
   * \return flag that indicates whether the execution can continue.
   */
  void read_command_line(int argc, char* argv[]);

  /// Consult number of checks.
  [[nodiscard]] size_t get_checks_number() { return m_checks_number.value_or(3); }

  /// Consult path to input file.
  [[nodiscard]] std::string get_input_file() { return m_input_file.value_or("data/input.txt"); }
};

#endif  // CLI_H
