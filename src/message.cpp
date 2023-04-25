#include "message.hpp"

#include <sstream>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace zifmann {
namespace chess {
namespace network {

std::string from(ushort number) {
    std::stringstream stream;
    stream << number;
    return stream.str();
}

std::string from(std::string string) {
    std::stringstream stream;
    stream << "\"" << string << "\"";
    return stream.str();
}

std::string makeJsonObject(std::unordered_map<std::string, std::string> props) {
    std::string result = "{";
    size_t count = 0;
    for (auto k : props) {
        if (k.second.empty()) {
            return "\"" + k.first + "\"";
        } else {
            result += from(k.first);
            result += ":" + k.second;
            if (count < props.size() - 1) {
                result += ",";
            }
        }
        count++;
    }
    result += "}";
    return result;
}

std::string OutgoingMessage::Serialize() {
    std::unordered_map<std::string, std::string> props;
    switch (this->type) {
        case Enqueue:
            props["Enqueue"] = "";
            break;
        case Dequeue:
            props["Dequeue"] = "";
            break;
        case Login:
            props["Login"] = std::get<std::string>(this->data);
            break;
        case LeaveGame:
            props["LeaveGame"] = "";
            break;
        case PlayAgain:
            props["PlayAgain"] = "";
            break;
        case MakeMove:
            auto details = std::get<MoveDetails>(this->data);
            auto inner = details.Serialize();
            props["MakeMove"] = inner;
            break;
    }
    return makeJsonObject(props);
}

std::string MoveDetails::Serialize() {
    std::unordered_map<std::string, std::string> props;
    props["piece"] = this->piece.Serialize();
    props["from"] = this->from.Serialize();
    props["to"] = this->to.Serialize();
    return makeJsonObject(props);
}

std::string Pos::Serialize() {
    std::unordered_map<std::string, std::string> props;
    props["x"] = from(this->x);
    props["y"] = from(this->y);
    return makeJsonObject(props);
}

std::string ChessPiece::Serialize() {
    std::unordered_map<std::string, std::string> props;
    auto color = this->color == White ? "White" : "Black";
    switch (this->variant) {
        case Rook:
            props[color] = from("Rook");
            break;
        case Knight:
            props[color] = from("Knight");
            break;
        case Bishop:
            props[color] = from("Bishop");
            break;
        case King:
            props[color] = from("King");
            break;
        case Queen:
            props[color] = from("Queen");
            break;
        case Pawn:
            props[color] = from("Pawn");
            break;
    }
    return makeJsonObject(props);
}

DeserializationResult Deserialize(std::string_view string) {}

}  // namespace network
}  // namespace chess
}  // namespace zifmann
