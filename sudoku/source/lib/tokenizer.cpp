#include "tokenizer.h"
#include <set>
#include <string>
#include <vector>

Tokenizer::Tokenizer(const std::string& input, const std::string& delimiters) {
  m_index = 0;
  for (int i{ 0 }; i < delimiters.size(); i++) {
    m_delimiters.insert(delimiters[i]);
  }
  process_input(input);
}

void Tokenizer::process_input(const std::string& input) {
  std::string tmp;  // Temporary string to represent tokens.
  for (char current : input) {
    if (m_delimiters.count(current) != 0) {
      if (not tmp.empty()) {
        m_tokens.push_back(tmp);
        tmp.clear();
      }
    } else {
      tmp.push_back(current);
    }
  }
  if (not tmp.empty()) {
    m_tokens.push_back(tmp);
  }
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
