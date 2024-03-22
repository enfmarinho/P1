/**!
 *  This program implements the Snake simulation.
 *  @author Selan R. dos Santos
 */

#include "simulation.h"
#include <memory>
using namespace sg;

int main(int argc, char *argv[]) {
  // SnazeSimulation is a singleton.
  SnazeSimulation game_manager;
  game_manager.initialize(argc, argv);
  // The Game Loop.
  while (not game_manager.is_over()) {
    game_manager.process_events();
    game_manager.update();
    game_manager.render();
  }
  return 0;
}
