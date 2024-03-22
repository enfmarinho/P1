#ifndef CLI_H
#define CLI_H

#include <optional>
#include "animal.h"
#include "billboard.h"

/// This class keeps track of all the running option, which may be modified via CLI.
class CLI {
  std::optional<Animal::mood_t> m_style;  //!< Define animal style.
  std::optional<std::string> m_eyes;      //!< Contains the appearance of the animal's eyes.
  std::optional<std::string> m_tongue;    //!< Contains the appearance of the animal's tongue.
  std::optional<int> m_wrap_column;       //!< Specifies where the message should be wrapped.
  std::optional<Billboard::align_t> m_alignment;  //!< Specifies how the message should be aligned.
  bool m_flip;      //!< Flag that indicates whether the user wants to flip the animal.
  bool m_trimming;  //!< Flag that indicates whether the user wants to trim the message.
  bool m_helped;    //!< Flag that indicates whether the help message was print.

public:
  /// Initialize values to default.
  CLI();
  /// Destructor.
  ~CLI() = default;

  /*!
   * Print help message.
   * @param program_name name of the program.
   */
  static void print_usage(const std::string& program_name);

  /*!
   * Reads command line arguments and set them to respective variables.
   * @param argv string with command line arguments.
   * @param argc size of argv.
   */
  void read_command_line(int argc, char* argv[]);

  /// Consult style.
  [[nodiscard]] std::optional<Animal::mood_t> get_style() const { return m_style; }
  /// Consult flag whether to flip animal or not.
  [[nodiscard]] bool get_flip() const { return m_flip; };
  /// Consult flag whether to preserve message or not.
  [[nodiscard]] bool get_trimming() const { return m_trimming; }
  /// Consult eyes.
  [[nodiscard]] std::optional<std::string> get_eyes() const { return m_eyes; }
  /// Consult tongue
  [[nodiscard]] std::optional<std::string> get_tongue() const { return m_tongue; }
  /// Consult wrap colum.
  [[nodiscard]] int get_wrap_colum() const { return m_wrap_column.value_or(40); }
  /// Consult alignment.
  [[nodiscard]] Billboard::align_t get_alignment() const {
    return m_alignment.value_or(Billboard::align_t::LEFT);
  }
};

#endif  // CLI_H
