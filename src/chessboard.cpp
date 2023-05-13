#include "chessboard.hpp"

#include <cstdint>
#include <cstring>
#include <variant>

#include "assetmanager.hpp"
#include "chesspiece.hpp"
#include "eventsystem.hpp"
#include "logger.hpp"
#include "message.hpp"
#include "networkclient.hpp"
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

ChessBoard::ChessBoard(EventSystem &eventSystem) {
    eventSystem.AddMouseListener(this);
    for (auto pieceColumn : m_piecePointers) {
        pieceColumn.fill(nullptr);
    }
    memset(m_config, 0, 64);
    m_config[0][0] = WHITE_PIECE | ROOK;
    m_config[0][1] = WHITE_PIECE | KNIGHT;
    m_config[0][2] = WHITE_PIECE | BISHOP;
    m_config[0][3] = WHITE_PIECE | QUEEN;
    m_config[0][4] = WHITE_PIECE | KING;
    m_config[0][5] = WHITE_PIECE | BISHOP;
    m_config[0][6] = WHITE_PIECE | KNIGHT;
    m_config[0][7] = WHITE_PIECE | ROOK;
    memset(m_config[1], PAWN | WHITE_PIECE, 8);
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
        auto tx = network::NetworkPlayer::PLAYER_WHITE_PIECES ? 7 - x : x;
        auto ty = network::NetworkPlayer::PLAYER_WHITE_PIECES ? 7 - y : y;
        squares[x][y] = sf::RectangleShape(sf::Vector2f(100, 100));
        squares[x][y].setFillColor((i + ((i / 8) % 2)) % 2 ? sf::Color::White
                                                           : sf::Color::Black);
        squares[x][y].setPosition(tx * 100, ty * 100);
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
        piece.m_sprite.setPosition(x + 10, y + 10);
        piece.m_sprite.setScale(5, 5);
    }
    for (int i = 0; i < 32; i++) {
        m_piecePointers[m_pieces[i].m_position.x][m_pieces[i].m_position.y] =
            m_pieces + i;
    }
    network::NetworkManager::AddCallback(
        network::IncomingMessageType::MovePiece,
        [this](network::IncomingMessage message) {
            auto pieceMoveDetails =
                std::get<network::PieceMoveData>(message.data);
            auto fy = pieceMoveDetails.from / 8;
            auto fx = pieceMoveDetails.from % 8;
            auto ty = pieceMoveDetails.to / 8;
            auto tx = pieceMoveDetails.to % 8;
            log_debug("moving piece from: %i, %i to %i, %i", fx, fy, tx, ty);
            MovePiece({fx, fy}, {tx, ty});
        },
        true);
    network::NetworkManager::AddCallback(
        network::IncomingMessageType::RemovePiece,
        [this](network::IncomingMessage message) {
            auto removePosition = std::get<ushort>(message.data);
            auto x = removePosition % 8;
            auto y = removePosition / 8;
            TakePiece(x, y);
        },
        true);
}

bool ChessBoard::TakePiece(uint8_t x, uint8_t y) {
    if (x > 8 || y > 8)
        return false;
    else if (!m_config[y][x])
        return false;  // empty
    // play animation of taking away the piece
    m_config[y][x] = 0;
    if (m_piecePointers[x][y] != nullptr) {
        m_piecePointers[x][y]->m_position = {8, 8};
        m_piecePointers[x][y]->m_sprite.setPosition(800, 200);
        m_piecePointers[x][y] = nullptr;
    }
    return true;
}

void ChessBoard::MovePiece(sf::Vector2i from, sf::Vector2i to) {
    auto piece = m_config[from.y][from.x];
    if (m_piecePointers[from.x][from.y] != nullptr) {
        log_debug("ehe");
        auto piece = m_piecePointers[from.x][from.y];
        piece->m_position = {(uint8_t)to.x, (uint8_t)to.y};
        float x = network::NetworkPlayer::PLAYER_WHITE_PIECES
                      ? 700 - piece->m_position.x * 100
                      : piece->m_position.x * 100;
        float y = network::NetworkPlayer::PLAYER_WHITE_PIECES
                      ? 700 - piece->m_position.y * 100
                      : piece->m_position.y * 100;
        piece->m_sprite.setPosition(x, y);
        m_piecePointers[from.x][from.y] = nullptr;
        m_piecePointers[to.x][to.y] = piece;
    }
    m_config[to.y][to.x] = piece;
    m_config[from.y][from.x] = 0;
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

int abs(int x) { return x * (int)(x >= 0) + (x * -1 * (int)(x < 0)); }

bool IsValidMove(uint8_t piece, int i, int j, int x, int y) {
    if (i < 8 && i >= 0 && j < 8 && j >= 0) {
        int dx = abs(x - i);
        int dy = abs(y - j);
        uint8_t pieceKind = piece & ~WHITE_PIECE;
        switch (pieceKind) {
            case PAWN: {
                if (dx == 0 && dy == 1) {
                    return true;
                }
                break;
            }
            case ROOK: {
                if ((dx == 0 && dy >= 1) || (dx >= 1 && dy == 0)) {
                    return true;
                }
                break;
            }
            case BISHOP: {
                if ((dx == dy && dy >= 1)) {
                    return true;
                }
                break;
            }
            case KNIGHT: {
                if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) {
                    return true;
                }
                break;
            }
            case KING: {
                if ((dx <= 1 && dy <= 1) && (dx == 1 || dy == 1)) {
                    return true;
                }
                break;
            }
            case QUEEN: {
                if ((dx == dy && dx >= 1) || (dx == 0 && dy >= 1) ||
                    (dx >= 1 && dy == 0)) {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

void ChessBoard::HighlightValidSquares() {
    // this is reversed as in x is column and y is row
    // because memsetting otherwise in 2d arrays is shit
    // i will fix this later, if it needs fixing that is :)
    int piece = m_config[picked_y][picked_x];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int dx = abs(picked_x - i);
            int dy = abs(picked_y - j);
            auto pieceKind = piece & ~WHITE_PIECE;
            squares[i][j].setOutlineColor(sf::Color::White);
            squares[i][j].setOutlineThickness(0);
            switch (pieceKind) {
                case PAWN: {
                    if (dx == 0 && dy == 1) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
                case ROOK: {
                    if ((dx == 0 && dy >= 1) || (dx >= 1 && dy == 0)) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
                case BISHOP: {
                    if ((dx == dy && dy >= 1)) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
                case KNIGHT: {
                    if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
                case KING: {
                    if ((dx <= 1 && dy <= 1) && (dx == 1 || dy == 1)) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
                case QUEEN: {
                    if ((dx == dy && dx >= 1) || (dx == 0 && dy >= 1) ||
                        (dx >= 1 && dy == 0)) {
                        squares[i][j].setOutlineColor(sf::Color::Yellow);
                        squares[i][j].setOutlineThickness(5);
                    }
                    break;
                }
            }
        }
    }
}

void ChessBoard::PickSquare(const sf::Vector2i mousePosition) {
    if (picked_x < 8 && picked_x >= 0 && picked_y < 8 && picked_y >= 0) {
        squares[picked_x][picked_y].setOutlineColor(sf::Color::Black);
        squares[picked_x][picked_y].setOutlineThickness(0);
    }
    auto mouse = mousePosition;
    // get x, y for tile from mouse position
    auto pos = mouse / 100;
    if (network::NetworkPlayer::PLAYER_WHITE_PIECES) {
        picked_x = 7 - pos.x;
        picked_y = 7 - pos.y;
    } else {
        picked_x = pos.x;
        picked_y = pos.y;
    }
    if (picked_x < 8 && picked_x >= 0 && picked_y < 8 && picked_y >= 0) {
        squares[picked_x][picked_y].setOutlineColor(sf::Color::Green);
        squares[picked_x][picked_y].setOutlineThickness(5);
    }
}

void ChessBoard::OnMouseButtonDown(sf::Mouse::Button button) {
    if (picked_x < 8 && picked_x >= 0 && picked_y < 8 && picked_y >= 0) {
        auto pieceInfo = m_config[picked_y][picked_y];
        if (selected_x < 8 && selected_y < 8) {
            // Something has already been picked
            // Make a move instead
            auto selected = m_config[selected_y][selected_x];
            auto color = ((selected & WHITE_PIECE) == WHITE_PIECE)
                             ? network::PieceColor::White
                             : network::PieceColor::Black;
            auto variant = selected & ~WHITE_PIECE;
            network::PieceVariant pvariant;
            switch (variant) {
                case ROOK:
                    pvariant = network::PieceVariant::Rook;
                    break;
                case KNIGHT:
                    pvariant = network::PieceVariant::Knight;
                    break;
                case BISHOP:
                    pvariant = network::PieceVariant::Bishop;
                    break;
                case KING:
                    pvariant = network::PieceVariant::King;
                    break;
                case QUEEN:
                    pvariant = network::PieceVariant::Queen;
                    break;
                case PAWN:
                    pvariant = network::PieceVariant::Pawn;
                    break;
            }
            network::ChessPiece piece(color, pvariant);
            network::Pos from, to;
            from.x = selected_x;
            from.y = selected_y;
            to.x = picked_x;
            to.y = picked_y;
            network::MoveDetails details(from, to, piece);
            network::OutgoingMessage message(
                network::OutgoingMessageType::MakeMove, details);
            network::NetworkManager::SendMessage(message);
            selected_x = 8;
            selected_y = 8;
        } else {
            auto color = (pieceInfo & WHITE_PIECE) ==
                         WHITE_PIECE;  // false => Black, true => White
            if (color == network::NetworkPlayer::PLAYER_WHITE_PIECES) {
                selected_x = picked_x;
                selected_y = picked_y;
                HighlightValidSquares();
            }
        }
    }
}

void ChessBoard::OnMouseButtonUp(sf::Mouse::Button button) {}

void ChessBoard::OnMouseMove(sf::Vector2i position) { PickSquare(position); }

}  // namespace chess
}  // namespace zifmann
