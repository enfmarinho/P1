#ifndef CLI_H
#define CLI_H
#include <optional>
#include <string>

/*
 * This class keeps records of all command line arguments,
 * contaning methods to read them.
 */
class CLI {
  std::string m_program_name; //!< Name of the executable.
  std::string m_input_file;   //!< Path to input data file.
  std::optional<int>
      m_animation_speed;           //!< Animation speed required by the user.
  std::optional<int> m_max_n_bars; //!< number of bars required by the user.
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
  bool read_command_line(int argc, char *argv[]);

  /// Consult path to input file.
  [[nodiscard]] std::string get_input_file() const { return m_input_file; }
  /*!
   * Consults animation speed.
   * \return value that determines animation speed(fps) or -1 if it was not set.
   */
  [[nodiscard]] int get_animation_speed() const {
    return m_animation_speed.value_or(-1);
  }
  /*!
   * Consults maximum number of bars.
   * \return value that determines number of bars or -1 if it was not set.
   */
  [[nodiscard]] int get_max_n_bars() const { return m_max_n_bars.value_or(-1); }
};

#endif // CLI_H
