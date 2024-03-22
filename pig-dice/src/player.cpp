#include <iostream>

#include "pigdice_common.h"
#include "player.h"

// === Auxiliary functions to help user input

/// Trim off white spaces from left part of a string.
inline std::string& ltrim(std::string& s, const char* t = " \t\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// === Regular methods of the Player class.
action_e Player::next_action()
{
    std::string input;
    getline(std::cin, input);
    input.push_back('\n'); // To compensate for '\n' ignored by getline.
    input = ltrim(input);
    char next_action = input[0];
    switch (next_action) {
        case '\n':
            return action_e::ROLL;
        case 'r':
            return action_e::ROLL;
        case 'h':
            return action_e::HOLD;
        case 'q':
            return action_e::QUIT;
        default:
            return action_e::NONE;
    }
}

ScoreType Player::get_score() const
{
    ScoreType current_score = 0;
    for (auto elements : m_turns_log) {
        current_score += elements.pts;
    }
    return current_score;
}


