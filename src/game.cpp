#include "game.hpp"

#include <memory>

#include "assetmanager.hpp"
#include "cursor.hpp"
#include "menu.hpp"

namespace zifmann {
namespace chess {

bool Game::m_running = false;

Game::Game() {}

int Game::Start() {
    m_window.create(sf::VideoMode(1000, 1000), "SuperMegaChess");
    m_window.setFramerateLimit(60);
    m_running = true;
    auto menu = std::make_unique<MenuScene>(&m_window);
    StateManager::LoadState(std::move(menu));
    Cursor::Setup(&m_window, AssetManager::GetTexture("cursor.png"),
                  sf::IntRect(0, 0, 14, 14));
    Cursor::SetSize(sf::Vector2f(28, 28));
    while (m_running) {
        sf::Event e{};
        auto dt = m_clock.restart().asSeconds();
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_running = false;
            } else
                ProcessEvents(e);
        }
        Update(dt);
        Render();
    }
    m_window.close();
    return EXIT_SUCCESS;
}

void Game::Update(float dt) { StateManager::Update(dt); }

void Game::ProcessEvents(const sf::Event &event) {
    if (event.type == sf::Event::MouseMoved) Cursor::Update();
    StateManager::ProcessEvent(event);
}

void Game::Render() {
    m_window.clear(sf::Color::Cyan);
    StateManager::Render(m_window);
    Cursor::Draw(m_window);
    m_window.display();
}

void Game::Quit() {
    StateManager::OnQuit();
    m_running = false;
}

}  // namespace chess
}  // namespace zifmann
