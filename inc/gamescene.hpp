#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <SFML/Audio/Sound.hpp>
#include <array>
#include <utility>

#include "animation.hpp"
#include "canvas.hpp"
#include "chessboard.hpp"
#include "chesspiece.hpp"
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
    Canvas m_canvas;
    sf::Sprite m_chessPiece;
    SpritesheetAnimation m_chessPieceAnim;
    sf::Sound m_bgmPlayer;
    std::array<std::pair<sf::Sprite, ChessPiece>, 32> m_pieceSprites;
    friend void CreateUI(GameScene &scene);
};

}  // namespace chess
}  // namespace zifmann

#endif
