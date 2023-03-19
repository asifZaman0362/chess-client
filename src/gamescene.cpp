#include "gamescene.hpp"
#include <cstring>

namespace zifmann {
namespace chess {

GameScene::GameScene() {
    m_board = ChessBoard();
}

void GameScene::Render(sf::RenderTarget& target) {
    m_board.Render(target);
}

void GameScene::Update(float dt) {}

void GameScene::ProcessEvent(const sf::Event& event) {}

}}
