#include "gamescene.hpp"

#include <SFML/Audio/SoundBuffer.hpp>
#include <array>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <utility>

#include "assetmanager.hpp"
#include "canvas.hpp"
#include "chesspiece.hpp"
#include "logger.hpp"
#include "prefs.hpp"
#include "stdinc.hpp"

namespace zifmann {
namespace chess {

sf::SoundBuffer *GetRandomAudioClip() {
    srand(time(0));
    int clip = rand() % 2;
    if (clip) {
        return AssetManager::GetAudioClip("creaturecomfort.ogg");
    } else
        return AssetManager::GetAudioClip("tranquility.ogg");
}

void CreateUI(GameScene &scene) {
    auto font = AssetManager::GetFont("kenneypixel.ttf");
    auto m_playerName = new CanvasText(
        "player", font, sf::IntRect(50, 50, 200, 40), Left | Top, Constant);
    auto m_opponentName =
        new CanvasText("opponent", font, sf::IntRect(50, 50, 200, 40),
                       Right | Bottom, Constant);
    scene.m_canvas.AddChild(m_playerName);
    scene.m_canvas.AddChild(m_opponentName);
}

GameScene::GameScene(sf::RenderWindow *window)
    : m_eventSystem(EventSystem()), m_board(m_eventSystem) {
    m_bgmPlayer.setBuffer(*GetRandomAudioClip());
    m_window = window;
    CreateUI(*this);
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        m_bgmPlayer.play();
    }
}

void GameScene::Render(sf::RenderTarget &target) { m_board.Render(target); }

void GameScene::Update(float dt) {}

void GameScene::ProcessEvent(const sf::Event &event) {
    m_eventSystem.ProcessEvent(event, *m_window);
}

}  // namespace chess
}  // namespace zifmann
