#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <utility>

#include "chesspiece.hpp"
#include "stdinc.hpp"

namespace zifmann {
namespace chess {

struct ChessSquare {
    Position m_position;
    Color m_color;
};

class ChessBoard {
   public:
    ChessBoard();
    bool MakeMove(uint8_t x, uint8_t y, uint8_t piece);
    bool TakePiece(uint8_t x, uint8_t y);
    void Render(sf::RenderTarget &target);
    void GenerateBoard(Color color);

   private:
    uint8_t m_config[8][8];
    sf::RectangleShape squares[8][8];
    std::array<std::pair<sf::Sprite, ChessPiece>, 32> m_pieces;
};

}  // namespace chess
}  // namespace zifmann

#endif
