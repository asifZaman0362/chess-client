#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "state.hpp"
#include "chessboard.hpp"

namespace zifmann {
namespace chess {

class GameScene : public State {
    public:
        GameScene();
        void Render();
        void Update();
        void ProcessEvent(const sf::Event& event);
    private:
        void DrawBoard();
        ChessBoard m_board;
}

}}

#endif
