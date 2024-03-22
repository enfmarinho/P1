#ifndef __DATA_BASE__
#define __DATA_BASE__

#include "libs/tokenizer.h"
#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <vector>

class DataBase {
public:
  //<! Information of each bar;
  struct BarItem {
    std::string title; //!< Title of the barchart.
    std::string name;  //!< Name of each bar.
    size_t value;      //!< Value of each bar.
    std::string label; //!< Label of each bar.
  };

private:
  std::string m_main_title;   //!< Main title of the barchart.
  std::string m_source;       //!< Source of information.
  std::string m_type_of_data; //!< The type of data that the bar chart is about.
  std::vector<std::vector<BarItem>> m_data; //!< Maintains the data of file
  size_t m_index_data;                //!< Index to current BarItem in data.
  std::set<std::string> m_all_labels; //!< Contains all labels.
  std::set<std::string>::iterator m_index_label; //!< Index to current label.

public:
  /// Default constructor.
  DataBase() = default;
  /// Default destructor.
  ~DataBase() = default;
  /*!
   * Reads data from input file.
   * \param file path to input file.
   * \return flag that indicates whether the file reading was successful.
   */
  bool read_file(const std::string &file);

  /// Consults main title.
  [[nodiscard]] std::string get_main_title() const { return m_main_title; }

  /// Consults source of information.
  [[nodiscard]] std::string get_source() const { return m_source; }

  /// Consults type of data.
  [[nodiscard]] std::string get_data_type() const { return m_type_of_data; }

  /// Consults number of labels.
  [[nodiscard]] int get_n_labels() const { return m_all_labels.size(); }

  /// Returns the label set
  [[nodiscard]] std::set<std::string> get_labels() const;

  /// Consults current vector of BarItem
  std::vector<BarItem> get_bar_items() const { return m_data[m_index_data]; }

  /// Consults whether the data list has ended.
  [[nodiscard]] bool data_ended() { return m_index_data == m_data.size(); }

  /// Go to next BarItem in the data.
  void next_data() { m_index_data++; }
};

#endif // __DATA_BASE__