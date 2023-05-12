#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <SFML/Audio/Sound.hpp>
#include <array>

#include "animation.hpp"
#include "canvas.hpp"
#include "chessboard.hpp"
#include "chesspiece.hpp"
#include "eventsystem.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {

class GameScene : public State {
   public:
    GameScene(sf::RenderWindow *window);
    void Render(sf::RenderTarget &target);
    void Update(float dt);
    void ProcessEvent(const sf::Event &event);

   private:
    EventSystem m_eventSystem;
    ChessBoard m_board;
    Canvas m_canvas;
    sf::Sprite m_chessPiece;
    SpritesheetAnimation m_chessPieceAnim;
    sf::Sound m_bgmPlayer;
    sf::RenderWindow *m_window;
    friend void CreateUI(GameScene &scene);
};

}  // namespace chess
}  // namespace zifmann

#endif
