#ifndef CLI_H
#define CLI_H
#include <optional>
#include <string>

/*
 * This class keeps records of all command line arguments,
 * containing methods to read them.
 */
class CLI {
  std::string m_program_name; //!< Name of the executable.
  std::string m_input_file;   //!< Path to input data file.
  std::optional<int>
      m_animation_speed;             //!< Animation speed required by the user.
  std::optional<int> m_number_lives; //!< Number of lives for the simulation.
  std::optional<int> m_number_food;  //!< Number of foods for the simulation.

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
   * \return value that determines animation speed(fps).
   */
  [[nodiscard]] int get_animation_speed() const {
    return m_animation_speed.value_or(10);
  }
  /*!
   * Consults number of food.
   * \return number of lives.
   */
  [[nodiscard]] int get_number_food() const {
    return m_number_food.value_or(10);
  }
  /*!
   * Consults number of lives for animation.
   * \return number of lives.
   */
  [[nodiscard]] int get_number_lives() const {
    return m_number_lives.value_or(5);
  }
};

#endif // CLI_H
