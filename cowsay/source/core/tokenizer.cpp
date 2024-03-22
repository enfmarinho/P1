#include "tokenizer.h"
#include <set>
#include <string>
#include <vector>

Tokenizer::Tokenizer(const std::string& input,
                     const std::string& delimiters,
                     const std::string& limiters,
                     bool trimming,
                     int size_limit) {
  m_index = 0;
  m_size_limit = size_limit;
  for (int i{ 0 }; i < delimiters.size(); i++) {
    m_delimiters.insert(delimiters[i]);
  }
  for (int i{ 0 }; i < limiters.size(); i++) {
    m_limiters.insert(limiters[i]);
  }
  m_trimming = trimming;
  process_input(input);
}

void Tokenizer::process_input(const std::string& input) {
  std::string tmp;             // Temporary string to represent tokens.
  bool limiter_found = false;  // Flag that indicates whether the last char is a limiter or not.
  for (char current : input) {
    if (tmp.size() == m_size_limit) {
      limiter_found = false;
      m_tokens.push_back(tmp);
      tmp.clear();
    }
    if (current == '\n') {
      limiter_found = false;
      m_tokens.push_back(tmp);
      tmp.clear();
      m_tokens.push_back("\n");
    } else if (m_delimiters.count(current) != 0) {
      if (not tmp.empty()) {
        limiter_found = false;
        m_tokens.push_back(tmp);
        tmp.clear();
      }
      if (not m_trimming) {
        std::string tmp(1, current);
        m_tokens.push_back(tmp);
      }
    } else if (m_limiters.count(current) != 0) {
      limiter_found = true;
      tmp.push_back(current);
    } else if (limiter_found) {
      limiter_found = false;
      m_tokens.push_back(tmp);
      tmp.clear();
      tmp.push_back(current);
    } else {
      tmp.push_back(current);
    }
  }
  if (m_tokens.back() == "\n")
    m_tokens.pop_back();
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
