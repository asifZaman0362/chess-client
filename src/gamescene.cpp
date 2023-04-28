#include "gamescene.hpp"

#include <SFML/Audio/SoundBuffer.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "assetmanager.hpp"
#include "logger.hpp"
#include "prefs.hpp"

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

GameScene::GameScene() {
    m_board = ChessBoard();
    m_chessPiece.setTexture(*AssetManager::GetTexture("chesspieces.png"));
    m_chessPiece.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_chessPiece.setScale(6, 6);
    if (!m_chessPieceAnim.Create(&m_chessPiece, Vector2ui(16, 16),
                                 Vector2ui(0, 0), 5, 0.16f)) {
        log_error("failed to load animation!");
    } else {
        m_chessPieceAnim.Play();
    }
    m_bgmPlayer.setBuffer(*GetRandomAudioClip());
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        m_bgmPlayer.play();
    }
}

void GameScene::Render(sf::RenderTarget &target) {
    m_board.Render(target);
    target.draw(m_chessPiece);
}

void GameScene::Update(float dt) { m_chessPieceAnim.Update(dt); }

void GameScene::ProcessEvent(const sf::Event &event) {}

}  // namespace chess
}  // namespace zifmann
