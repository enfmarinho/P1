#ifndef __MY_ALGORITHMS__
#define __MY_ALGORITHMS__

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <cstring>
#include <algorithm>
#include <cmath>

namespace algo {
    
    /**
     * Determine if @op1 has priority over @op2
     * 
     * @param op1 first operator.
     * @param op2 second operator.
     * 
    */
    bool higher_priority(std::string op1, std::string op2);
    /**
     * Converts a regular arithmetic expression to a posfix arithmetic expression.
     * 
     * @param tokens tokens of the regular arithmetic expression.
    */
    std::vector <std::string> convert_expression(const std::vector <std::string> &tokens);

    /**
     * Gives the result of the expression.
     * 
     * @param tokens expression in posfix order.
    */
    std::string evaluate_expression(const std::vector <std::string> &tokens);

    /**
     * Gives the result of a arithmetic expression.
     * 
     * @param a First term. It must be a integer.
     * @param b Second term. It must be an integer.
     * @param operand Operand of the expression. 
    */
    int result(int a, int b, std::string op);


    /**
     * Transforms a int in string.
     * 
     * @param x number.
    */
    std::string make_string(int x);

}
#endif