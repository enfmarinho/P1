#include <string>
using std::string;

#include "animal.h"

//== THESE ARE AUXILIARY FUNCTIONS.

//== THESE ARE THE METHODS.
Animal::Animal(std::optional<mood_t> style,
               bool flip,
               std::optional<string> eyes,
               std::optional<string> tongue) {
  init_templates();
  if (style) {
    m_style = style.value();
    m_eyes = m_styles_templates[m_style].eyes;
    m_tongue = m_styles_templates[m_style].tongue;
  } else {
    m_style = mood_t::NEUTRAL;
    m_eyes = m_styles_templates[mood_t::NEUTRAL].eyes;
    m_tongue = m_styles_templates[mood_t::NEUTRAL].tongue;
  }

  if (eyes and not style)
    m_eyes = eyes.value();

  if (tongue and not style)
    m_tongue = tongue.value();

  m_flip = flip;

  if (not m_flip) {
    m_body = std::string{ "       \\   ^__^            \n" }
             + std::string{ "        \\  (" + m_eyes + ")\\_______\n" }
             + std::string{ "           (__)\\       )\\/\\     \n" }
             + std::string{ "            " + m_tongue + " ||----w |      \n" }
             + std::string{ "               ||     ||           \n" };
  } else {
    m_body = std::string{ "             ^__^   /  \n" }
             + std::string{ "     _______/(" + m_eyes + ")  /\n" }
             + std::string{ " /\\/(       /(__)             \n" }
             + std::string{ "    | w---- | " + m_tongue + "          \n" }
             + std::string{ "    ||     ||                  \n" };
  }
}

void Animal::init_templates() {
  m_styles_templates[mood_t::NEUTRAL] = { "oo", "  " };
  m_styles_templates[mood_t::BORG] = { "==", "  " };
  m_styles_templates[mood_t::DEAD] = { "xx", " U" };
  m_styles_templates[mood_t::GREEDY] = { "$$", "  " };
  m_styles_templates[mood_t::PARANOIA] = { "@@", "  " };
  m_styles_templates[mood_t::STONED] = { "**", " U" };
  m_styles_templates[mood_t::TIRED] = { "--", "  " };
  m_styles_templates[mood_t::WIRED] = { "OO", "  " };
  m_styles_templates[mood_t::YOUTH] = { "..", "  " };
}

int Animal::get_width() {
  if (m_flip)
    return 20;
  else
    return 0;
}

// ================================[ cowsay.cpp ]=====================================
