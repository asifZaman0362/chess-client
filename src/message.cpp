#include "message.hpp"

#include <cstdint>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>

#include "simdjson.h"

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

std::string OutgoingMessage::Serialize() const {
    std::unordered_map<std::string, std::string> props;
    switch (this->type) {
        case Ping:
            props["Ping"] = "";
            break;
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
        case Disconnect:
            props["Disconnect"] = "";
            break;
        case MakeMove:
            auto details = std::get<MoveDetails>(this->data);
            auto inner = details.Serialize();
            props["MakeMove"] = inner;
            break;
    }
    return makeJsonObject(props);
}

std::string MoveDetails::Serialize() const {
    std::unordered_map<std::string, std::string> props;
    props["piece"] = this->piece.Serialize();
    props["from"] = this->from.Serialize();
    props["to"] = this->to.Serialize();
    return makeJsonObject(props);
}

std::string Pos::Serialize() const {
    std::unordered_map<std::string, std::string> props;
    props["x"] = from(this->x);
    props["y"] = from(this->y);
    return makeJsonObject(props);
}

std::string ChessPiece::Serialize() const {
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

// this is one big mess
// TODO: Fix code and make things less nested

template <>
DeserializationResult<IncomingMessage> FromString(const std::string& str) {
    DeserializationResult<IncomingMessage> res;
    IncomingMessage data;
    simdjson::dom::parser parser;
    auto padded = simdjson::padded_string(str);
    auto doc = parser.parse(padded);
    if (doc.error()) {
        res.success = false;
        return res;
    }
    if (doc.is_string()) {
        auto string = doc.get_string().value();
        if (string == "GameStarted") {
            data.type = GameStarted;
            res.success = true;
            res.value = data;
        } else {
            res.success = false;
        }
        return res;
    } else {
        if (doc.is_object()) {
            for (auto field : simdjson::dom::object(doc)) {
                if (field.key == "MovePiece" && field.value.is_object()) {
                    auto inner = simdjson::dom::object(doc["MovePiece"]);
                    PieceMoveData details;
                    if (auto to = inner["to"].get_uint64(),
                        from = inner["from"].get_uint64();
                        !to.error() && !from.error()) {
                        details.to = to.value();
                        details.from = from.value();
                        res.success = true;
                        data.type = MovePiece;
                        data.data = details;
                    } else
                        res.success = false;
                } else if (field.key == ("RemovePiece")) {
                    auto inner = simdjson::dom::object(doc["RemovePiece"]);
                    if (auto at = inner["at"].get_uint64(); !at.error()) {
                        res.success = true;
                        data.type = RemovePiece;
                        data.data = ushort(at.value());
                    } else
                        res.success = false;
                } else if (field.key == ("Result")) {
                    auto inner = simdjson::dom::object(doc["Result"]);
                    data.type = Result;
                    if (auto err = inner["Err"].get_string(); !err.error()) {
                        std::string error = std::string(err.value());
                        res.success = true;
                        data.data = Result_t{Result_t::Status::Ok, error};
                    } else if (!inner["Ok"].error()) {
                        res.success = true;
                        data.data = Result_t{Result_t::Status::Ok, ""};
                    } else {
                        res.success = false;
                    }
                } else if (field.key == "Check") {
                    auto inner = doc["Check"];
                    if (inner.is_object()) {
                        data.type = Check;
                        if (auto winner = inner["checker"].get_string();
                            !winner.error()) {
                            std::string winner_str =
                                std::string(winner.value());
                            data.data = winner_str;
                            res.success = true;
                        } else
                            res.success = false;
                    } else
                        res.success = false;
                } else if (field.key == ("Checkmate")) {
                    auto inner = doc["Checkmate"];
                    if (inner.is_object()) {
                        data.type = Checkmate;
                        if (auto winner = inner["winner"].get_string();
                            !winner.error()) {
                            data.data = std::string(winner.value());
                            res.success = true;
                        } else
                            res.success = false;
                    } else
                        res.success = false;
                } else if (field.key == ("WinGame")) {
                    auto inner = doc["WinGame"];
                    if (inner.is_object()) {
                        data.type = WinGame;
                        if (auto reason = inner["reason"].get_string();
                            !reason.error()) {
                            data.data = std::string(reason.value());
                            res.success = true;
                        } else
                            res.success = false;
                    } else
                        res.success = false;
                } else if (field.key == ("LoseGame")) {
                    auto inner = doc["LoseGame"];
                    if (inner.is_object()) {
                        data.type = LoseGame;
                        if (auto reason = inner["reason"].get_string();
                            !reason.error()) {
                            data.data = std::string(reason.value());
                            res.success = true;
                        } else
                            res.success = false;
                    } else
                        res.success = false;
                } else {
                    res.success = false;
                }
            }
        }
        res.value = data;
        return res;
    }
}

}  // namespace network
}  // namespace chess
}  // namespace zifmann
