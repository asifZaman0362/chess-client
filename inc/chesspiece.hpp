#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

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
    WHITE = 64
};

class ChessPiece {
    Color m_color;
    Position m_position;
    PieceKind m_kind;
};

}  // namespace chess
}  // namespace zifmann

#endif
