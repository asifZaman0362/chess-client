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

template <typename T>
struct DeserializationResult {
    bool success;
    T value;
};

class Serializable {
   public:
    virtual std::string Serialize() const = 0;
};

template <typename T>
DeserializationResult<T> FromString(const std::string&);

class Pos : Serializable {
   public:
    Pos() = default;
    ushort x;
    ushort y;

    std::string Serialize() const;
};

enum OutgoingMessageType {
    Enqueue,
    Dequeue,
    MakeMove,
    LeaveGame,
    Login,
    PlayAgain,
    Disconnect
};

enum PieceVariant { Rook, Knight, Bishop, Queen, King, Pawn };

enum PieceColor { White, Black };

class ChessPiece : Serializable {
   public:
    ChessPiece(PieceColor color, PieceVariant variant)
        : color(color), variant(variant) {}
    PieceColor color;
    PieceVariant variant;

    std::string Serialize() const;
};

class MoveDetails : Serializable {
   public:
    MoveDetails(Pos from, Pos to, ChessPiece piece)
        : from(from), to(to), piece(piece) {}
    Pos from;
    Pos to;
    ChessPiece piece;

    std::string Serialize() const;
};

class OutgoingMessage : Serializable {
   public:
    OutgoingMessage(OutgoingMessageType type,
                    std::variant<std::string, MoveDetails> data)
        : type(type), data(data) {}
    OutgoingMessageType type;
    std::variant<std::string, MoveDetails> data;

    std::string Serialize() const;
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
    ushort from;
    ushort to;
};

struct Result_t {
    enum Status { Ok, Err } status;
    std::string error;
};

struct IncomingMessage {
    IncomingMessageType type;
    std::variant<PieceMoveData, Result_t, ushort, std::string> data;
};

DeserializationResult<IncomingMessage> FromString(const std::string&);

}  // namespace network
}  // namespace chess
}  // namespace zifmann

#endif
