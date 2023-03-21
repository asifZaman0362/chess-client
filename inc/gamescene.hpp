#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "animation.hpp"
#include "chessboard.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {

class GameScene : public State {
   public:
    GameScene();
    void Render(sf::RenderTarget &target);
    void Update(float dt);
    void ProcessEvent(const sf::Event &event);

   private:
    ChessBoard m_board;
    sf::Sprite m_chessPiece;
    SpritesheetAnimation m_chessPieceAnim;
};

}  // namespace chess
}  // namespace zifmann

#endif
