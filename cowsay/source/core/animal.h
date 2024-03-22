#ifndef COWSAY_H
#define COWSAY_H

/*!
 *  The cowsay classes
 */

#include <iostream>
#include <string>
using std::string;
#include <optional>
// using std::optional;
#include <vector>
#include <array>
using std::vector;

//=== Forward declarations
struct CLIOptions;

/// Class representing a text-based cow witha a personality!
class Animal {
public:
  //!< The cow's possible moods/appearances.
  enum mood_t : uint {
    NEUTRAL = 0,  //!< Default cow
    BORG,         //!< Borg mode
    DEAD,         //!< Dead
    GREEDY,       //!< Greedy
    PARANOIA,     //!< Paranoia
    STONED,       //!< Stoned
    TIRED,        //!< Tired
    WIRED,        //!< Wired
    YOUTH         //!< Young cow
  };

  /**
   * Initializes the animal.
   * @param style the mood of the animal.
   * @param flip flag that indicates if the animal has to be flipped.
   * @param eyes two characters representing the eyes of the animal.
   * @param tongue two characters representing the tongue of the animal.
   */
  Animal(std::optional <mood_t> style, bool flip, std::optional <string>eyes, std::optional <string> tongue);
  /// Displays the animal.
  void print() { std::cout << m_body; }
  /// Initialiezes the body of each animal style
  void init_templates ();
  ///  @return an int that represents the width of the animal. 
  int get_width();

private:
  bool m_flip;        //<! Flag that indicates if the animal has to be flipped.                        
  string m_body;      //<! The template of the animal's body.
  string m_eyes;      //<! The eyes of the animal that will be printed.
  string m_tongue;    //<! The tongue of the animal that will be printed.
  mood_t m_style;     //<! The mood of the animal.
  //!< Pair with the eyes and tongue of the animal.
  struct Body {
    string eyes;      //<! Eyes of the animal.
    string tongue;    //<! Tongue of the animal.
  };

  std::array <Body,9> m_styles_templates; //<! Saves the eyes and tongue of each possible style. 

  // TODO: Add other members/methods
};

#endif