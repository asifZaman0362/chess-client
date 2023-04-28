#include "gamescene.hpp"

#include <SFML/Audio/SoundBuffer.hpp>
#include <array>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <utility>

#include "assetmanager.hpp"
#include "canvas.hpp"
#include "chesspiece.hpp"
#include "logger.hpp"
#include "prefs.hpp"
#include "stdinc.hpp"

namespace zifmann {
namespace chess {

sf::SoundBuffer *GetRandomAudioClip() {
    srand(time(0));
    int clip = rand() % 2;
    if (clip) {
        return AssetManager::GetAudioClip("creaturecomfort.ogg");
    } else
        return AssetManager::GetAudioClip("tranquility.ogg");
}

void CreateUI(GameScene &scene) {
    auto font = AssetManager::GetFont("kenneypixel.ttf");
    auto m_playerName = new CanvasText(
        "player", font, sf::IntRect(50, 50, 200, 40), Left | Top, Constant);
    auto m_opponentName =
        new CanvasText("opponent", font, sf::IntRect(50, 50, 200, 40),
                       Right | Bottom, Constant);
    scene.m_canvas.AddChild(m_playerName);
    scene.m_canvas.AddChild(m_opponentName);
}

std::array<std::pair<sf::Sprite, ChessPiece>, 32> GenerateSprites(
    std::array<ChessPiece, 32> pieces) {
    std::array<std::pair<sf::Sprite, ChessPiece>, 32> result;
    size_t i = 0;
    for (auto piece : pieces) {
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
        result[i++] = std::make_pair(sprite, piece);
    }
    return result;
}

std::array<ChessPiece, 32> GenerateChessPieces() {
    ChessPiece blacks[16];
    for (size_t i = 0; i < 8; i++) {
        auto &piece = blacks[i];
        piece.m_color = BLACK;
        piece.m_kind = PAWN;
        piece.m_position = {uint8_t(i), 0};
    }
    blacks[8] = blacks[15] = {BLACK, {0, 1}, ROOK};
    blacks[15].m_position.x = 7;
    blacks[9] = blacks[14] = {BLACK, {1, 1}, KNIGHT};
    blacks[15].m_position.x = 6;
    blacks[10] = blacks[13] = {BLACK, {2, 1}, BISHOP};
    blacks[15].m_position.x = 5;
    blacks[11] = {BLACK, {3, 1}, QUEEN};
    blacks[12] = {BLACK, {4, 1}, KING};
    ChessPiece whites[16];
    for (size_t i = 0; i < 8; i++) {
        auto &piece = whites[i];
        piece.m_color = WHITE;
        piece.m_kind = PAWN;
        piece.m_position = {uint8_t(i), 0};
    }
    whites[8] = whites[15] = {WHITE, {0, 1}, ROOK};
    whites[15].m_position.x = 7;
    whites[9] = whites[14] = {WHITE, {1, 1}, KNIGHT};
    whites[15].m_position.x = 6;
    whites[10] = whites[13] = {WHITE, {2, 1}, BISHOP};
    whites[15].m_position.x = 5;
    whites[11] = {WHITE, {3, 1}, QUEEN};
    whites[12] = {WHITE, {4, 1}, KING};
    std::array<ChessPiece, 32> pieces;
    size_t i = 0;
    for (auto piece : blacks) pieces[i++] = piece;
    for (auto piece : whites) pieces[i++] = piece;
    return pieces;
}

GameScene::GameScene() {
    m_board = ChessBoard();
    m_chessPiece.setTexture(*AssetManager::GetTexture("chesspieces.png"));
    m_chessPiece.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_chessPiece.setScale(6, 6);
    if (!m_chessPieceAnim.Create(&m_chessPiece, Vector2ui(16, 16),
                                 Vector2ui(0, 0), 5, 0.16f)) {
        log_error("failed to load animation!");
    } else {
        m_chessPieceAnim.Play();
    }
    m_bgmPlayer.setBuffer(*GetRandomAudioClip());
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        m_bgmPlayer.play();
    }
}

void GameScene::Render(sf::RenderTarget &target) {
    m_board.Render(target);
    target.draw(m_chessPiece);
}

void GameScene::Update(float dt) { m_chessPieceAnim.Update(dt); }

void GameScene::ProcessEvent(const sf::Event &event) {}

}  // namespace chess
}  // namespace zifmann
