#ifndef _PIG_DICE_GAME_H_
#define _PIG_DICE_GAME_H_

#include "pigdice_common.h"
#include "player.h"
#include "dice.h"

/*!
 * This class represents the Game Controller which keeps track of player,
 * scores, and turn total values, as well as determining when a match ends.
 */
class GameController {
    private:
        //=== Structs
        //!< Possible game states.
        enum class game_state_e : uint {
            STARTING=0,          //!< Beginning the game.
            WELCOME,             //!< Opening messages.
            PLAYING,             //!< Where the game action takes place.
            ROLLING,             //!< We roll the dice to get a face number.
            HOLDING,             //!< User asked to hold and pass turn to the next player.
            UPDATING_SCORE,      //!< Update the score board and show command issued in the turn.
            QUITTING,            //!< User might want to quit (before game finishes), need to confirm.
            ENDING,              //!< Closing the game (final message).
        };

        //!< Represents strategies to be followed.
        enum risk_strategies {
            SAFE=0,
            RISKY
        };

        //=== Data members
        Player m_players[N_PLAYERS]; //!< List of players.
        bool m_asked_to_quit;        //!< Flag that indicates whether the user wants to end an ongoing game.
        size_t m_player;             //!< Index of the player of the turn.
        action_e m_action;           //!< Action chosen by the current player.
        Dice::FaceType m_face;       //!< Current face.
        Dice m_dice_object;          //!< Dice object.
        TurnInfo m_turn_total;       //!< Current turn score <pts, # of times dice has been rolled>/
        game_state_e m_game_state;   //!< Current game state.
        size_t m_winner;             //!< Index of the winner.

    public:
        GameController()=default;
        GameController(const GameController&) = delete;
        GameController(GameController&&) = delete;
        GameController& operator=(const GameController&) = delete;
        GameController& operator=(GameController&&) = delete;
        ~GameController() = default;

        //=== Common methods for the Game Loop design pattern.
        void initialize();
        /// Renders the game to the user.
        void render();
        /// Update the game based on the current game state.
        void update();
        /// Process user input events, depending on the current game state.
        void process_events();
        /// Returns true when the user wants to quit the game.
        bool game_over() const;
        /// Displays welcome message on the screen.
        void display_welcome_screen() const;
        /**
         * Displays the current player and instructions, if it's the human's turn.
         * @param player_of_the_moment The current play of the turn.
        */
        void display_playing_screen(size_t player_of_the_moment) const;
        /// Displays the requested action, dice value and turn total.
        void display_rolling_screen() const;
        /// Displays the requested action and turn total.
        void display_holding_screen() const;
        /// Displays the score board of the game.
        void display_score_board() const;
        /// Displays the ending screen based on two possible situations: there was a winner or the player wanted to quit.
        void display_ending_screen() const;
        /// Displays a message for the quitting player.
        void display_quitting_screen() const;

    private:
        /// Read input until enter key.
        void read_enter();
        /// Reads human command.
        void read_human_cmd();
        /**
         * Define a risk strategy to be followed.
         * @return risk_strategy that contains the strategy.
         */
        risk_strategies risk_strategy();
        /// Calls for machine move.
        void machine_action();
        /// Checks if user confirm action.
        bool read_user_confirmation() const;
        /// Checks if user confirm quitting.
        void confirm_quitting();
};
#endif // _PIG_DICE_GAME_H_