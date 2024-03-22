/*!
 * Bar Char Race project.
 *
 * @author Selan
 * @data October 5th, 2021.
 */

#include "barchart.h"
#include "database.h"
#include "libs/text_color.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace bcra {
bool cmp_func(const DataBase::BarItem &a, const DataBase::BarItem &b) {
  return a.value > b.value;
}

value_t BarChart::calculate_bar_size(value_t value) {
  return (value * m_max_bar_size / m_item[0].value);
}

value_t BarChart::to_max() {
  value_t temp = m_item[0].value + 1;
  value_t mod = 10;

  while (temp % mod)
    temp++;

  return temp;
}

value_t BarChart::to_min(value_t min) {
  size_t d = 0;
  value_t sig;

  while (min) {
    sig = min % 10;
    min /= 10;
    if (min)
      d++;
  }

  for (size_t i = 0; i < d; i++)
    sig *= 10;

  return sig;
}

std::vector<value_t> get_digits(value_t value) {
  std::vector<value_t> digits;
  while (value != 0) {
    digits.push_back(value % 10);
    value /= 10;
  }

  std::reverse(digits.begin(), digits.end());
  return digits;
}

void BarChart::insert(std::string &line, const std::vector<value_t> &digits,
                      value_t index) {
  for (auto d : digits) {
    line[index++] = d + '0';
    std::cout << line[index];
  }
}

BarChart::BarChart(std::vector<DataBase::BarItem> item, std::string source,
                   std::string title, std::string type_of_data,
                   value_t bars_number, value_t max_bar_size,
                   std::map<std::string, short> color_map) {
  m_item = item;
  m_source = source;
  m_title = title;
  m_type_of_data = type_of_data;
  m_bars_number = bars_number;
  m_max_bar_size = max_bar_size;
  m_color_map = color_map;
}

void BarChart::show() {
  title();
  bars();
  x_axis();
  legend();
  source();
  color_legend();
  std::cout << m_output.str();
}

void BarChart::title() {
  m_output << Color::tcolor("\n\n\t" + m_title, Color::MAGENTA) << "\n\n";
  std::string time_stamp = "\t\tTime stamp: " + m_item.begin()->title;
  m_output << Color::tcolor(time_stamp, Color::MAGENTA) << "\n\n";
}

void BarChart::bars() {
  std::sort(m_item.begin(), m_item.end(), cmp_func);
  if (m_bars_number > m_item.size()) {
    m_bars_number = m_item.size();
  }

  double greatest_value = m_item[0].value;
  for (value_t index{0}; index < m_bars_number; index++) {
    std::string bar = "";
    value_t size = calculate_bar_size(m_item[index].value);
    for (int i{0}; i < size; i++) {
      bar += "\u2588";
    }

    short color = m_color_map[m_item[index].label];

    m_output << Color::tcolor(bar, color) << ' '
             << Color::tcolor(m_item[index].name, color) << " ["
             << m_item[index].value << "]\n\n";
  }
}

void BarChart::legend() {
  m_output << Color::tcolor("\n" + m_type_of_data, Color::YELLOW);
}

void BarChart::source() {
  m_output << Color::tcolor("\n\n" + m_source, Color::BRIGHT_WHITE);
}

void BarChart::x_axis() {
  std::string axis = "<";
  std::string numbers;
  for (value_t counter{0}; counter < m_max_bar_size * 2; counter++) {
    axis += '-';
    numbers += " ";
  }
  axis += ">\n";

  value_t greatest_value = to_max();
  value_t smallest_value = to_min(m_item[m_bars_number - 1].value);

  double increment = (greatest_value - smallest_value) / 5.0;

  for (value_t value = smallest_value; value <= greatest_value;
       value += increment) {
    value_t index = calculate_bar_size(value);
    axis[index] = '+';
    std::vector<value_t> digits = get_digits(value);
    insert(numbers, digits, index);
  }

  m_output << Color::tcolor(axis, Color::BRIGHT_WHITE)
           << Color::tcolor(numbers, Color::BRIGHT_WHITE) << '\n';
}

void BarChart::color_legend() {
  if (m_color_map.size() < 17) {
    m_output << '\n';
    value_t line_size = 0;
    for (auto it = m_color_map.begin(); it != m_color_map.end(); it++) {
      std::string bar = " ";
      std::string label = it->first;
      bar = Color::tcolor(bar, it->second);
      label = Color::tcolor(label, it->second);

      std::string tmp;
      if (it != m_color_map.begin()) {
        tmp = bar + ": " + label + ' ';
      } else {
        tmp = label;
      }

      if (line_size + tmp.size() > 60) {
        line_size = 0;
        m_output << '\n';
      }
      m_output << tmp;
    }
    m_output << '\n';
  }
}
} // namespace bcra
