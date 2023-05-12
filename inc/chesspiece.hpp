#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "stdinc.hpp"

namespace zifmann {
namespace chess {

enum PieceKind {
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 4,
    ROOK = 8,
    QUEEN = 16,
    KING = 32,
    WHITE_PIECE = 64,
};

struct ChessPiece {
    Color m_color;
    Position m_position;
    PieceKind m_kind;
    sf::Sprite m_sprite;
};

}  // namespace chess
}  // namespace zifmann

#endif
