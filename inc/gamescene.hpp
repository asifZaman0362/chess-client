#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "state.hpp"
#include "chessboard.hpp"
#include "animation.hpp"

namespace zifmann {
namespace chess {

class GameScene : public State {
    public:
        GameScene();
        void Render(sf::RenderTarget& target);
        void Update(float dt);
        void ProcessEvent(const sf::Event& event);
    private:
        ChessBoard m_board;
        sf::Sprite m_chessPiece;
        SpritesheetAnimation m_chessPieceAnim;
};

}}

#endif
