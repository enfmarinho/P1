#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <iostream>

#include "pigdice_common.h"

/// Representing a single player.
class Player {
    private:
        std::string m_name; //!< The player's name
        std::vector<TurnInfo> m_turns_log; //!< List of turn total the player received in the match.

    public:
        explicit Player(std::string &n) : m_name{n}
        {/*empty*/}
        Player() = default;
        /// Destructor.
        ~Player() = default;
        /// Returns the player decision (action).
        action_e next_action();
        /// Returns the player's name
        std::string name() const { return m_name; };
        /// Returns the player's log of turn total s/he's won so far in the match.
        std::vector<TurnInfo> get_turns_log() const { return m_turns_log; }
        /// Add a turn value to the player's log.
        void add_turn_total(const TurnInfo &turn_value) { m_turns_log.push_back(turn_value);}
        /// Return the player's current score.
        ScoreType get_score() const;
};

#endif //_PLAYER_H_