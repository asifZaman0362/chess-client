#include "chessboard.hpp"

#include <cstring>

#include "assetmanager.hpp"
#include "chesspiece.hpp"
#include "networkplayer.hpp"

namespace zifmann {
namespace chess {

void SetPieceSprite(ChessPiece &piece) {
    sf::Sprite sprite(*AssetManager::GetTexture("chesspieces.png"));
    sf::IntRect textureRect{0, 0, 16, 16};
    if (piece.m_color == WHITE) {
        textureRect.top += 16 * 6;
    }
    switch (piece.m_kind) {
        case PAWN:
            textureRect.top += 0;
            break;
        case ROOK:
            textureRect.top += 16;
            textureRect.left += 16;
            break;
        case KNIGHT:
            textureRect.top += 32;
            textureRect.left += 16;
            break;
        case BISHOP:
            textureRect.top += 48;
            textureRect.left += 16;
            break;
        case KING:
            textureRect.top += 64;
            textureRect.left += 16;
            break;
        case QUEEN:
            textureRect.top += 80;
            textureRect.left += 16;
            break;
    }
    sprite.setTextureRect(textureRect);
    piece.m_sprite = sprite;
}

ChessBoard::ChessBoard() {
    memset(m_config, 0, 64);
    m_config[0][0] = WHITE | ROOK;
    m_config[0][1] = WHITE | KNIGHT;
    m_config[0][2] = WHITE | BISHOP;
    m_config[0][3] = WHITE | QUEEN;
    m_config[0][4] = WHITE | KING;
    m_config[0][5] = WHITE | BISHOP;
    m_config[0][6] = WHITE | KNIGHT;
    m_config[0][7] = WHITE | ROOK;
    memset(m_config[1], PAWN | WHITE, 8);
    memset(m_config[6], PAWN, 8);
    m_config[7][0] = ROOK;
    m_config[7][1] = KNIGHT;
    m_config[7][2] = BISHOP;
    m_config[7][3] = QUEEN;
    m_config[7][4] = KING;
    m_config[7][5] = BISHOP;
    m_config[7][6] = KNIGHT;
    m_config[7][7] = ROOK;
    for (uint8_t i = 0; i < 64; i++) {
        auto x = i / 8;
        auto y = i % 8;
        int ty = network::NetworkPlayer::PLAYER_WHITE_PIECES ? 7 - y : y;
        squares[x][y] = sf::RectangleShape(sf::Vector2f(100, 100));
        squares[x][y].setFillColor((i + ((i / 8) % 2)) % 2 ? sf::Color::White
                                                           : sf::Color::Black);
        squares[x][y].setPosition(x * 100, ty * 100);
    }
    ChessPiece blacks[16];
    for (size_t i = 0; i < 8; i++) {
        auto &piece = blacks[i];
        piece.m_color = BLACK;
        piece.m_kind = PAWN;
        piece.m_position = {uint8_t(i), 1};
    }
    blacks[8] = blacks[15] = {BLACK, {0, 0}, ROOK};
    blacks[15].m_position.x = 7;
    blacks[9] = blacks[14] = {BLACK, {1, 0}, KNIGHT};
    blacks[14].m_position.x = 6;
    blacks[10] = blacks[13] = {BLACK, {2, 0}, BISHOP};
    blacks[13].m_position.x = 5;
    blacks[11] = {BLACK, {3, 0}, QUEEN};
    blacks[12] = {BLACK, {4, 0}, KING};
    ChessPiece whites[16];
    for (size_t i = 0; i < 8; i++) {
        auto &piece = whites[i];
        piece.m_color = WHITE;
        piece.m_kind = PAWN;
        piece.m_position = {uint8_t(i), 6};
    }
    whites[8] = whites[15] = {
        WHITE,
        {0, 7},
        ROOK,
    };
    whites[15].m_position.x = 7;
    whites[9] = whites[14] = {WHITE, {1, 7}, KNIGHT};
    whites[14].m_position.x = 6;
    whites[10] = whites[13] = {WHITE, {2, 7}, BISHOP};
    whites[13].m_position.x = 5;
    whites[11] = {WHITE, {3, 7}, QUEEN};
    whites[12] = {WHITE, {4, 7}, KING};
    size_t i = 0;
    for (auto piece : blacks) {
        m_pieces[i++] = piece;
    }
    for (auto piece : whites) {
        m_pieces[i++] = piece;
    }
    for (auto &piece : m_pieces) {
        SetPieceSprite(piece);
        float x = network::NetworkPlayer::PLAYER_WHITE_PIECES
                      ? 700 - piece.m_position.x * 100
                      : piece.m_position.x * 100;
        float y = network::NetworkPlayer::PLAYER_WHITE_PIECES
                      ? 700 - piece.m_position.y * 100
                      : piece.m_position.y * 100;
        piece.m_sprite.setPosition(piece.m_position.x * 100 + 10, y + 10);
        piece.m_sprite.setScale(5, 5);
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
    for (auto &piece : m_pieces) {
        target.draw(piece.m_sprite);
    }
}

}  // namespace chess
}  // namespace zifmann
