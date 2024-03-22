#include "tokenizer.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

Tokenizer::Tokenizer(const std::string &input, const std::string &delimiters,
                     const std::string &limiters, bool trimming) {
  m_index = 0;
  for (int i{0}; i < delimiters.size(); i++) {
    m_delimiters.push_back(delimiters[i]);
  }
  for (int i{0}; i < limiters.size(); i++) {
    m_limiters.push_back(limiters[i]);
  }
  m_trimming = trimming;
  process_input(input);
}

void Tokenizer::process_input(const std::string &input) {
  bool operation = true;
  std::string tmp; // Temporary string to represent tokens.
  for (char current : input) {
    if (std::find(m_delimiters.begin(), m_delimiters.end(), current) !=
        m_delimiters.end()) {
      if (not tmp.empty()) {
        m_tokens.push_back(tmp);
        tmp.clear();
        operation = false;
      }
    } else if (current == '-') {
      if (operation) {
        tmp.push_back(current);
        operation = false;
      } else {
        if (not tmp.empty()) {
          m_tokens.push_back(tmp);
          tmp.clear();
        }
        m_tokens.push_back("-");
        operation = true;
      }
    } else if (std::find(m_limiters.begin(), m_limiters.end(), current) !=
               m_limiters.end()) {
      if (not tmp.empty()) {
        m_tokens.push_back(tmp);
        tmp.clear();
      }
      tmp = current;
      m_tokens.push_back(tmp);
      tmp.clear();
      operation = true;
    } else {
      tmp.push_back(current);
      operation = false;
    }
  }

  if (not tmp.empty()) {
    m_tokens.push_back(tmp);
  }
}

void Tokenizer::print_all_tokens() {
  beginning();
  std::cout << "tokens: \n";
  for (; not ended(); next()) {
    std::cout << "t: " << get_token() << "\n";
  }
  std::cout << "Fim\n";
  beginning();
}

bool Tokenizer::in_range() {
  if (m_index >= 0 and m_index < m_tokens.size()) {
    return true;
  } else {
    return false;
  }
}

std::string Tokenizer::get_token() {
  if (in_range()) {
    return m_tokens[m_index];
  } else {
    return "";
  }
}
