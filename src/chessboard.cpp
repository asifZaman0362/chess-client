#include "chessboard.hpp"

#include <cstring>

namespace zifmann {
namespace chess {

ChessBoard::ChessBoard() {
    memset(m_config, 0, 64);
    m_config[0][0] = ROOK;
    m_config[0][1] = KNIGHT;
    m_config[0][2] = BISHOP;
    m_config[0][3] = QUEEN;
    m_config[0][4] = KING;
    m_config[0][5] = BISHOP;
    m_config[0][6] = KNIGHT;
    m_config[0][7] = ROOK;
    memset(m_config[1], PAWN, 8);
    memset(m_config[6], PAWN | WHITE, 8);
    m_config[7][0] = WHITE | ROOK;
    m_config[7][1] = WHITE | KNIGHT;
    m_config[7][2] = WHITE | BISHOP;
    m_config[7][3] = WHITE | QUEEN;
    m_config[7][4] = WHITE | KING;
    m_config[7][5] = WHITE | BISHOP;
    m_config[7][6] = WHITE | KNIGHT;
    m_config[7][7] = WHITE | ROOK;
    for (uint8_t i = 0; i < 64; i++) {
        auto x = i / 8;
        auto y = i % 8;
        squares[x][y] = sf::RectangleShape(sf::Vector2f(100, 100));
        squares[x][y].setFillColor((i + ((i / 8) % 2)) % 2 ? sf::Color::White
                                                           : sf::Color::Black);
        squares[x][y].setPosition(x * 100, y * 100);
    }
}

bool ChessBoard::TakePiece(uint8_t x, uint8_t y) {
    if (x > 8 || y > 8)
        return false;
    else if (!m_config[y][x])
        return false;  // empty
    // play animation of taking away the piece
    return true;
}

bool ChessBoard::MakeMove(uint8_t x, uint8_t y, uint8_t piece) {
    if (x > 8 || y > 8) return false;  // invalid position
    if ((m_config[y][x] & 64) ==
        (piece & 64))  // same color piece already in x,y
        return false;
    if (m_config[y][x] != 0) TakePiece(x, y);
    m_config[y][x] = piece;
    return true;
}

void ChessBoard::Render(sf::RenderTarget &target) {
    // Draw board
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            target.draw(squares[x][y]);
        }
    }
}

}  // namespace chess
}  // namespace zifmann
