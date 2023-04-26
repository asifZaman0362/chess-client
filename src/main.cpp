#include <cassert>

#include "game.hpp"
#include "message.hpp"

int main(int argc, char **argv) {
    using namespace zifmann::chess;
    Game game;
    return game.Start();
}
