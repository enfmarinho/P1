#include "algorithms.h"

bool algo::higher_priority(std::string op1, std::string op2) {
  if (op1 == "(")
    return true;
  else if (op1 == "^" and op2 == "^")
    return true;
  else if (op1 == "^" and
           (op2 == "+" or op2 == "-" or op2 == "*" or op2 == "%" or op2 == "/"))
    return true;
  else if ((op1 == "*" or op1 == "/" or op1 == "%") and
           (op2 == "+" or op2 == "-"))
    return true;
  else if (op2 == "(")
    return true;
  else
    return false;
}

std::vector<std::string>
algo::convert_expression(const std::vector<std::string> &tokens) {
  std::stack<std::string> prision;
  std::vector<std::string> posfix;
  std::set<std::string> operators = {"+", "-", "^", "/", "%", "*", "("};
  for (auto t : tokens) {
    if (t == ")") {
      while (prision.top() != "(") {
        posfix.push_back(prision.top());
        prision.pop();
      }
      prision.pop();
      continue;
    }

    if (operators.find(t) != operators.end()) {
      if (prision.empty() or higher_priority(t, prision.top()))
        prision.push(t);
      else {
        while (!prision.empty() and prision.top() != "(" and
               !higher_priority(t, prision.top())) {
          posfix.push_back(prision.top());
          prision.pop();
        }
        prision.push(t);
      }
    } else
      posfix.push_back(t);
  }

  while (!prision.empty()) {
    posfix.push_back(prision.top());
    prision.pop();
  }

  return posfix;
}

std::string algo::evaluate_expression(const std::vector<std::string> &tokens) {
  std::stack<int> aux;
  std::set<std::string> operators = {"+", "-", "^", "/", "%", "*", "("};
  for (auto t : tokens) {
    if (operators.find(t) != operators.end()) {
      int second = aux.top();
      aux.pop();
      int first = aux.top();
      aux.pop();
      if (t == "/" and second == 0)
        return "Division by zero!";
      if (t == "^" and first == 0 and second == 0)
        return "Undefined value!";
      aux.push(result(first, second, t));
      if (aux.top() < -32768 or aux.top() > 32767)
        return "Numeric overflow error!";
    } else {
      aux.push(std::stoi(t));
    }
  }
  return make_string(aux.top());
}

int algo::result(int a, int b, std::string op) {
  if (op == "+")
    return a + b;
  else if (op == "-")
    return a - b;
  else if (op == "*")
    return a * b;
  else if (op == "/")
    return a / b;
  else if (op == "^")
    return (int)pow(a, b);
  else
    return a % b;
}

std::string algo::make_string(int x) {
  std::string aux;
  if (x == 0)
    aux += '0';
  else {
    bool flag = false;
    if (x < 0) {
      x = std::abs(x);
      flag = true;
    }
    while (x) {
      aux += (x % 10) + '0';
      x /= 10;
    }
    if (flag)
      aux += '-';
  }
  reverse(aux.begin(), aux.end());
  return aux;
}
