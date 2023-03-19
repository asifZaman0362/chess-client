#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "state.hpp"
#include "gamescene.hpp"

namespace zifmann {
    namespace chess {
        class Game {
            private:
                sf::RenderWindow m_window;
                bool m_running = false;

                void Update();
                void Render();
                void ProcessEvents(const sf::Event& event);

            public:
                Game();
                int Start();
        };
    }
}

#endif
