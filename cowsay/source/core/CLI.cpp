#include "CLI.h"
#include "animal.h"
#include "billboard.h"

#include <iostream>
#include <optional>

CLI::CLI() {
  m_flip = false;
  m_trimming = true;
  m_helped = false;
}

void CLI::read_command_line(int argc, char** argv) {
  for (int index{ 1 }; index < argc; index++) {
    if (strcmp(argv[index], "-b") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::BORG;
    } else if (strcmp(argv[index], "-d") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::DEAD;
    } else if (strcmp(argv[index], "-g") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::GREEDY;
    } else if (strcmp(argv[index], "-p") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::PARANOIA;
    } else if (strcmp(argv[index], "-s") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::STONED;
    } else if (strcmp(argv[index], "-t") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::TIRED;
    } else if (strcmp(argv[index], "-w") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::WIRED;
    } else if (strcmp(argv[index], "-y") == 0) {
      if (m_style.has_value())
        std::cerr << "Style already defined.\n";
      else
        m_style = Animal::mood_t::YOUTH;
    } else if (strcmp(argv[index], "-f") == 0) {
      m_flip = true;
    } else if (strcmp(argv[index], "-n") == 0) {
      m_trimming = false;
    } else if (strcmp(argv[index], "-h") == 0) {
      if (not m_helped) {
        m_helped = true;
        print_usage(argv[0]);
      }
    } else if (strcmp(argv[index], "-E") == 0) {
      if (m_eyes) {
        std::cerr << "Eyes are already defined.\n";
      } else if (index < argc - 1 and strlen(argv[index + 1]) > 1) {
        index++;
        std::string tmp = argv[index];
        tmp.resize(2);
        m_eyes = tmp;
      } else {
        std::cerr << "A two character string must be given.\n";
      }
    } else if (strcmp(argv[index], "-T") == 0) {
      if (m_tongue) {
        std::cerr << "Tongue are already defined.\n";
      } else if (index < argc - 1 and strlen(argv[index + 1]) > 1) {
        index++;
        std::string tmp = argv[index];
        tmp.resize(2);
        m_tongue = tmp;
      } else {
        std::cerr << "A two character string must be given.\n";
      }
    } else if (strcmp(argv[index], "-W") == 0) {
      if (index >= argc - 1) {
        std::cerr << "Missing wrap column value.\n";
      } else {
        int tmp = std::stoi(argv[index + 1]);
        if (tmp >= 0 and tmp <= 100) {
          index++;
          m_wrap_column = tmp;
        } else {
          std::cerr << "wrap_column value must be in range [1,100]. Assuming -W 40.\n";
        }
      }
    } else if (strcmp(argv[index], "-J") == 0) {
      if (index >= argc - 1) {
        std::cerr << "Missing alignment option." << '\n';
      } else {
        char align = argv[index + 1][0];
        switch (align) {
        case 'r':
          m_alignment = Billboard::align_t::RIGHT;
          index++;
          break;
        case 'c':
          m_alignment = Billboard::align_t::CENTERED;
          index++;
          break;
        default:
          std::cerr << "Unknown alignment option: " << align << '\n';
          break;
        }
      }
    } else {
      std::cerr << "Unknown option: " << argv[index] << '\n';
    }
  }
}

void CLI::print_usage(const std::string& program_name) {
  std::cerr << "Welcome to C++ cowsay clone, version 1.0, (c) 2023, DIMAp, UFRN.\n";
  std::cerr << "Usage:\n";
  std::cerr << '\t' << program_name
            << " [-E eyes] [-T tongue] [-W wrap_column] [-J alignment] [-h]"
               " [-f] [-n] [<style>]\n";
  std::cerr << "Where:\n";
  std::cerr << '\t' << "eyes is a two character string.\n";
  std::cerr << '\t' << "tongue is a two character string.\n";
  std::cerr << '\t' << "wrap_column is a number in the range [1,100].\n";
  std::cerr << '\t'
            << "alignment is either 'c' or 'r' representing centered and right respectively.\n";
  std::cerr << '\t'
            << "<style> can be -b, -d, -g, -p, -s, -t, -w or -y representing borg, dead, "
               "greedy, paranoia, stoned, tired, wired and youth respectively.\n";
  std::cerr << "Commands description:\n";
  std::cerr << '\t'
            << "-E: indicates that the following argument characterize the cow's eyes."
               " If used more than once, only the first use will be considered.\n";
  std::cerr << '\t'
            << "-T: indicates that the following argument characterize the cow's tongue."
               " If used more than once, only the first use will be considered.\n";
  std::cerr << '\t' << "-W: indicates that the following argument is the size of each line.\n";
  std::cerr << '\t'
            << "-J: indicates that the following argument represents the chosen alignment.\n";
  std::cerr << '\t' << "-h: prints help message.\n";
  std::cerr << '\t' << "-f: flips the animal horizontally.\n";
  std::cerr << '\t' << "-n: preserve the original message.\n";
  std::cerr << '\t' << "<style> represents the chosen animal style.\n";
  std::cerr << "Observations:\n";
  std::cerr << '\t'
            << "If any of the arguments is used more then once only the first use will be"
               " considered.\n";
  std::cerr << '\t' << "If more than one style is chosen only the first will be considered.\n";
  std::cerr << '\t'
            << "<style> option has preverence over the commands -E and -T, i.e. if the <style> "
               "option is used -E and -T will be desconsidered.\n";
}
