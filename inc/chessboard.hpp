#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "stdinc.hpp"
#include "chesspiece.hpp"

namespace zifmann {
namespace chess {


struct ChessSquare {
    Position m_position;
    Color m_color;
};

class ChessBoard {
    uint8_t m_config[8][8];
};

}}

#endif
