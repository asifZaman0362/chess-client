#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SFML/Graphics.hpp>
#include <array>

#include "chesspiece.hpp"
#include "eventsystem.hpp"
#include "stdinc.hpp"

namespace zifmann {
namespace chess {

struct ChessSquare {
    Position m_position;
    Color m_color;
};

class ChessBoard : MouseListener {
   public:
    ChessBoard(EventSystem &eventSystem);
    bool MakeMove(uint8_t x, uint8_t y, uint8_t piece);
    bool TakePiece(uint8_t x, uint8_t y);
    void Render(sf::RenderTarget &target);
    void GenerateBoard(Color color);
    void PickSquare(const sf::Vector2i mousePosition);

    void OnMouseButtonDown(sf::Mouse::Button button) override;
    void OnMouseButtonUp(sf::Mouse::Button button) override;
    void OnMouseMove(sf::Vector2i position) override;

   private:
    uint8_t m_config[8][8];
    int picked_x = 0;
    int picked_y = 0;
    int selected_x = -1, selected_y = -1;
    sf::RectangleShape squares[8][8];
    std::array<ChessPiece, 32> m_pieces;
    sf::RenderWindow *m_window;

    void HighlightValidSquares();
};

}  // namespace chess
}  // namespace zifmann

#endif
