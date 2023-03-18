#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

#include "stdinc.hpp"

namespace zifmann {
namespace chess {

#define BLACK_PAWN 0
#define BLACK_KNIGHT 1
#define BLACK_BISHOP 2
#define BLACK_ROOK 3
#define BLACK_KING 4
#define BLACK_QUEEN 5
#define WHITE_PAWN 6
#define WHITE_KNIGHT 7
#define WHITE_BISHOP 8
#define WHITE_ROOK 9
#define WHITE_QUEEN 10
#define WHITE_KING 11

enum PieceKind {
    PAWN = 0, KNIGHT, BISHOP, ROOK, QUEEN, KING
};


class ChessPiece {
    Color m_color;
    Position m_position;
    PieceKind m_kind;
};

}}

#endif
