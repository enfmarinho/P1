#ifndef _PIG_DICE_COMMON_H_
#define _PIG_DICE_COMMON_H_

#include "dice.h"

/// Enumeration for each dice face.
enum dice_face_e : short { FACE_1=0, FACE_2, FACE_3, FACE_4, FACE_5, FACE_6, FACE_UNDEFINED };
/// Type of action that can happen in the game.
enum class action_e : short { HOLD=0, ROLL, QUIT, NONE };
/// Player index ID
enum player_e : short { HUMAN=0, MACHINE, UNDEFINED };
/// Type associated with scores in this game.
using ScoreType = short;
/// # of players in a match.
constexpr short N_PLAYERS = 2;
/// Minimum score to win.
constexpr ScoreType WINING_SCORE = 100;
/*!
 * A turn item is a pair that associates a score (pts) and the number
 * of times the dice was rolled to produce that score.
 * For instance: <21, 4> means that the player got 21 pts after
 * rolling the dice 4 times.
 * Similarly, <0, 3> means that the player got not points after rolling
 * the dice 3 times.
 */
struct TurnInfo {
    ScoreType pts; //!< Accumulated points of a turn in progress.
    short n_rolls; //!< # of dice roll of a turn in progress.
};

#endif
