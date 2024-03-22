/**
 * @file main.cpp
 *
 * @description
 * This program implements a clone of the classic cowsay program.
 *
 * @author	Selan, <selan.santos@ufrn.br>
 * @date	November 13th, 2021
 *
 * @remark On 2022-09-08 new version initiated.
 */

#include <cstdlib>  // EXIT_SUCCESS
#include <string>
#include "CLI.h"
#include "animal.h"
#include "billboard.h"
#include "tokenizer.h"

int main(int argc, char** argv) {
  CLI arguments;
  arguments.read_command_line(argc, argv);

  // Reads input.
  std::string input, tmp;
  while (std::getline(std::cin, tmp)) {
    input += tmp + '\n';
  }
  std::string delimiters = " \t\v\f\r";
  std::string limiters = ",.:;?!";
  Tokenizer data(input, delimiters, limiters, arguments.get_trimming(), arguments.get_wrap_colum());

  Animal animal(
    arguments.get_style(), arguments.get_flip(), arguments.get_eyes(), arguments.get_tongue());
  Billboard billboard(
    data, arguments.get_wrap_colum(), arguments.get_alignment(), animal.get_width());
  billboard.print();
  animal.print();

  return EXIT_SUCCESS;
}
