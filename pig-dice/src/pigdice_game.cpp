/*!
 * Game Controller implementation.
 *
 * \author Selan
 * \date March 23rd, 2022
 */
#include <iomanip>

#include "pigdice_common.h"
#include "pigdice_game.h"

// Just for debugging.
static std::string state_names[] = {
    "STARTING",
    "WELCOME",
    "PLAYING",
    "ROLLING",
    "HOLDING",
    "UPDATING SCORE",
    "QUITTING",
    "ENDING"
};

/// Action names
static const std::string action_names[] = { "Hold", "Roll", "Quit", "Unknown" };
/// String representation for the dice faces (Unicode).
static const std::string dice_faces[] = { "\u2680", "\u2681", "\u2682", "\u2683", "\u2684", "\u2685", "Undefined" };

void GameController::initialize()
{
    // -----------------------------------------------------------------------
    // Choosing between a "fixed" or random dice.
    // -----------------------------------------------------------------------
    // [1]== Fixed seed, which means this dice generates the same sequence of numbers.
    //m_dice_object.set_seed( Dice::seed_t::FIXED );
    // [2]== A random seed means the numbers produced by the dice ara random.
    m_dice_object.set_seed( Dice::seed_t::RANDOM );
    // -----------------------------------------------------------------------

    // Create players
    std::string Jack = "Jack";
    std::string Machine = "Machine";
    m_players[player_e::MACHINE] = Player{Machine};
    m_players[player_e::HUMAN] = Player{ Jack};
    // Reset turn total information: <score,# rolls>
    m_turn_total = TurnInfo{0,0};
    // No starting player defined just yet.
    m_player = player_e::UNDEFINED;
    // set current action.
    m_action = action_e::NONE;
    // No winner yet.
    m_winner = player_e::UNDEFINED;
    // Die undefined.
    m_face = dice_face_e::FACE_UNDEFINED;
    // Current game state.
    m_game_state = game_state_e::STARTING;
    // We do not want to quit just yet.
    m_asked_to_quit = false;
}

bool GameController::game_over() const
{
    return m_game_state == game_state_e::ENDING;
}

void GameController::read_enter()
{
    // Read input until receive a '\n'.
    char input;
    do {
        input = getchar();
    } while (input != '\n');
}

void GameController::read_human_cmd()
{
    m_action = m_players[player_e::HUMAN].next_action();
}

GameController::risk_strategies GameController::risk_strategy()
{
    ScoreType human_score = m_players[player_e::HUMAN].get_score();
    ScoreType machine_score = m_players[player_e::MACHINE].get_score();

    ScoreType my_score, adversary_score;
    if (m_player == player_e::HUMAN) {
        my_score = human_score;
        adversary_score = machine_score;
    } else {
        my_score = machine_score;
        adversary_score = human_score;
    }

    if (adversary_score > 85) {
        return risk_strategies::RISKY;
    } else if (adversary_score - my_score > 50) {
        return risk_strategies::RISKY;
    } else {
        return risk_strategies::SAFE;
    }
}

void GameController::machine_action()
{
    risk_strategies strategy = risk_strategy();
    ScoreType machine_points = m_players[player_e::MACHINE].get_score();
    // Check if there is enough points to win.
    if (m_turn_total.pts + machine_points >= 100) {
        m_action = action_e::HOLD;
        return;
    }

    if (strategy == risk_strategies::RISKY && m_turn_total.pts < 20) {
        m_action = action_e::ROLL;
    }
    else if (strategy == risk_strategies::RISKY) {
        m_action = action_e::HOLD;
    }
    else if (m_turn_total.pts < 15) {
        m_action = action_e::ROLL;
    }
    else {
        m_action = action_e::HOLD;
    }
}

bool GameController::read_user_confirmation() const
{
    auto STR_LOWERCASE = [](const char* t)->std::string
    {
        std::string str{ t };
        std::transform( str.begin(), str.end(), str.begin(), ::tolower );
        return str;
    };
    // Read a simple line with the command from user.
    std::string line;
    std::getline(std::cin,line); // Let us ignore any prepending white spaces.
    std::string low_line = STR_LOWERCASE(line.c_str());

    if ( low_line == "y" or low_line == "yes" or low_line == "ye" or low_line == "ys")
        return true;
    else return false;
}

void GameController::confirm_quitting()
{
    m_asked_to_quit = read_user_confirmation();
    if (not m_asked_to_quit) {
        m_game_state = game_state_e::PLAYING;
    }
}

void GameController::process_events()
{
    if ( m_game_state == game_state_e::STARTING or
         m_game_state == game_state_e::ROLLING or
         m_game_state == game_state_e::HOLDING )
    {
        // Do nothing, no interaction in these states.
    }
    else if ( m_game_state == game_state_e::WELCOME )
    {
        read_enter();
    }
    else if ( m_game_state == game_state_e::PLAYING and
              m_player == player_e::HUMAN)
    {
        read_human_cmd();
    }
    else if (m_game_state == game_state_e::PLAYING and
             m_player == player_e::MACHINE)
    {
        machine_action();
    }
    else if ( m_game_state == game_state_e::UPDATING_SCORE )
    {
        read_enter();
    }
    else if ( m_game_state == game_state_e::QUITTING )
    {
        confirm_quitting();
    }
}

void GameController::update()
{
    if ( m_game_state == game_state_e::STARTING )
    {
        // Randomly choose who's going to start the match
        m_player = m_dice_object.roll(2); // heads or tails?
        m_game_state = game_state_e::WELCOME;
    }
    else if ( m_game_state == game_state_e::WELCOME )
    {
        // Go to PLAY state.
        m_game_state = game_state_e::PLAYING;
    }
    else if ( m_game_state == game_state_e::PLAYING )
    {   
        if (m_action == action_e::HOLD) m_game_state = game_state_e::HOLDING;
        else if (m_action == action_e::ROLL) {
            m_face = m_dice_object.roll(6);
            if (m_face == dice_face_e::FACE_1) {
                m_turn_total.pts = 0;
            }
            else {
                m_turn_total.pts += m_face + 1;
            }
            
            m_turn_total.n_rolls ++;
            m_game_state = game_state_e::ROLLING;
        }
        else if (m_action == action_e::QUIT) m_game_state = game_state_e::QUITTING;
    }
    else if ( m_game_state == game_state_e::ROLLING )
    {
    }
    else if ( m_game_state == game_state_e::HOLDING )
    {
    }
    else if ( m_game_state == game_state_e::UPDATING_SCORE )
    {
        m_players[m_player].add_turn_total(m_turn_total);
        if (m_players[m_player].get_score() >= 100)
        {
            m_game_state = game_state_e::ENDING;
            m_winner = m_player;
        }
        else {
            if (m_player == player_e::MACHINE) m_player = player_e::HUMAN;
            else m_player = player_e::MACHINE;
            m_turn_total.pts = 0;
            m_turn_total.n_rolls = 0;
            m_game_state = game_state_e::PLAYING;
        }
    }
    else if ( m_game_state == game_state_e::QUITTING )
    {   
        if (m_asked_to_quit) m_game_state = game_state_e::ENDING;
    }
    else
    {
        // Do nothing in these states.
        //  + ENDING
    }
}

void GameController::display_welcome_screen() const 
{
    std::cout << "\t\t---> Welcome to the Pig Dice game (v 1.1) <---\n";
    std::cout << "\t\t\t-copyright DIMAp/UFRN 2022-\n\n";
    std::cout << "The object of the jeopardy dice game Pig is to be the first player to reach 100 points.\n";
    std::cout << "Each player's turn consists of repeatedly rolling a die. After each roll, the player is\n";
    std::cout << "faced with two choices: roll again, or hold (decline to roll again).\n";
    std::cout << "\t• If the player rolls a 1, the player scores nothing and it becomes the opponent's turn.\n";
    std::cout << "\t• If the player rolls a number other than 1, the number is added to the player's turn\n";
    std::cout << "\t    total and the player's turn continues.\n";
    std::cout << "\t• If the player holds, the turn total, the sum of the rolls during the turn, is added\n";
    std::cout << "\t  to the player's score, and it becomes the opponent's turn.\n\n";
    std::cout << ">>> The players of the game are: " << m_players[player_e::HUMAN].name();
    std::cout << " & " << m_players[player_e::MACHINE].name() << "\n\n";
    std::cout << ">>> The player who will start the game is \"" << m_players[m_player].name() << "\"\n";
    std::cout << "    Press <Enter> to start the match.";
}

void GameController::display_playing_screen(size_t player_of_the_moment) const 
{
    if (player_of_the_moment == player_e::HUMAN) {
        std::cout << "-------------------------------------------------------\n";
        std::cout << ">>> The current player is: " << m_players[HUMAN].name() << "\n\n";
        std::cout << "Commands syntax:\n";
        std::cout << "  <Enter>       -> ROLL the dice.\n";
        std::cout << "  'r' + <Enter> -> ROLL the dice.\n";
        std::cout << "  'h' + <Enter> -> HOLD (add turn total and pass turn over).\n";
        std::cout << "  'q' + <Enter> -> quit the match (no winner).\n\n";
        std::cout << "Enter command > ";

    }
    else if (player_of_the_moment == player_e::MACHINE) {
        std::cout << "-------------------------------------------------------\n";
        std::cout << ">>> The current player is: " << m_players[player_e::MACHINE].name()<< "\n";
    }
}

void GameController::display_rolling_screen() const 
{
    if (m_face == dice_face_e::FACE_1) {
        std::cout << ">>> Requested action: \"Roll\"\n";
        std::cout << "    Dice value is 1\n";
        std::cout << "    Ops, got a Pig, holding...\n";
        std::cout << "    The turn total is: 0\n";
    }
    else {
        std::cout << ">>> Requested action: \"Roll\"\n";
        std::cout << "    Dice value is " << m_face + 1 << "\n";
        std::cout << "    The turn total is: " << m_turn_total.pts << "\n";
    }
}

void GameController::display_score_board() const 
{
    std::cout << '\n';
    std::cout << "┌───────────────────────┐\n";
    std::cout << "│      Score Board      │\n";
    std::cout << "├───────────┬───────────┤\n";
    std::cout << "│       Jack│        ";
    std::cout << std::setw(3) << (m_player == player_e::HUMAN? m_players[player_e::HUMAN].get_score()+m_turn_total.pts : m_players[player_e::HUMAN].get_score());
    std::cout << "│\n";
    std::cout << "│    Machine│        ";
    std::cout << std::setw(3) << (m_player == player_e::MACHINE? m_players[player_e::MACHINE].get_score() + m_turn_total.pts : m_players[player_e::MACHINE].get_score());
    std::cout << "│\n";
    std::cout << "└───────────┴───────────┘\n\n";
    std::cout << ">>> Press enter to continue...\n";
}

void GameController::display_holding_screen() const 
{
    std::cout << ">>> Requested action: \"Hold\"\n";
    std::cout << "    The turn total is: " << m_turn_total.pts << '\n';
}

void GameController::display_ending_screen() const 
{
    if (not m_asked_to_quit){
        std::cout << ">>> THE WINNER IS "<< (m_winner == player_e::MACHINE? "Machine\n" : "Jack\n") << "\n";
        std::cout << "[[ LOG of ACTIONS during the game ]]\n";
        std::cout << "Decisions taken by player \"Jack\":\n";
        std::vector <TurnInfo> human_turn_log = m_players[player_e::HUMAN].get_turns_log();
        for (size_t idx = 0; idx < human_turn_log.size(); idx++) {
            std::cout << human_turn_log[idx].n_rolls << " dice rolls produced " << human_turn_log[idx].pts << "\n";
        }
        std::cout << "\n Decisions taken by player \"Machine\":\n";
        std::vector <TurnInfo> machine_turn_log = m_players[player_e::MACHINE].get_turns_log();
        for (size_t idx = 0; idx < machine_turn_log.size(); idx++) {
            std::cout << machine_turn_log[idx].n_rolls << " dice rolls produced " << machine_turn_log[idx].pts << "\n";
        }
        std::cout << "\n\t---> [ Thanks for playing. See you next time! ] <---\n";
    }
    else {
        std::cout << "Sorry you wanna quit\n";
    }
}

void GameController::display_quitting_screen() const 
{
    std::cout << "Are you sure wanna quit? [Y,N]:\n";
}

void GameController::render()
{
    if ( m_game_state == game_state_e::STARTING )
    {
        // Do nothing in this state.
    }
    else if ( m_game_state == game_state_e::WELCOME)
    {
        // Welcome msg and rules.
        display_welcome_screen();
    }
    else if ( m_game_state == game_state_e::PLAYING )
    {
        display_playing_screen(m_player);
    }
    else if ( m_game_state == game_state_e::ROLLING)
    {
        display_rolling_screen();
        display_playing_screen(m_player);
        if (m_face == dice_face_e::FACE_1) {
            display_score_board();
            m_game_state = game_state_e::UPDATING_SCORE;
        }
        else m_game_state = game_state_e::PLAYING;

    }
    else if ( m_game_state == game_state_e::HOLDING )
    {
        display_holding_screen();
        display_score_board();
        m_game_state = game_state_e::UPDATING_SCORE;
    }
    else if ( m_game_state == game_state_e::UPDATING_SCORE )
    {
    }
    else if ( m_game_state == game_state_e::ENDING )
    {
        display_ending_screen();
    }
    else if ( m_game_state == game_state_e::QUITTING )
    {
        display_quitting_screen();
    }
}