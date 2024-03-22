#include "dice.h"
#include <random>

Dice::Dice()
{
    execution_state = RANDOM;
    default_value = 0;
}

void Dice::set_seed(seed_t execution_state, FaceType default_value)
{
    this->execution_state = execution_state;
    // Check if value of "default_value" is invalid.
    if (default_value < 0 || default_value > 5) {
        default_value = 0;
    }
    this->default_value = default_value;
}

Dice::FaceType Dice::roll(FaceType limit_value)
{
    if (execution_state == RANDOM) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis;
        return dis(gen) % limit_value;
    }
    else {
        return default_value;
    }
}
