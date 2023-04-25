#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <sys/types.h>

#include <cstddef>
#include <string>
#include <string_view>
#include <variant>

namespace zifmann {
namespace chess {
namespace network {

class Serializable {
   public:
    virtual std::string Serialize() = 0;
};

class Pos : Serializable {
   public:
    Pos() = default;
    ushort x;
    ushort y;

    std::string Serialize();
};

enum OutgoingMessageType {
    Enqueue,
    Dequeue,
    MakeMove,
    LeaveGame,
    Login,
    PlayAgain
};

enum PieceVariant { Rook, Knight, Bishop, Queen, King, Pawn };

enum PieceColor { White, Black };

class ChessPiece : Serializable {
   public:
    ChessPiece() = default;
    PieceColor color;
    PieceVariant variant;

    std::string Serialize();
};

class MoveDetails : Serializable {
   public:
    MoveDetails() = default;
    Pos from;
    Pos to;
    ChessPiece piece;

    std::string Serialize();
};

class OutgoingMessage : Serializable {
   public:
    OutgoingMessage() = default;
    OutgoingMessageType type;
    std::variant<std::string, MoveDetails> data;

    std::string Serialize();
};

enum IncomingMessageType {
    MovePiece,
    RemovePiece,
    Check,
    Checkmate,
    WinGame,
    LoseGame,
    GameStarted,
    Result,
};

struct PieceMoveData {
   public:
    ushort from;
    ushort to;
};

struct Result_t {
   public:
    enum Status { Ok, Err } status;
    std::string error;
};

struct IncomingMessage {
   public:
    IncomingMessage() = default;
    IncomingMessageType type;
    std::variant<PieceMoveData, Result_t, ushort, std::string> data;
};

struct DeserializationResult {
    bool success;
    IncomingMessage value;
};

DeserializationResult Deserialize(std::string_view string);

}  // namespace network
}  // namespace chess
}  // namespace zifmann

#endif
