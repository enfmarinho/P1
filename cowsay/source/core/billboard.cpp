#include "billboard.h"
#include <iostream>
#include <sstream>
//
Billboard::Billboard(Tokenizer& data, int wrap_column, Billboard::align_t alignment, int padding) {
  m_data = &data;
  m_alignment = alignment;
  m_wrap_column = wrap_column;
  m_padding = padding;
}

void Billboard::print_n_times(int size, const char& c) const {
  for (int i{ 0 }; i < size; i++) {
    std::cout << c;
  }
}

void Billboard::print_line(std::ostringstream& line, int line_size) {
  int number_of_spaces = m_wrap_column - line_size;
  if (m_alignment == align_t::LEFT) {
    std::cout << line.str();
    print_n_times(number_of_spaces, ' ');
  } else if (m_alignment == align_t::RIGHT) {
    print_n_times(number_of_spaces, ' ');
    std::cout << line.str();
  } else if (m_alignment == align_t::CENTERED) {
    print_n_times(number_of_spaces / 2, ' ');
    std::cout << line.str();
    print_n_times(ceil(number_of_spaces / (float)2), ' ');
  }
}

void Billboard::print() {
  std::ostringstream line;
  bool space = true;  // Flag that indicates whether the last char was a space.
  int line_size = 0;
  int current_line = 1;
  while (not m_data->ended()) {
    std::string current_token = m_data->get_token();
    if (current_token[0] == '\n' or line_size + current_token.size() > m_wrap_column) {
      if (current_line == 1) {
        // Prints top of the billboard.
        std::cout << '\n';
        print_n_times(m_padding + 1, ' ');
        print_n_times(m_wrap_column + 2, '_');
        std::cout << '\n';

        print_n_times(m_padding, ' ');
        std::cout << "/ ";
        print_line(line, line_size);
        std::cout << " \\\n";
      } else {
        print_n_times(m_padding, ' ');
        std::cout << "| ";
        print_line(line, line_size);
        std::cout << " |\n";
      }
      line.str("");
      line_size = 0;
      current_line++;
      space = true;
      if (current_token[0] == '\n')
        m_data->next();
    } else if (not space and line_size != m_wrap_column) {
      line << ' ';
      line_size++;
      space = true;
    } else {
      line << current_token;
      line_size += current_token.size();
      space = isspace(current_token[0]);
      m_data->next();
    }
  }

  if (current_line == 1) {
    // Prints top of the billboard.
    std::cout << '\n';
    print_n_times(m_padding + 1, ' ');
    print_n_times(line_size + 2, '_');
    std::cout << '\n';

    print_n_times(m_padding, ' ');
    std::cout << "< " << line.str() << " >\n";

    // Prints bottom of the billboard.
    print_n_times(m_padding + 1, ' ');
    print_n_times(line_size + 2, '-');
    std::cout << '\n';
  } else {
    print_n_times(m_padding, ' ');
    std::cout << "\\ ";
    print_line(line, line_size);
    std::cout << " /\n";

    // Prints bottom of the billboard.
    print_n_times(m_padding + 1, ' ');
    print_n_times(m_wrap_column + 2, '-');
    std::cout << '\n';
  }
}
