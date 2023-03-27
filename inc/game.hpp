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
    static bool m_running;

    void Update(float dt);
    void Render();
    void ProcessEvents(const sf::Event &event);

   public:
    Game();
    int Start();
    static void Quit();
};
}  // namespace chess
}  // namespace zifmann

#endif
