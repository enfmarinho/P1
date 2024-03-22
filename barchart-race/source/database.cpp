#include "database.h"
#include "libs/text_color.h"
#include "libs/tokenizer.h"
#include <cstdio>
#include <iostream>
#include <string>

bool DataBase::read_file(const std::string &file) {
  std::ifstream iptf(file);

  // Check for errors.
  if (not iptf.good()) {
    std::string message = "The file could not be read: " + file;
    std::cerr << Color::tcolor(message, Color::RED) << '\n';
    return false;
  }

  getline(iptf, m_main_title);
  getline(iptf, m_type_of_data);
  getline(iptf, m_source);

  int bars{0};
  std::vector<BarItem> aux;

  while (iptf >> bars) {
    iptf.get();
    m_data.push_back(aux);
    std::string delimiters = "\n,";
    Tokenizer tokenizer(delimiters);
    for (int idx{0}; idx < bars; idx++) {
      std::string line;
      getline(iptf, line);
      tokenizer.process_input(line);

      BarItem tmp;
      while (not tokenizer.ended()) {
        if (tokenizer.get_index() == 0) {
          tmp.title = tokenizer.get_token();
        } else if (tokenizer.get_index() == 1) {
          tmp.name = tokenizer.get_token();
        } else if (tokenizer.get_index() == 3) {
          tmp.value = std::stoi(tokenizer.get_token());
        } else if (tokenizer.get_index() == 4) {
          tmp.label = tokenizer.get_token();
        }
        tokenizer.next();
      }

      m_data.back().push_back(tmp);
      m_all_labels.insert(tmp.label);
    }
  }

  iptf.close();
  return true;
}

std::set<std::string> DataBase::get_labels() const { return m_all_labels; }
