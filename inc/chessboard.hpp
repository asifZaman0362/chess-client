#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "stdinc.hpp"
#include <SFML/Graphics.hpp>
#include "chesspiece.hpp"

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
        void Render(sf::RenderTarget& target);
    private:
        uint8_t m_config[8][8];
        sf::RectangleShape squares[8][8];
};

}}

#endif
