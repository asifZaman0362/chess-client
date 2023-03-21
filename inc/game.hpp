#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "gamescene.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {
class Game {
   private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_running = false;

    void Update(float dt);
    void Render();
    void ProcessEvents(const sf::Event &event);

   public:
    Game();
    int Start();
};
}  // namespace chess
}  // namespace zifmann

#endif
