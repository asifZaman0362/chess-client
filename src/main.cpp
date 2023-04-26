#include "game.hpp"
#include "message.hpp"

void test_json_deserialize();

int main(int argc, char **argv) {
    /*using namespace zifmann::chess;
    Game game;
    return game.Start();*/
    test_json_deserialize();
}

void test_json_deserialize() {
    using namespace zifmann::chess::network;
    auto message = std::string("\"GameStarted\"");
    auto res = FromString<IncomingMessage>(message);
    assert(res.success);
    assert(res.value.type == IncomingMessageType::GameStarted);
    message = "{\"MovePiece\":{\"from\":10,\"to\":5}}";
    res = FromString<IncomingMessage>(message);
    assert(res.success);
    assert(res.value.type == zifmann::chess::network::MovePiece);
}
