#include "barchart.h"
#include "libs/CLI.h"
#include "libs/text_color.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <string>
#include <thread>
using std::string;
#include <sstream>
using std::istringstream;

#include "bcr_am.h"

namespace bcra {
// Instantiation of the global configuration variable.
const Cfg global_cfg;

auto str_lowercase = [](const char *t) -> std::string {
  std::string str{t};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

void welcome_message() {
  std::cerr << "\n===============================================\n";
  std::cerr << "  Welcome to the Bar Chart Race, v1.0\n";
  std::cerr << "  Copyright (C) 2020, Selan R. dos Santos\n";
  std::cerr << "===============================================\n\n";
}

void read_enter() {
  // Read input until receive a '\n'.
  char input;
  do {
    input = getchar();
  } while (input != '\n');
}

BCRAnimation::BCRAnimation() {
  m_opt.input_filename = "";
  m_ended_successfully = false;
}

/// Initializes the animation engine.
void BCRAnimation::initialize(int argc, char *argv[]) {
  if (not m_arguments.read_command_line(argc, argv)) {
    m_animation_state = ani_state_e::END;
  } else {
    m_opt.input_filename = m_arguments.get_input_file();
    m_opt.fps = m_arguments.get_animation_speed();
    m_opt.n_bars = m_arguments.get_max_n_bars();
    m_animation_state = ani_state_e::START;
  }
}

bool BCRAnimation::is_over() const {
  return m_animation_state == ani_state_e::END;
}

void BCRAnimation::process_events() {
  if (m_animation_state == ani_state_e::WELCOME) {
    std::string message = ">>> Preparing to read input file \"" +
                          m_opt.input_filename + "\"...\n\n";
    std::cerr << Color::tcolor(message, Color::GREEN);
    if (not m_data.read_file(m_opt.input_filename)) {
      m_animation_state = ani_state_e::END;
    }
    message = ">>> Input file was successfuly read.\n\n";
    std::cerr << Color::tcolor(message, Color::GREEN);
  } else if (m_animation_state == ani_state_e::READING_INPUT) {
    read_enter();
  }
}

void BCRAnimation::update() {
  if (m_animation_state == ani_state_e::START) {
    m_animation_state = ani_state_e::WELCOME;
  } else if (m_animation_state == ani_state_e::WELCOME) {
    m_animation_state = ani_state_e::READING_INPUT;
  } else if (m_animation_state == ani_state_e::READING_INPUT) {
    m_animation_state = ani_state_e::RACING;
  } else if (m_animation_state == ani_state_e::RACING and m_data.data_ended()) {
    m_animation_state = ani_state_e::END;
    m_ended_successfully = true;
  }
}

void BCRAnimation::render() {
  if (m_animation_state == ani_state_e::WELCOME) {
    welcome_message();
  } else if (m_animation_state == ani_state_e::READING_INPUT) {
    std::string animation_speed = std::to_string(m_opt.fps);
    std::string n_categories = std::to_string(m_data.get_n_labels());
    std::string message = ">>> Animation speed is: " + animation_speed + '\n' +
                          ">>> Title: " + m_data.get_main_title() + '\n' +
                          ">>> Value is: " + m_data.get_data_type() + '\n' +
                          ">>> " + m_data.get_source() + '\n' +
                          ">>> # of categories found: " + n_categories + '\n' +
                          ">>> Press enter to begin animation.";
    std::cerr << Color::tcolor(message, Color::GREEN);
    // Set color map.
    std::set<std::string> labels = m_data.get_labels();
    if (m_data.get_n_labels() > Color::color_list.size()) {
      for (auto it = labels.begin(); it != labels.end(); it++) {
        color_map.insert({*it, Color::WHITE});
      }
    } else {
      auto it = labels.begin();
      for (value_t index{0};
           index < m_data.get_n_labels() && it != labels.end(); index++, it++) {
        color_map.insert({*it, Color::color_list[index]});
      }
    }
  } else if (m_animation_state == ani_state_e::RACING) {
    BarChart output(m_data.get_bar_items(), m_data.get_source(),
                    m_data.get_main_title(), m_data.get_data_type(),
                    m_opt.n_bars, 45, color_map);
    output.show();
    m_data.next_data();
    std::chrono::milliseconds duration{1000 / m_opt.fps};
    std::this_thread::sleep_for(duration);
  } else if (m_animation_state == ani_state_e::END and m_ended_successfully) {
    std::string end_message = "\nHope you have enjoyed the Bar Chart race!\n";
    std::cerr << Color::tcolor(end_message, Color::MAGENTA);
  }
}

}; // namespace bcra