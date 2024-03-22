#include "algorithms.h"
#include "lib/parser.h"
#include "lib/tokenizer.h"
#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // cerr, getline, cin
#include <string>

int main() {
  std::string input;
  std::string delimiters = " ";
  std::string limiters = "+-/*^%()";
  while (std::getline(std::cin, input)) {
    Parser parser(input);
    if (parser.consult_err_msg() != "") {
      std::cout << parser.consult_err_msg() << '\n';
      continue;
    }
    Tokenizer tokens(input, delimiters, limiters);
    // tokens.print_all_tokens();
    std::cout << algo::evaluate_expression(
                     algo::convert_expression(tokens.get_all_tokens()))
              << '\n';
  }

  return EXIT_SUCCESS;
}
