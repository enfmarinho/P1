#ifndef BARCHART_H
#define BARCHART_H

/*!
 * Bar Char Race project.
 *
 * @author Selan
 * @data October 5th, 2020.
 */

#include "database.h"
#include "libs/text_color.h"

#include <chrono>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace bcra {
// The value type of a data item.
using value_t = long;

/// This class represents a single Bar Chart.
class BarChart {
  std::vector<DataBase::BarItem> m_item; //!< Data to output.
  ostringstream m_output;                //!< Output object.
  std::string m_title;                   //!< Title of the barchart.
  std::string m_source;                  //!< Source of the barchart data.
  std::string m_type_of_data;            //!< Description of barchart data.
  value_t m_bars_number;                 //!< Number of bars to show.
  value_t m_max_bar_size;                //!< Size of greatest bar on barchart.
  std::map<std::string, short> m_color_map; //!< Map of colors for each label.

  /*!
   * Insertes a number digit by digit on a string
   * \param line string to be changed.
   * \param digits digits to be inserted.
   * \param index position to start inserting.
   */
  void insert(std::string &line, const std::vector<value_t> &digits,
              value_t index);
  /// Round the maximum bar value.
  value_t to_max();
  /// Round the minimum bar value.
  value_t to_min(value_t min);
  /*!
   * Calculates the bar size relative to the max size.
   * \param value value of the bar.
   * \return size of the bar.
   */
  value_t calculate_bar_size(value_t value);
  /// Set title to output object.
  void title();
  /// Set bars to output object.
  void bars();
  /// Set type of data to output object.
  void legend();
  /// Set source to output object.
  void source();
  /// Set x-axis to output object.
  void x_axis();
  /// Set color legend to output object.
  void color_legend();

public:
  /*!
   * Initialize value of data members.
   * \param item data to output.
   * \param source source of the barchart data.
   * \param title title of the barchart.
   * \param type_of_data description of barchart data.
   * \param bars_number number of bars to show.
   * \param max_bar_size size of greatest bar on barchar.
   * \param color_map map of colors for each label.
   */
  BarChart(std::vector<DataBase::BarItem> item, std::string source,
           std::string title, std::string type_of_data, value_t bars_number,
           value_t max_bar_size, std::map<string, short> color_map);
  /// Default destructor.
  ~BarChart() = default;
  /// Output BarChart animation.
  void show();
};

} // namespace bcra.
#endif
