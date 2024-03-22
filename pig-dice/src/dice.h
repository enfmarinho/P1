#ifndef _DICE_H_
#define _DICE_H_

/**
 * This class represents a dice object, which keeps track of the current state of
 * execution, i.e. if it should generate a random number or a fixed one (for
 * debugging), and generates a random number.
 */
class Dice {
    public:
        using FaceType = short;
        /// Sets parameters to default.
        Dice();

        //!< Represents current state of execution.
        enum seed_t {
            FIXED,
            RANDOM
        };

        /*!
         * Generates a random value in the range [0;limit_value).
         * \param limit_value range limit value. If none is provided 6 will be used.
         */
        FaceType roll(FaceType limit_value = 6);

        /*!
         * Sets current state. Allows to choose between "fixed" or random dice.
         * \param execution_state represents current state of execution.
         * \param default_value value to be set to default. If none is provided or a invalid value is provided 0 will be used.
         */
        void set_seed(seed_t execution_state, FaceType default_value = 0);
    private:
        seed_t execution_state;
        FaceType default_value;
};

#endif // _DICE_H_