#include "game.hpp"
#include <memory>


namespace zifmann {
    namespace chess {

        Game::Game() {}

        int Game::Start() {
            m_window.create(sf::VideoMode(1000, 1000), "Chess");
            m_window.setFramerateLimit(60);
            m_running = true;
            auto gamescreen = std::make_unique<GameScene>();
            StateManager::LoadState(std::move(gamescreen));
            while (m_running) {
                sf::Event e{};
                auto dt = m_clock.restart().asSeconds();
                while (m_window.pollEvent(e)) {
                    if (e.type == sf::Event::Closed) {
                        m_running = false;
                    } else ProcessEvents(e);
                }
                Update(dt);
                Render();
            }
            m_window.close();
            return EXIT_SUCCESS;
        }

        void Game::Update(float dt) {
            StateManager::Update(dt);
        }

        void Game::ProcessEvents(const sf::Event& event) {}

        void Game::Render() {
            m_window.clear(sf::Color::Cyan);
            StateManager::Render(m_window);
            m_window.display();
        }

    }
}
